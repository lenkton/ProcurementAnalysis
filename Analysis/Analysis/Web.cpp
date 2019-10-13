#include "Web.h"
#include "Analysis.h"
#include <Windows.h>
#include <vector>
#include <filesystem>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using std::wstring; using std::vector; using std::wcout;


int GetWebInfo(Table& stat, const Request& request, const size_t& n,
	const std::filesystem::path& MainPath)
{
	size_t counter = 1;
	vector<wstring>HTMLs,HTML1s;
	if (!std::filesystem::exists(MainPath)) {
		std::filesystem::create_directory(MainPath);
	}
	
#pragma omp parallel for
	for (Table::iterator s = stat.begin(); s<stat.end();s++)
		/*FileStats s : stat*/
	{
		if ((*s)[n].empty()) { counter++; HTMLs.push_back(L""); HTML1s.push_back(L""); continue; }

		wstring fileNameHTML = DownloadResultsInfo((*s)[n], MainPath, counter,L"common-info");
		if (!fileNameHTML.empty()) {
			HTMLs.push_back(fileNameHTML);
			//ParceHTML(*s, fileNameHTML);
			//std::filesystem::remove(fileNameHTML);
		}
		else {
			HTMLs.push_back(fileNameHTML);
			Sleep(10000);
			continue;
		}	
		fileNameHTML = DownloadResultsInfo((*s)[n], MainPath, counter, L"supplier-results");
		if (!fileNameHTML.empty()) {
			HTML1s.push_back(fileNameHTML);
			//ParceHTMLSupplier(*s, fileNameHTML);
			//std::filesystem::remove(fileNameHTML);
		}
		else {
			HTML1s.push_back(fileNameHTML);
			Sleep(10000);
			continue;
		}
		counter++;
		
	}

	ParceHTML(stat, HTMLs);
	ParceHTML1(stat, HTML1s);
	std::filesystem::remove_all(MainPath);

	for (auto &fs : stat) {
		if(fs.size()<WEB_WIDTH)
			for (size_t i = fs.size(); i < WEB_WIDTH; i++) {
				fs.push_back(L"");
			}
	}
	return 0;
}



int ParceHTML(Table& table,const vector<wstring>& p) {
	table.header.push_back(L"NMCK");
	Table::iterator ti; vector<wstring>::const_iterator pi;
	int counter = 0;
#pragma omp parallel for
	for (ti = table.begin(), pi = p.begin(); ti < table.end() && pi < p.end(); ti++, pi++)
	{
		if ((*pi).empty()) {
			(*ti).push_back(L"");
			continue;
		}
		std::ifstream file(*pi);
		if (file.is_open()) {
			//...
			wstring wline, NMCK;
			std::string line;
			while (std::getline(file, line))
			{
				wline=strtowstr(line);
				if (wline.find(L"Ќачальна€ (максимальна€) цена контракта</td>") != wstring::npos) {
					NMCK = GetNMCK(file);
					break;
				}
			}
			file.close();
			(*ti).push_back(NMCK);
		}	
	}
	return 0;
}


wstring GetNMCK(std::ifstream& is) {
	wstring NMCK,wline;
	std::string line,inp;
	do {
		is >> inp;
		line += inp;
	} while (inp.find("</td>") == std::string::npos);
	

	wline = strtowstr(line);
	NMCK = wline.substr(wline.find(L"<td>") + 4, wline.find(L"</td>") - wline.find(L"<td>") - 4);

	return NMCK;
}

wstring DownloadResultsInfo(const wstring& regNum, const path& p, const size_t& ordNum,
	const wstring& type) {
	if (regNum.empty())
		return L"";

	wstring file = p.wstring() + std::to_wstring(ordNum)+L"_" +type+ L".html";
	wstring URL = L"http://zakupki.gov.ru/epz/order/notice/ea44/view/"+ type +L".html?regNumber=" + regNum;
	HRESULT b = URLDownloadToFileW(NULL, URL.c_str(), file.c_str(), 0, NULL);
	if (b != S_OK) {
		if (b == INET_E_DOWNLOAD_FAILURE)
			mylog <<ordNum<< L". Internet Issues! Contract Num: " << regNum << std::endl;
		else
			mylog << L"Download Issues! Contract Num: " << regNum << std::endl;
	}
	else {
		return file;
	}
	return L"";
}



std::wstring strtowstr(const std::string& str)
{
	// Convert an ASCII string to a Unicode String
	std::wstring wstrTo;
	wchar_t* wszTo = new wchar_t[str.length() + 1];
	wszTo[str.size()] = L'\0';
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wszTo, (int)str.length());
	wstrTo = wszTo;
	delete[] wszTo;
	return wstrTo;
}
std::string utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}



int ParceHTML1(Table& table, const vector<wstring>& p) {
	table.header.push_back(L"NumOfParticipants");
	table.header.push_back(L"Participant1");
	table.header.push_back(L"Participant2");
	Table::iterator ti; vector<wstring>::const_iterator pi;
	int counter = 0;

#pragma omp parallel for
	for (
			ti = table.begin(), pi = p.begin();
			ti < table.end() && pi < p.end();
			ti++, pi++
		)
	{
		if ((*pi).empty()) {
			(*ti).push_back(L""); (*ti).push_back(L""); (*ti).push_back(L"");
			continue;
		}
		std::ifstream file(*pi);
		if (file.is_open()) {
			ParceHTML1File(file, *ti);
		}
		file.close();
	}
	return 0;
}

int ParceHTML1File(std::ifstream& file, FileStats& fs) {
	wstring wline, NMCK;
	vector<wstring> result;
	size_t counter=0;
	while (!(NMCK = GetContent(file, L"<span style=\"white-space: nowrap;\">", L"</span>")).empty())
		//result.push_back(NMCK);
	{
		result.push_back(NMCK);
		counter++;
	}
	
	fs.push_back(std::to_wstring(counter));
	//for (auto s : result) std::wcout << s<<std::endl;
	for (int i = 0; i < 2; i++) fs.push_back(result.size() > i ? result[i] : L"");

	
	return 0;
}





wstring GetContent(std::ifstream& is, const std::wstring& tagStart, const std::wstring& tagEnd) {
	wstring content, wline;
	std::string line, inp,ends;
	size_t a, b,counter=0;
	ends = utf8_encode(tagStart);
	
	do {
		if (!(getline(is, inp))) { return L""; }
		counter++;
	} while (inp.find(ends)==std::string::npos);
	
	line = inp;

	ends = utf8_encode(tagEnd);
	do {
		if (!(is >> inp)) {
			return L"";
		}
		line += inp;
	} while (inp.find(ends) == std::string::npos);

	wline = strtowstr(line);

	a = wline.find(tagStart) + tagStart.length();
	b = wline.find(tagEnd) - a;
	content = wline.substr(a, b);//must be optimized

	return content;
}