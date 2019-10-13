// Analysis.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "Web.h"
#include "Analysis.h"
#include <filesystem>
#include <string>
#include <io.h>
#include <fcntl.h>
#include "Sum.h"
#include <Windows.h>
#include <ctime>

std::wofstream mylog;

int main()
{
	int a = _setmode(_fileno(stdout), _O_U16TEXT);
	a = _setmode(_fileno(stdin), _O_U16TEXT);
	a = _setmode(_fileno(stderr), _O_U16TEXT);

	time_t vremya = time(NULL);

	size_t n=1;
	while (std::filesystem::exists(L"C:/Analysis/log"+std::to_wstring(++n)+L".txt")) {

	}
	mylog.open(L"C:/Analysis/log" + std::to_wstring(n++) + L".txt");

	int i = 1;
	if(i ==0)
	{
		std::ifstream file(L"C:/Analysis/Web/10_supplier-results.html");

		FileStats fs;
		ParceHTML1File(file,fs);
		file.close();
		for (auto s : fs)
			std::wcout << s << std::endl;
	}



	if (i==1)
	{
		
		std::wcout << L"Hello World!\n";
		//for (p:std::filesystem::directory_iterator)...
		wstring adr;
		std::wcin >> adr;
		std::filesystem::path pat = adr; //L"C:\\NewDir1\\ContractsRegs\\Altaj";
		wstring reg = pat.wstring().substr(pat.wstring().find_last_of(L"\\")+1);

		Request request;
		Table stat;

		FillRequestAndHeader(request,stat);

		std::wcout << L"DirProcessing()"<<std::endl;
		Table stat1 = DirProcessing(pat, request);
		stat.insert(stat.end(), stat1.begin(),stat1.end());

		std::wcout << L"GetWebInfo()" << std::endl;
		std::filesystem::path webPath = L"C:/Analysis/Web/";
		GetWebInfo(stat, request, NOTIF_NUM, webPath);//nums of contracts, ....

		std::wcout << L"Print()" << std::endl;
		std::ofstream table;
		table.open(L"C:/Analysis/output" + reg + L".csv");
		Print(request, stat, table,reg);
		table.close();
	}

	mylog.close();
	std::wcout <<L"It took: "<< std::to_wstring(time(NULL) - vremya) << std::endl;
}

int FillRequestAndHeader(Request& request,Table& stat) {

	//FileStats fs;
	ComplexTag ct;
	vector<ComplexTag> vct;


	stat.header.push_back(L"notificationNumber");
	ct.outerTag = L"order";
	ct.Tag = L"notificationNumber";			//must be with no outer tag
	vct.push_back(ct);
	request.push_back(vct);
	vct.clear();
	
	stat.header.push_back(L"OKPD2orKTRU");
	ct.outerTag = L"OKPD2";
	ct.Tag = L"code";
	vct.push_back(ct);

	ct.outerTag = L"KTRU";
	ct.Tag = L"code";
	vct.push_back(ct);
	request.push_back(vct);
	vct.clear();

	stat.header.push_back(L"advancePercent");
	ct.outerTag = L"advancePaymentSum";
	ct.Tag = L"sumInPercents";
	vct.push_back(ct);
	request.push_back(vct);
	vct.clear();

	stat.header.push_back(L"ContractSum");
	ct.outerTag = L"";
	ct.Tag = L"sum";
	vct.push_back(ct);
	request.push_back(vct);
	vct.clear();

	stat.header.push_back(L"isSingleCustomer");
	ct.outerTag = L"singleCustomer";					//TODO: обработать в чтении заявок и в Print()
	ct.Tag = L"";
	vct.push_back(ct);
	request.push_back(vct);
	vct.clear();

	stat.header.push_back(L"ProtocolDate");
	ct.outerTag = L"";
	ct.Tag = L"protocolDate";
	vct.push_back(ct);
	request.push_back(vct);
	//vct.clear();

	return 0;
}


int Print(const Request& request,const Table& stat, std::ostream& os,const wstring& reg) {
	/*os << L"#;SubRF;";
	size_t counter = 0;
	for (auto a : request) {
		os << (a[0].Tag.empty()?a[0].outerTag: a[0].Tag) << L";";
	}
	os << std::endl;*/
	os << "N;Reg;";
	for (auto s : stat.header) {	os << utf8_encode(s) << ";";}
	os << std::endl;
	size_t counter = 0;
	for (Table::const_iterator fs = stat.begin(); fs < stat.end();fs++) {
		os << ++counter<<";"<< utf8_encode(reg)<<";";
		for (auto s:(*fs)) {
			os << utf8_encode(s) <<";";
		}
		os << std::endl;
	}
	return 0;
}

Table DirProcessing(const std::filesystem::path& MainPath, const Request& request) 
{
	Table t;
	int counter = 0;
#pragma omp parallel for
	for (auto p : std::filesystem::recursive_directory_iterator(MainPath))
		if (!(p.is_directory()))
		{
			t.push_back(FileProcessing(p.path(), request));
		}
		else {
			std::wcout << p.path().wstring() << std::endl;
			//if (counter++ == 1)	return t;
		}
	return t;
}

FileStats FileProcessing(const std::filesystem::path& p,const Request& request) {
	static size_t counter = 0;
	FileStats fs;
	fs.resize(request.size());
	std::wifstream file;
	wstring line;

	file.open(p);
	counter++;
	while (std::getline(file, line))
	{
		ParceLine(fs,request,line);

	}
	file.close();

	if (fs[NOTIF_NUM].empty()) {
		mylog <<counter<< L". Empty Notification Number: " << p.wstring() << std::endl;
	}

	return fs;

}


int ParceLine(FileStats& fs, const Request& request, wstring& line) {
	static wstring outerTag = L"";
	size_t l=0;
	if ((l = line.find(L"<"))==wstring::npos)
		return 0;
	size_t r = line.find(L">");
	wstring tag = line.substr(l + 1, r - l - 1);
	if (r+1 == line.length()) {
		
		if (tag[0] == L'/') {
			outerTag.clear();
			return 0;
		}
		else {
			outerTag = tag;
			CheckTags(fs, request, outerTag, tag, line);
		}
	}
	else {
		CheckTags(fs,request,outerTag,tag, line);

	}
	return 0;

}

int CheckTags(FileStats& fs,const Request& request,
	const wstring& outerTag,const wstring& tag, const wstring& line)
{
	int counter = 0;
	for (vector<ComplexTag> ctv : request) {
		for (ComplexTag ct : ctv) {
			if (ct.Tag._Equal(tag)&&(/*ct.outerTag.empty() ||*/ct.outerTag._Equal(outerTag)))		//TODO: make it outer if
			{
				size_t tmp = line.find(L">") + 1;
				fs[counter] = line.substr((tmp), line.find_last_of(L"<") - tmp);
				return 0;
			}
			if (ct.Tag.empty()&&outerTag._Equal(ct.outerTag)) {
				fs[counter] = L"1";
			}
		}
		counter++;
	}
	return 1;
}