#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "src/MapLoader.h"

struct Territory {
  int x;       // X coordinate
  int y;       // Y coordinate
  int region;  // Region that the territory belongs to (-1 if unassigned)
  Territory(){};
  Territory(int x, int y, int region) : x(x), y(y), region(region){};
};

double distance(const Territory& t1, const Territory& t2);

void loadMap(Regions& regions, int r, int c);

std::vector<Territory> findBorders(
    const std::vector<std::vector<Territory>>& map);

void removeRegion(std::vector<std::vector<Territory>>& map, int region);