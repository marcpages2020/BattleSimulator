#ifndef _WARRIOR_H_
#define _WARRIOR_H_
#include "Action.h"
#include <vector>

class GameManager;

enum ActionStrategy
{
	RANDOM,
	ALWAYS_ATTACK,
};

enum class ChooseTargetStrategy
{
	RANDOM,
	OPTIMIZED
};

class Warrior {
public:
	Warrior(const char* name, float health, float energy, float attack, float defense, float magicalAttack, float magicalDefense, GameManager* gameManager);
	~Warrior();

	void SetEnemies(std::vector<Warrior>* enemies);

	void ChooseAction(ActionStrategy strategy);
	void ChooseRandomAction();
	Action* ActionTypeToAction(ActionType actionType);
	void HandleInput();
	void ExecuteAction();

	Warrior* ChooseEnemy(ChooseTargetStrategy strategy);

	void TakeDamage(float damage);
	void TakeMagicalDamage(float magicalDamage);

	inline bool IsAlive() { return _health > 0; }
	void IncreaseStats(float attackIncrease, float defenseIncrease, float magicalAttackIncrease, float magicalDefenseIncrease, float healthIncrease, float energyIncrease);
	void ShowStats();

private:
	Warrior* ChooseRandomEnemy();
	Warrior* ChooseOptimizedEnemy();

public:
	const char* name;

	float _health;
	float _energy;
	float _attack;
	float _defense;
	float _defenseMultiplier;
	float _magicalAttack;
	float _magicalDefense;
	float _magicalDefenseMultiplier;

	Warrior* _enemy;

private:
	std::vector<Warrior>* _enemies;
	Action* _currentAction;
	GameManager* _gameManager;
};

#endif // !_WARRIOR_H_

