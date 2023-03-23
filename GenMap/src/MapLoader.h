#pragma once

#include <vector>
#include <set>
#include <utility>
#include "../../Commun/IMapLib.h"

using Regions = std::vector< std::vector<std::pair<unsigned int, unsigned int>> >;

class MapLoader : public IMap
{
public:
	MapLoader() {}
	virtual ~MapLoader() { for (auto pMap : Maps) DeleteMap(pMap); }
	SRegions* GenerateMap(unsigned int& r, unsigned int& c);
	void DeleteMap(SRegions* regions);

private:
	SRegions* ConvertMap(Regions& regions, unsigned int& nbR, unsigned int& nbC);

private:
	std::set<SRegions*> Maps;
};
