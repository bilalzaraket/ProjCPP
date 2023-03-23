#include "MapLoader.h"
#include "DefaultMap.h"
#include "../DynamicMap.h"
SRegions* MapLoader::GenerateMap(unsigned int& r, unsigned int& c)
{
	Regions regions;
	//LoadDefaultMap(regions);
	r = 30;
	c = 20;
	loadMap(regions, r, c);//r,c
	unsigned int nbR, nbC;
	SRegions* sregions = ConvertMap(regions, nbR, nbC);//nbr,nbc
	r = nbR;
	c = nbC;

	Maps.insert(sregions);

	return(sregions);
}

void MapLoader::DeleteMap(SRegions* regions)
{
	for (unsigned int i = 0; i < regions->nbRegions; ++i)
	{
		delete[](regions->region[i].cells);
	}
	delete[] regions->region;
	delete regions;
}

SRegions* MapLoader::ConvertMap(Regions& regions, unsigned int& nbR, unsigned int& nbC)
{
	SRegions* reg = new SRegions;

	nbR = 0;
	nbC = 0;
	reg->nbRegions = regions.size();
	reg->region = new SRegion[reg->nbRegions];
	for (unsigned int i = 0; i < reg->nbRegions; ++i)
	{
		reg->region[i].nbCells = regions[i].size();
		reg->region[i].cells = new SRegionCell[reg->region[i].nbCells];
		for (unsigned int j = 0; j < reg->region[i].nbCells; ++j)
		{
			if (nbR < regions[i][j].first) nbR = regions[i][j].first;
			if (nbC < regions[i][j].second) nbC = regions[i][j].second;
			reg->region[i].cells[j].y = regions[i][j].first;
			reg->region[i].cells[j].x = regions[i][j].second;
		}
	}

	return(reg);
}
