#include "DynamicMap.h"
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include<iostream>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <set>
#include <map>
#include <queue>
/*
    Map creation based on Voronoi diagram
*/

// Computes the distance between two territories
double distance(const Territory& t1, const Territory& t2) {
    int dx = t1.x - t2.x;
    int dy = t1.y - t2.y;
    return std::sqrt(dx * dx + dy * dy);
}
// Tests whether two territories are neighbors in a 6-neighbor grid with the specified neighborhood pattern
bool areNeighbors(const std::pair<unsigned int, unsigned int>& t1, const std::pair<unsigned int, unsigned int>& t2) {
    int dx = t1.first - t2.first;
    int dy = t1.second - t2.second;
    //if (dx == 0 && dy == 0) return false;  // territories must be distinct
    if (dx == 0 && std::abs(dy) == 1) return true;  // territories must be directly above or below each other
    if (dy == 0 && std::abs(dx) == 1) return true;  // territories must be directly left or right of each other
    if (t1.first % 2 == 0) {  // even row
        if (std::abs(dx) == 1 && std::abs(dy) == 1 && dy == 1) return true;  // territories must be diagonally adjacent and separated by an empty cell
        if (std::abs(dx) == 1 && std::abs(dy) == 1 && dy == -1) return true;  // territories must be diagonally adjacent and separated by an empty cell
    }
    else {  // odd row
        if (std::abs(dx) == 1 && std::abs(dy) == 1 && dy == 1) return true;  // territories must be diagonally adjacent and separated by an empty cell
        if (std::abs(dx) == 1 && std::abs(dy) == 1 && dy == -1) return true;  // territories must be diagonally adjacent and separated by an empty cell
    }
    return false;
}
bool notIsolated(const std::pair<unsigned int, unsigned int>& t, const std::vector<std::pair<unsigned int, unsigned int>>& region) {

    for (auto pair = region.begin(); pair != region.end(); pair++) {
        if (areNeighbors(t, *pair) == true) {
            return true;
        }
    }
    return false;
}

std::vector<Territory> findBorders(const std::vector<std::vector<Territory>>& map) {
    std::vector<Territory> borders;
    for (int x = 0; x < map.size(); x++) {
        for (int y = 0; y < map[0].size(); y++) {
            if (x > 0 && map[x][y].region != map[x - 1][y].region) {
                borders.push_back(map[x][y]);
            }
            if (y > 0 && map[x][y].region != map[x][y - 1].region) {
                borders.push_back(map[x][y]);
            }
        }
    }
    return borders;
}
// Removes the given region from the map
void removeRegion(std::vector<std::vector<Territory>>& map, int region) {
    // Set all territories in the region to be empty
    for (int x = 0; x < map.size(); x++) {
        for (int y = 0; y < map[0].size(); y++) {
            if (map[x][y].region == region) {
                map[x][y].region = -1;
            }
        }
    }

    // Find the borders between the regions on the map
    std::vector<Territory> borders = findBorders(map);

    // Connect the regions by merging the borders
    while (!borders.empty()) {
        Territory border = borders.back();
        borders.pop_back();
        std::queue<Territory> queue;
        queue.push(border);
        while (!queue.empty()) {
            Territory current = queue.front();
            queue.pop();
            if (current.region == border.region) continue;
            current.region = border.region;
            map[current.x][current.y] = current;
            if (current.x > 0 && map[current.x - 1][current.y].region != border.region) {
                queue.push(map[current.x - 1][current.y]);
            }
            if (current.x < map.size() - 1 && map[current.x + 1][current.y].region != border.region) {
                queue.push(map[current.x + 1][current.y]);
            }
            if (current.y > 0 && map[current.x][current.y - 1].region != border.region) {
                queue.push(map[current.x][current.y - 1]);
            }
            if (current.y < map[0].size() - 1 && map[current.x][current.y + 1].region != border.region) {
                queue.push(map[current.x][current.y + 1]);
            }
        }
    }
}


void loadMap(Regions& regions, int r, int c)
{
    // Choose the size of the map
    const int mapWidth = r;//C
    const int mapHeight = c;//R

    // Create the map as a 2D array of territories
    std::vector<std::vector<Territory>> map(mapWidth, std::vector<Territory>(mapHeight));

    // Initialize the map by assigning coordinates to each territory
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            map[x][y].x = x;
            map[x][y].y = y;
            map[x][y].region = -1;  // Unassigned
        }
    }

    // Choose a random set of seeds for the Voronoi diagram
    //by choosing always random seed we garuntee that its always a random map
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> distX(0, mapWidth - 1);
    std::uniform_int_distribution<std::mt19937::result_type> distY(0, mapHeight - 1);
    const int numSeeds = c; // number of seed also reflects number of regions
    std::vector<Territory> seeds;
    for (int i = 0; i < numSeeds; i++) {
        seeds.push_back(Territory(distX(rng), distY(rng), i));
    }

    // Assign each territory to the region with the closest seed
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            double minDistance = std::numeric_limits<double>::max();
            int closestSeed = -1;
            for (int i = 0; i < numSeeds; i++) {
                double d = distance(map[x][y], seeds[i]);
                if (d < minDistance) {
                    minDistance = d;
                    closestSeed = seeds[i].region;
                }
            }
            map[x][y].region = closestSeed;
        }
    }
    //we remove some random regions
    srand(time(0));
    for (int i = 0; i < map.size() / 5; i++) {
        std::cout << i;
        removeRegion(map, (rand() % map.size()));
    }

    regions.clear();
    //Adapt the territories map to regions map
    for (int i = 0; i < numSeeds; i++) {
        std::vector<std::pair<unsigned int, unsigned int>> region;
        for (unsigned int y = 0; y < mapHeight; y++) {
            for (unsigned int x = 0; x < mapWidth; x++) {
                if (map[x][y].region == i) {
                    region.push_back({ x,y });
                }
            }

        }
        regions.push_back(region);
    }
    regions.erase(std::remove_if(regions.begin(), regions.end(),
        [&](std::vector<std::pair<unsigned int, unsigned int>>& const t) {return t.size() < 2;  }), regions.end());

    //make sure there is no isolated cells 
    for (auto region = regions.begin(); region != regions.end(); region++) {
        int position = 0; 
        region->erase(std::remove_if(region->begin(), region->end(),
            [&](std::pair<unsigned int, unsigned int>& const t) { return !notIsolated(t, *region); }), region->end());
    }
}

