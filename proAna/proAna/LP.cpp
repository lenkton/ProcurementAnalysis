#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include "Windows.h"
#include "proAna.h"


//HEADER???
bool Contains(std::vector<std::string> &v,const std::string &key);

using std::string;

string getField(const string& line) {
	size_t left, right = 0;
	if (((left = line.find("<")) != std::string::npos)
		&& ((right = line.find(">")) != std::string::npos)
		)
		return line.substr(left + 1, right - left - 1);
	return "";
}

string getContent(
	const string&				line, 
	std::vector<std::string>&	v,
	const std::string&			field
) 
{
	size_t  right, tmp = 0;
	
	if ((tmp = line.find_last_of("<")) != (line.find("<"))){
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


int LineProcessingNew(
	std::string					&line,
	//FileData					&fd,
	bool						&toDelete,
	bool&						toDeleteTest
)
{
	/*if (line.find("<")==std::string::npos)
		return 0;
	
	size_t tmp;
	if (line[tmp = line.find("<") + 1] == '/') {
		v.pop_back();
		return 0;
	}
	if (line[tmp] == '?') {
		return 0;
	}
	
	

	std::string field=getField(line);
	
	std::string content=getContent(line,v,field);
	if (content.empty()) {
		v.push_back(field);
		return 0;
	}
	*/
	
	//Product
	
	/*if (field._Equal("name") && Contains(v,"product")) 
		fd.name = content;*/


	/*if (!hasAdv && field._Equal("sumInPercents")&& (Contains(v,"dvance"))) {
		fd.adv.perc = content;
		hasAdv = true;
		std::cout << "hasAdv"<<"\n";
	}
	if (hasAdv && field._Equal("priceValue")&&Contains(v,"dvance"))
		fd.adv.val = content;
*/
	//attachments
	/*
	//Attachments
	//name
	//POSSIBLE FAIL: 
	//заточено под структуру, где имя файла ВСЕГДА ПЕРЕД адресом
	if (field._Equal("fileName")) 
		att.name = content;
	
	//URL
	if ( field._Equal("url")&& Contains(v, "attachment")) {
		att.URL = content;
		fd.att.push_back(att);
	}

	

	//End of Att
	*/
	/*if (field._Equal("code") &&  Contains(v, "OKPD"))
	{
		//std::cout << content << "\n";
		//if (((tmp = content.find("41.20.40")) != std::string::npos) && (tmp == 0)) {
		if (content[0] == '4' &&
			(content[1]=='1'||content[1]=='2'||content[1]=='3')
			) 
		{
			//contains = true;
			//std::cout << "contains" << "\n"<<content<<"\n";
		}
		else
		{
			toDelete = true;
		}*/

	static bool containsOKPD = false;
	size_t tmp;
	if (line.find("<OKPD>") != std::string::npos) {
		toDelete = true;
	}
	if (line.find("<OKPD2>") != std::string::npos|| line.find("<KTRU>") != std::string::npos)
	{
		containsOKPD = true;
		return 0;
	}
	if (containsOKPD && (line.find("</OKPD2>") != std::string::npos || line.find("</KTRU>") != std::string::npos))
	{
		containsOKPD = false;
		return 0;
	}
	if (containsOKPD && (line.find("code") != std::string::npos))
	{
		tmp = line.find_first_of(">") + 1;

		if (line[tmp] == '4' &&
			(line[tmp+1] == '1' || line[tmp + 1] == '2' || line[tmp + 1] == '3')
			)
		{
			//contains = true;
			//std::cout << "contains" << "\n"<<content<<"\n";
			toDeleteTest = false;
		}
		else
		{
			toDelete = true;
		}
	}


	

	

	return 0;
}