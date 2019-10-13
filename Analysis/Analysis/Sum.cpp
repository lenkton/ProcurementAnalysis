#include "Sum.h"
#include "Analysis.h"


int PrintSumStat(const SumStat& ss, std::wostream& os) {
	for (AtomSumDistr atom : ss) {
		os << atom.lowerBorder << L":" << atom.num << std::endl;
	}
	return 0;
}

int CreateSumStat(std::wistream& is, SumStat& ss) {
	double d;
	AtomSumDistr atom;
	while (is >> d) {
		atom.lowerBorder = d;
		atom.num = 0;
		ss.push_back(atom);
	}
	return 0;
}

int FillSumStat(SumStat& ss, const Table& t, const size_t& n) {
	vector <unsigned int> v;
	//size_t
	for (FileStats fs : t) {
		//TODO
		for (size_t i = 0; i < ss.size(); i++)
			if (stod(fs[n]) < ss[i].lowerBorder) {
				ss[i].num++;
				break;
			}
	}
	FillLast(ss, t);
	return 0;
}

int FillLast(SumStat& ss, const Table& t){
	size_t overall = 0;
	for (auto s : ss) {
		overall += s.num;
	}
	AtomSumDistr atom;
	atom.lowerBorder = -1;
	atom.num = t.size() - overall;
	ss.push_back(atom);
	return 0;
}