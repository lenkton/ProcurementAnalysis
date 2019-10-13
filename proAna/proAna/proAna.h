#include <vector>
#include <string>

struct Attachment {
	std::string name;
	std::string URL;
};

struct Param {
	std::string Name;
	std::string content;
};


struct FileData {
	//std::vector<Attachment> att;
	std::string name;
	bool hasAdvance = false;
	struct advTag {
		std::string perc;
		std::string val;
	} adv;
};

typedef  std::vector<std::string> strucedData;

int Unzip();
int LineProcessing(std::string&, strucedData&);
std::vector<FileData> DirProcessing(std::ifstream&, strucedData&,const std::filesystem::path&);
int CheckForAdv(std::vector<FileData>);
