// proAna.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include "Windows.h"
#include "proAna.h"


strucedData GetData(std::ifstream& data) {
	strucedData sd;
	std::string tmp;
	while (std::getline(data, tmp)) {
		sd.push_back(tmp);
		std::cout << tmp << "\n";
	}
	return sd;
}

int SetUp(std::ifstream& file, std::ifstream& data, strucedData& sd) {

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	data.open("Data.txt",std::ios::binary);
	//setlocale(LC_ALL, "Russian");
	//std::locale loc("ru_RU.UTF-8");
	//file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
	 sd = GetData(data);
	return 0;
}

int ShutDown(std::ifstream& file, std::ifstream& data) {
	data.close();
	return 0;
}

int main()

{
	//Unzip();
	std::ifstream file, data;
	strucedData sd;
	std::vector<FileData> fdv;

	//std::filesystem::path dir = "C:/NewDir1/contracts/contract_Permskij_kraj_2019020100_2019030100_004.xml";

	SetUp(file, data,sd);

	std::filesystem::path dirPath = "C:\\NewDir1\\ContractsRegs\\Kemero";
	fdv = DirProcessing(file, sd, dirPath);

	/*
	std::vector<Param> params;

	Param par;
	par.Name = "OKPD";
	par.content ="41"
	params.push_back()
	*/
	//parces XML-files in directory and returns vector of data's w/ info
	//about contracts with requested params

	/////////////////fdv = ParceDir(dir);
	std::cout << fdv.size() << std::endl;

	//CheckForAdv(fdv);

	ShutDown(file,data);
	
	
	
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
