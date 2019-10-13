#pragma once
#include <vector>
#include <filesystem>
#include <string>

//defines
#define NOTIF_NUM 0 

extern std::wofstream mylog;


using std::wstring; using std::vector; using std::filesystem::path;

class DistribAtom {
public: 
	unsigned int num;
};
//class SumStat:Statistica{public unsigned int num;};


//types

typedef vector<wstring> FileStats;


/*struct FileStats {
	string OKPD;
	string Payment;
	string Advance;

};*/

struct ComplexTag {
	wstring outerTag;
	wstring Tag;
};

typedef std::vector<std::vector<ComplexTag>> Request;
//typedef  std::vector<FileStats> Table;
class Table {
	std::vector<FileStats> v;
	
public: 
	FileStats header;

	size_t size() const { return v.size(); }
	FileStats& operator[](size_t n) { return v[n]; }
		FileStats const operator[](size_t n)const  { return v[n]; }
		void push_back(const FileStats& b) { v.push_back(b); }
		typedef vector<FileStats>::iterator iterator;
		typedef vector<FileStats>::const_iterator const_iterator;

		void insert(const_iterator a, iterator b, iterator c) { v.insert(a, b, c); }
		iterator begin() { return v.begin(); }
		iterator end() { return v.end(); }

		const_iterator begin() const { return v.begin(); }
		const_iterator end() const { return v.end(); }
};
//funcs
int main();

int FillRequestAndHeader(Request& request, Table& stat);



int Print(const Request& request,const Table& stat, std::ostream&, const wstring&);

Table DirProcessing(const std::filesystem::path& MainPath, const Request& request);
FileStats FileProcessing(const std::filesystem::path&, const Request& request);
int ParceLine(FileStats& fs, const Request& request, wstring& line);

int CheckTags(FileStats& fs, const Request& request, const wstring& outerTag, const wstring& tag, const wstring& line);
