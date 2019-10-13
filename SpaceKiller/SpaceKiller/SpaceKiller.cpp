// SpaceKiller.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <sstream>
#include <fstream>
#include <codecvt>

#include <cstdlib>








#define NMCK_POS 3
#define WIDTH 12

int main()
{
    std::cout << "Hello World!\n"; 
	std::locale::global(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstring filename;
	std::wcin >> filename;
	std::wifstream file(L"C:/Analysis/"+filename);
	if (!(file.is_open())) {
		std::wcout << L"NO FILE!" << std::endl;
		return 0;
	}
	std::wofstream newFile(L"C:/Analysis/NOSPACE"+filename);
	std::wstring s;
	wchar_t c;
	size_t counter = 0;
	while (file>>c){
		if (c == L'.' && counter % WIDTH != NMCK_POS) {
			newFile << L",";
			continue;
		}
		newFile << c;
		if (c == L';'&&++counter%WIDTH==0) {
			newFile << L"\n";
		}
		
	}
	file.close();
	newFile.close();
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
