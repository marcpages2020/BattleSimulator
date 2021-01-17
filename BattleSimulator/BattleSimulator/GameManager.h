#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <vector>
#include "Warrior.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void CheckAliveWarriors();
	int CheckWinner();
	void SetSimulation();

public:
	std::vector<Warrior> _heroes;
	std::vector<Warrior> _villains;
};
#endif // !_GAME_MANAGER_H_