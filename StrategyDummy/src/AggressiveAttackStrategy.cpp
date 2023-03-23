#include "AggressiveAttackStrategy.h"
#include "interface_lib.h"
#include <iostream>
#include <set>

double const WEAKNESS_FACTOR = 0.5;
AggressiveAttackStrategy::AggressiveAttackStrategy(unsigned int id, unsigned int nbPlayer, const SMap* map) :
	Id(id),
	NbPlayer(nbPlayer)
{
	Map.cells = map->cells;
	Map.nbCells = map->nbCells;
	Map.cells = new SCell[map->nbCells];
	Map.nbCells = map->nbCells;
	for (unsigned int i = 0; i < map->nbCells; i++) {
		Map.cells[i] = map->cells[i];
		Map.cells[i].nbNeighbors = map->cells[i].nbNeighbors;
		Map.cells[i].infos = map->cells[i].infos;

		Map.cells[i].neighbors = new pSCell[map->cells[i].nbNeighbors];
	}
	for (unsigned int i = 0; i < map->nbCells; i++) {
		for (unsigned int j = 0; j < map->cells[i].nbNeighbors; j++) {
			for (unsigned int k = 0; k < Map.nbCells; k++) {
				if (map->cells[i].neighbors[j]->infos.id == Map.cells[k].infos.id) {
					Map.cells[i].neighbors[j] = &Map.cells[k];
					break;
				}
			}
		}
	}
	regions = new int[Map.nbCells];
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		regions[i] = 0;
	}

}

AggressiveAttackStrategy::~AggressiveAttackStrategy()
{
	for (unsigned int i = 0; i < Map.nbCells; i++) {
		delete[](Map.cells + i)->neighbors;
	}
	delete[] Map.cells;
	delete[] regions;

}
void AggressiveAttackStrategy::explore_each_region_alone(int land, int* explored, int owner, int region_number) {
	for (int j = 0; j < Map.cells[land].nbNeighbors; j++) {
		if ((explored[Map.cells[land].neighbors[j]->infos.id] != 1) && (Map.cells[land].neighbors[j]->infos.owner == owner)) {
			explored[Map.cells[land].neighbors[j]->infos.id] = 1;
			regions[Map.cells[land].neighbors[j]->infos.id] = region_number;
			explore_each_region_alone(Map.cells[land].neighbors[j]->infos.id, explored, owner, region_number);
		}
	}

}

//Main function to discover regions:
void AggressiveAttackStrategy::Divide_into_regions() {
	//checks if the cell was already visited to prevent infinite loops and overflows
	int* explored = new int[Map.nbCells];
	//NB: the regions start from 1
	//we only calculate the regions of the player having our strategy
	//the other regions have 0 region values
	int region_number = 1;

	//first we initialize the explored array with 0
	for (int i = 0; i < Map.nbCells; i++) {
		explored[i] = 0;
	}
	//Now we find each region using a Dfs algorithm
	for (int i = 0; i < Map.nbCells; i++) {
		if (explored[i] == 0) {
			if (Map.cells[i].infos.owner == Id) {
				regions[i] = region_number;
				//mark as explored
				explored[i] = 1;
				//explore a region
				explore_each_region_alone(i, explored, Id, region_number);
				//find another region
				region_number++;
			}

		}
	}
	delete[] explored;
}

