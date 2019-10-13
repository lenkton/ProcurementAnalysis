// analyzer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include "C:\Users\Сергей\source\repos\proAna\proAna\proAna.h"
using std::vector;				using std::string;
using std::filesystem::path;	using std::sort;

struct Statistics {
	int total;
	int single;
	
};

struct Contract {
	string OKPD;
	string name;
	bool isSingle = false; 
	string price;			//TODO:multiple objects can occure
};

bool CompCon(const Contract& a, const Contract& b) {
	int counter = 0;
	for (char c : a.OKPD)
		if (c < b.OKPD[counter++])
			return true;
		else if (c > b.OKPD[counter - 1])
			return false;
	return false;
}

 
bool CopmConPrice(const Contract& a, const Contract& b) {
	if (a.price.length() > b.price.length())
		return false;
	if (a.price.length() < b.price.length())
		return true;
	size_t counter = 0;
	for (char c : a.price) {
		if (c < b.price[counter])
			return true;
		else if (c > b.price[counter])
			return false;
		counter++;
	}
	return false;
}

bool Contains(std::vector<std::string>& v, const std::string& key) {
	for (auto s : v) {
		if (s.find(key) != std::string::npos)
			return true;
	}
	return false;
}
string getField(const string& line) {
	size_t left, right = 0;
	if (((left = line.find("<")) != std::string::npos)
		&& ((right = line.find(">")) != std::string::npos)
		)
		return line.substr(left + 1, right - left - 1);
	return "";
}

string getContent(const string & line)
{
	size_t  right, tmp = 0;

	if ((tmp = line.find_last_of("<")) != (line.find("<"))) {
		right = line.find(">");
		//std::cout << line << '\n';
		return line.substr(right + 1, tmp - right - 1);
	}
	//right = line.find(">");
	//if (line[right +1] != '\0') {
		//std::cout << "\n****************\n"<<line<<"\n\n\n";
		//return line.substr(right + 1, line.length() - right - 1);
	//}
	//std::cout << line<<'\n';
	return "";
}




int LineProcessingOKPD(std::string& line, Contract& con)

{
	static vector<string> v;
	if (line.find("<") == std::string::npos)
		return 0;

	size_t tmp;
	if (line[tmp = line.find("<") + 1] == '/') {
		v.pop_back();
		return 0;
	}
	if (line[tmp] == '?') {
		return 0;
	}



	std::string field = getField(line);
	if (field._Equal("singleCustomer")) {
		con.isSingle = true;
	}
	std::string content = getContent(line);
	if (content.empty()) {
		v.push_back(field);
		return 0;
	}

	if (field._Equal("priceRUR"))
	{
		con.price = content;
	}

	if (field._Equal("code") && content[0] == '4' && Contains(v, "OKPD"))
	{
		con.OKPD = content;
		return 0;
	}



	return 0;
}


int FillCon(path pat,Contract& con) {
	std::ifstream file(pat);
	string line;
	while (std::getline(file, line) )
		LineProcessingOKPD(line,con);
	file.close();
	return 0;
}

int main()
{
	std::ifstream log;
	log.open("C:/analyzer/log.txt");

	std::ofstream output;
	output.open("C:/analyzer/output.txt");
	Statistics stat; stat.single = 0; stat.total = 0;
	Contract con;
	vector<Contract> vc;
	string line;
	path pat;
	int counter=0;
	while (std::getline(log, line))
		switch (counter++ % 6) {
		case 0:
			con.name = line;
			break;
		case 2:
			pat = line;
			FillCon(pat, con);
			
			break;
		case 5:
			vc.push_back(con);
			if (con.isSingle) {
				stat.single++;
			}
			stat.total++;
			con = {};
			con.isSingle = false;

		}
	log.close();
	//sort
	sort(vc.begin(), vc.end(), CopmConPrice);
	//output
	for (Contract c : vc) {
		output << c.OKPD << "\n" << c.name << "\n" << string('*', 20) << "\n";
		output << (c.isSingle ? "true":"false" )<< "\n";
		output << c.price << "\n";
		output << pat.string();

	}
	output.close();
	std::cout << "Total :" << stat.total << "\nSingle:" << stat.single << "\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
