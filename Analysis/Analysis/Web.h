#pragma once
#include "Analysis.h"

#define WEB_WIDTH 10

int GetWebInfo(Table&, 
	const Request&, const size_t&, 
	const path&);

int ParceHTML1(Table& table, const vector<wstring>& p);

int ParceHTML1File(std::ifstream& file, FileStats& fs);

wstring GetContent(std::ifstream& is, const std::wstring& tagStart, const std::wstring& tagEnd);

int ParceHTML(Table& table, const vector<wstring>& p);

wstring GetNMCK(std::ifstream& is);

wstring DownloadResultsInfo(const wstring& regNum, const path& p, const size_t& ordNum, const wstring& type);


std::wstring strtowstr(const std::string& str);

std::string utf8_encode(const std::wstring& wstr);
