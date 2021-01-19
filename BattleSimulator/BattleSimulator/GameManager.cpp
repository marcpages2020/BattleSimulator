#include "GameManager.h"

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::CheckAliveWarriors()
{
	std::vector<Warrior> tmp_alive_heroes;
	for (size_t i = 0; i < aliveHeroes.size(); i++)
	{
		if (aliveHeroes[i].IsAlive()) {
			tmp_alive_heroes.push_back(aliveHeroes[i]);
		}

	}
	aliveHeroes = tmp_alive_heroes;

	std::vector<Warrior> tmp_alive_villains;
	for (size_t i = 0; i < aliveVillains.size(); i++)
	{
		if (aliveVillains[i].IsAlive()) {
			tmp_alive_villains.push_back(aliveVillains[i]);
		}
	}
	aliveVillains = tmp_alive_villains;
}

int GameManager::CheckWinner()
{
	CheckAliveWarriors();

	//Heroes win
	if (aliveVillains.size() == 0)
		return 0;

	//Villains win
	if (aliveHeroes.size() == 0)
		return 1;

	//No winner
	return -1;
}

void GameManager::SetSimulation()
{
	aliveHeroes = _heroes;
	aliveVillains = _villains;

	for (size_t i = 0; i < aliveVillains.size(); i++)
	{
		aliveHeroes[i].SetEnemies(&aliveVillains);
	}

	for (size_t i = 0; i < aliveVillains.size(); i++)
	{
		aliveVillains[i].SetEnemies(&aliveHeroes);
	}
}
