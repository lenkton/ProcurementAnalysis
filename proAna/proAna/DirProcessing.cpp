#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include "Windows.h"
#include "proAna.h"

using std::filesystem::directory_iterator;

//HEADER???
int LineProcessingNew(std::string&,bool&,bool&);




bool Contains(std::vector<std::string>& v, const std::string& key){
	for (auto s : v) {
		if (s.find(key) != std::string::npos)
			return true;
	}
	return false;
}


int LineProcessing(std::string& line, strucedData& sd) {

	bool contains = false;
	bool isMinus = false;
	int modCount;


	for (strucedData::iterator iter = sd.begin(); iter != sd.end(); iter++) {

		modCount = 0;
		switch ((*iter)[0])
		{
		case '-':
			isMinus = true;
			modCount++;
			//(*iter).erase(0, 1);
			break;
		default:
			isMinus = false;
			break;
		}
		std::string key = (*iter).substr(modCount, (*iter).length() - modCount);
		size_t num = line.find(key);
		if (!contains && (num != std::string::npos)) {
			//line.append(*iter);
			contains = true;
		}
		if (isMinus && num != std::string::npos) {
			line = "";
			return 0;
		}
	}

	if (!contains)
		line = "";
	else
		line.append("\n");
	return 0;
}

std::vector<FileData> DirProcessing(std::ifstream& file, strucedData& sd,
	const std::filesystem::path& MainPath) {
	//boooools
	//bool hasAdv = false;		
	//bool contains;	
	bool toDelete,toDeleteTest = false;
	
	//int advanced = 0;	
	int counter = 0;

	//txt output
	//std::ofstream log;	log.open("C:/contracts/log.txt");
	
	//Attachment att = {};
	//FileData fd;		
	std::vector<FileData> fdv;	
		
	//std::vector<std::string> v;
		
	std::filesystem::path pat;// MainPath;
	std::string line;




#pragma omp parallel for
	for(auto& p : std::filesystem::recursive_directory_iterator(MainPath))
		if((p.path().has_filename()))
		{				
			if (p.is_directory())
				continue;
			pat = p.path();
			if (pat.extension().string()._Equal(".sig")) {
				std::filesystem::remove(pat);
				continue;
			}
			if (pat.extension().string()._Equal(".zip")) {
				std::filesystem::remove(pat);
				continue;
			}
			//contractCancel
			if (pat.filename().string().find("contractCancel") == 0) {
				std::filesystem::remove(pat);
				continue;
			}
			if (pat.filename().string().find("contractProcedure") ==0) {
				std::filesystem::remove(pat);
				continue;
			}
			//if (counter > 5342) 
			{
				//hasAdv = false;
				//contains = false;



				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


				toDelete = false;
				toDeleteTest = true;
				//fd = {};
				
				//v.clear();

				//parcing
				file.open(pat.u8string(), std::ios::binary);
				/*
				while (file.get(c)
				*/
				//обработка линий
				//std::cout << pat.string() << '\n';
				while (std::getline(file, line)) {
					LineProcessingNew(line,toDelete,toDeleteTest);
					//for (auto s : v)
						//std::cout << s << "//";
					//std::cout << '\n';
				}
				file.clear();
				file.close();

				if (!toDelete && toDeleteTest)
					std::cout << pat.string() << std::endl;
				if (toDelete)
					std::filesystem::remove(pat);

				//output
				/*if (contains && hasAdv) {
					fdv.push_back(fd);
					log << fd.name << "\n";
					//for (auto s : fd.att)
						//log << s.URL << "\n";
					log << "\n" << pat.string() << '\n';
					log << fd.adv.perc << "\n" << fd.adv.val << "\n";
					log << std::string(80, '*') << std::endl;
					//return fdv;
				}*/
				/*if (hasAdv)
					advanced++;*/
			}
			//std::cout <<"\r"<< counter++;
				
		}

	//std::cout <<"Advanced/Counter: "<< advanced << "/" << counter<<"\n";
	//log.close();

	for (auto& p : std::filesystem::directory_iterator(MainPath))
		if ((p.is_directory()))
			if (std::filesystem::is_empty(p.path()))
				std::filesystem::remove(p.path());
	return fdv;
}
