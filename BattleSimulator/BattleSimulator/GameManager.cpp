#include "GameManager.h"

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::CheckAliveWarriors()
{
	std::vector<Warrior> alive_heroes;
	for (size_t i = 0; i < _heroes.size(); i++)
	{
		if (_heroes[i].IsAlive()) {
			alive_heroes.push_back(_heroes[i]);
		}

	}
	_heroes = alive_heroes;

	std::vector<Warrior> alive_villains;
	for (size_t i = 0; i < _villains.size(); i++)
	{
		if (_villains[i].IsAlive()) {
			alive_villains.push_back(_villains[i]);
		}
	}
	_villains = alive_villains;
}

int GameManager::CheckWinner()
{
	CheckAliveWarriors();

	//Heroes win
	if (_villains.size() == 0)
		return 0;

	//Villains win
	if (_heroes.size() == 0)
		return 1;

	//No winner
	return -1;
}

void GameManager::SetSimulation()
{
	for (size_t i = 0; i < _heroes.size(); i++)
	{
		_heroes[i].SetEnemies(&_villains);
	}

	for (size_t i = 0; i < _villains.size(); i++)
	{
		_villains[i].SetEnemies(&_heroes);
	}
}
