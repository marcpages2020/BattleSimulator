#ifndef _WARRIOR_H_
#define _WARRIOR_H_
#include "Action.h"
#include <vector>

class GameManager;

enum Strategy
{
	RANDOM,
	ALWAYS_ATTACK,
};

class Warrior {
public:
	Warrior(const char* _name, float _health, float _attack, float _defense, float _magicalAttack, float _magicalDefense, GameManager* gameManager);
	~Warrior();

	void SetEnemies(std::vector<Warrior>* enemies);
	
	void ChooseAction(Strategy strategy);
	void ChooseRandomAction();
	void ExecuteAction();
	Warrior* ChooseRandomEnemy();

	void TakeDamage(float damage);
	void TakeMagicalDamage(float magicalDamage);

	inline bool IsAlive() { return _health > 0; }
	void IncreaseStats(float attackIncrease, float defenseIncrease, float magicalAttackIncrease, float magicalDefenseIncrease, float healthIncrease);

public:
	const char* name;

	float _health;
	float _attack;
	float _defense;
	float _defenseMultiplier;
	float _magicalAttack;
	float _magicalDefense;
	float _magicalDefenseMultiplier;

private:
	std::vector<Warrior>* _enemies;
	Action* _currentAction;
	GameManager* _gameManager;
};

#endif // !_WARRIOR_H_