//recursive function
int AggressiveAttackStrategy::get_largest_region() {
	//exploring all the neighbors of the cell
	//call recursively to find other regions
	int* largest = new int[Map.nbCells];
	for (int i = 0; i < Map.nbCells; i++) {
		largest[i] = 0;
	}
	for (int i = 0; i < Map.nbCells; i++) {
		if (regions[i] != 0)
			largest[regions[i]]++;
	}
	int larg_rg = -3, larg_nbr = 0, reg1 = 0, reg2 = 0;
	for (int i = 1; i < Map.nbCells; i++) {
		if (largest[i] == 0) {
			break;
		}
		if (largest[i] > larg_nbr) {
			larg_nbr = largest[i];
			larg_rg = i;
		}
	}
	for (int j = 0; j < Map.nbCells; j++) {
		if (regions[j] == larg_rg)
			reg1 += Map.cells[j].infos.nbDices;
	}
	for (int i = 1; i < Map.nbCells; i++) {
		if (largest[i] == 0)
			break;
		if (largest[i] == largest[larg_rg] && i != larg_rg) {

			for (int j = 0; j < Map.nbCells; j++) {
				if (regions[j] == i)
					reg2 += Map.cells[j].infos.nbDices;
			}
			if (reg2 > reg1) {
				larg_rg = i;
				reg1 = reg2;
			}
			reg2 = 0;
		}
	}

	delete[] largest;
	return larg_rg;
}
bool isLegalMove(SMap& const map, int Id, int from, int to) {
	SCell fromCell;
	SCell toCell;
	fromCell = map.cells[0];
	toCell = map.cells[0];
	for (int i = 0; i < map.nbCells; i++) {
		if (map.cells[i].infos.id == from) {
			fromCell = map.cells[i];

		}
		else if (map.cells[i].infos.id == to) {
			toCell = map.cells[i];
		}
	}
	//first condition cell from to player Id and cell to is enemy
	if (fromCell.infos.owner != Id || toCell.infos.owner == Id) {
		return false;
	}
	//2nd condition : dices
	if (fromCell.infos.nbDices < 2 || ((int)fromCell.infos.nbDices - (int)toCell.infos.nbDices) < 0) {
		return false;
	}
	return true;
}

void getPossibleTargets(SMap& Map, int Id, std::set<int> biggestRegion, std::priority_queue<Attack, std::vector<Attack>, LessThanByDices>& targets) {
	for (unsigned int i = 0; i < Map.nbCells; i++) {

		//if its our cell and have more than one dice
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
			//collect the enemy neighbors to our cell and have lower dices to us
			for (int j = 0; j < (int)Map.cells[i].nbNeighbors; j++) {
				if ((Map.cells[i].neighbors[j]->infos.owner != Id && Map.cells[i].infos.nbDices > Map.cells[i].neighbors[j]->infos.nbDices)) {

					//if the attacking cell is in largest region we increment it priority
					auto pos = biggestRegion.find(Map.cells[i].infos.id);
					if (pos != biggestRegion.end()) {
						targets.push(Attack(
							Map.cells[i].infos.id,
							Map.cells[i].neighbors[j]->infos.id,
							Map.cells[i].infos.nbDices - Map.cells[i].neighbors[j]->infos.nbDices + 3)
						);
					}
					else {
						targets.push(Attack(
							Map.cells[i].infos.id,
							Map.cells[i].neighbors[j]->infos.id,
							Map.cells[i].infos.nbDices - Map.cells[i].neighbors[j]->infos.nbDices)
						);
					}


				}
			}

		}

	}
}

bool AggressiveAttackStrategy::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{

	for (unsigned int i = 0; i < Map.nbCells; i++) {
		Map.cells[i].infos.owner = state->cells[i].owner;
		Map.cells[i].infos.nbDices = state->cells[i].nbDices;
	}
	for (int i = 0; i < (int)Map.nbCells; i++) {
		regions[i] = 0;
	}
	Divide_into_regions();
	std::priority_queue<Attack, std::vector<Attack>, LessThanByDices> empty;
	std::swap(targets, empty);
	//we calculate our biggest region to provide it to the possible targets function
	int largest_region = get_largest_region();
	std::set<int> biggestRegion;
	//we create a set with all the cells composing this region
	for (int i = 0; i < (int)Map.nbCells; i++) {
		if (regions[i] == largest_region) {
			biggestRegion.insert(i);
		}
	}
	//we call the possible targets function to populate the vector targets
	getPossibleTargets(Map, Id, biggestRegion, targets);

	if (targets.empty()) {
		//if no possible attack we return false
		return false;
	}
	else {
		while (!targets.empty() && !isLegalMove(Map, Id, Attack(targets.top()).from, Attack(targets.top()).to)) {
			targets.pop();
		}
		if (!targets.empty()) {
			Attack bestMove = Attack(targets.top());
			//we take the best legalmove having the best priority
			turn->cellFrom = bestMove.from; turn->cellTo = bestMove.to;
			if (isLegalMove(Map, Id, bestMove.from, bestMove.to)) {
				return true;

			}
			return false;
		}
		return false;
	}

}
