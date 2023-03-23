#pragma once
#include <queue>

#include "../../Commun/IStrategyLib.h"
#include <map>
#include <vector>

struct Attack {
	int from;
	int to;
	int dicesDiff;

	Attack(int from, int to, int dicesDiff) :from(from), to(to), dicesDiff(dicesDiff) {};

};
struct LessThanByDices
{
	bool operator()(const Attack& a, const Attack& b) const
	{
		return a.dicesDiff < b.dicesDiff;
	}
};
class AggressiveAttackStrategy
{
public:
	AggressiveAttackStrategy(unsigned int id, unsigned int nbPlayer, const SMap* map);
	AggressiveAttackStrategy(const AggressiveAttackStrategy &obj) =delete;
	AggressiveAttackStrategy(AggressiveAttackStrategy &&obj) =delete;
	~AggressiveAttackStrategy();
	AggressiveAttackStrategy& operator=(const AggressiveAttackStrategy &obj) =delete;
	AggressiveAttackStrategy& operator=(AggressiveAttackStrategy &&obj) =delete;

public:
	bool PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn);

protected:
	int* regions;
	int find_smallest_path(int land, int region, int capacity, int nbr_steps, int discovered, int);
	void Divide_into_regions();
	void explore_each_region_alone(int land, int* explored, int owner, int region_number);
	int get_largest_region();
	
	const unsigned int Id;
	const unsigned int NbPlayer;
	SMap Map;
	std::priority_queue<Attack,std::vector<Attack>,LessThanByDices> targets;
};
