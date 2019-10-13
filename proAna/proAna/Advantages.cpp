#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include "Windows.h"
#include "proAna.h"

std::string Utf8_to_cp1251(const char* str);
//TODO: parse xml's not by tags but by OKPD num
//TODO: create special macros for copying substring
//TODO: сделать вывод нормального названия

//I will try out creating a new directory for each of the FileData's,
//naming downloads according to their <fileName>'s 

int CheckForAdv(std::vector<FileData> fdv) {
	std::filesystem::path root = "C:/contracts/";
	if (!std::filesystem::exists(root))
		std::filesystem::create_directory(root);
	else;
	for (auto fd : fdv) {
		//std::string foldername = root.u8string()+ fd.name;
		//std::cout << foldername << "\n";
		std::string folderName = (Utf8_to_cp1251(fd.name.c_str()));
		folderName = folderName.substr(0, 24);
		std::filesystem::path fldr = root / folderName; //std::filesystem::u8path( foldername);
		if (!std::filesystem::exists(fldr))
			std::filesystem::create_directory(fldr);
		else;
		/*
		for (auto a : fd.att) {
			std::string converted1251 = Utf8_to_cp1251((fldr.u8string() + "/" + a.name).c_str());

			HRESULT hr = URLDownloadToFileA(FALSE, a.URL.c_str(), converted1251.c_str(), 0, NULL);
			//DownloadURLs(fldr, fdv);
			if (hr == S_OK)
				std::cout << "OK";

			std::cout << hr;
		}
		*/
	}
	return 0;
}


std::string Utf8_to_cp1251(const char* str)
{
	std::string res;
	int result_u, result_c;


	result_u = MultiByteToWideChar(CP_UTF8,	0, str, -1, 0, 0);

	if (!result_u)
		return "";

	wchar_t* ures = new wchar_t[result_u];

	if (!MultiByteToWideChar(CP_UTF8,0,str,-1,ures,result_u))
	{
		delete[] ures;
		return "";
	}


	result_c = WideCharToMultiByte(	1251,0,ures,-1,0,0,0, 0);

	if (!result_c)
	{
		delete[] ures;
		return "";
	}

	char* cres = new char[result_c];

	if (!WideCharToMultiByte(1251,0,ures,-1,cres,result_c,0, 0))
	{
		delete[] cres;
		return "";
	}
	delete[] ures;
	res.append(cres);
	delete[] cres;
	return res;
}