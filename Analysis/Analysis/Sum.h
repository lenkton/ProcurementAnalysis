#pragma once
#include <vector>
#include <string>
#include "Analysis.h"
using std::wstring;
using std::vector;

//defines
#define SUM 4

/*struct AtomSumStat {
	double lowerBorder;
	unsigned int num = 0;
};*/
class AtomSumDistr :public DistribAtom {
public : double lowerBorder;
};
typedef vector<AtomSumDistr> SumStat;


int PrintSumStat(const SumStat& ss, std::wostream& os);

int CreateSumStat(std::wistream& is, SumStat& ss);

int FillSumStat(SumStat& ss, const Table& t, const size_t& n);

int FillLast(SumStat& ss, const Table& t);
