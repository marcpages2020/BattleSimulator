#include "Warrior.h"
#include "GameManager.h"
#include <stdlib.h>
#include <ctime>
#include <iostream>
using namespace std;

Warrior::Warrior(const char* g_name, float health, float energy, float attack, float defense, float magicalAttack, float magicalDefense, GameManager* gameManager)
	: name(g_name), _currentHealth(health), _maxHealth(health), _currentEnergy(energy), _maxEnergy(energy), _attack(attack), _defense(defense), _magicalAttack(magicalAttack), _magicalDefense(magicalDefense), _gameManager(gameManager),
	_defenseMultiplier(1.0f), _magicalDefenseMultiplier(1.0f), _currentAction(nullptr), _enemies(nullptr)
{}

Warrior::~Warrior() {}

void Warrior::SetEnemies(std::vector<Warrior>* enemies)
{
	_enemies = enemies;
}

void Warrior::ChooseAction(ActionStrategy strategy)
{
	int action;
	bool magical;
	switch (strategy)
	{
	case ActionStrategy::RANDOM:
		ChooseRandomAction();
		break;
	case ActionStrategy::ATTACK_TENDENCY:
		 action = rand() % 4;
		 magical = rand() % 2;

		//only defend 1/4 of the times
		if (action == 3)
		{
			if (magical && _currentEnergy > _magicalDefense * 0.5f) {
				_currentAction = new MagicalDefend(this);
			}
			else {
				_currentAction = new Defend(this);
			}
		}
		//then the attack probability is of 3/4
		else
		{
			if (magical && _currentEnergy > _magicalAttack * 0.5f) {
				_currentAction = new MagicalAttack(this);
			}
			else {
				_currentAction = new Attack(this);
			}
		}
		break;
	case ActionStrategy::ALWAYS_ATTACK:
		if (_attack > _magicalAttack || _currentEnergy < _magicalAttack * 0.5f)
		{
			_currentAction = new Attack(this);
		}
		else
		{
			_currentAction = new MagicalAttack(this);
		}
		break;
	default:
		break;
	}
}

void Warrior::ChooseRandomAction()
{
	ActionType actionType = (ActionType)(rand() % (_currentEnergy > 0.0f ? MAX_ACTIONS : MAGICAL_ATTACK));

	switch (actionType)
	{
	case ActionType::ATTACK:
		_currentAction = new Attack(this);
		break;
	case ActionType::DEFEND:
		_currentAction = new Defend(this);
		break;
	case ActionType::MAGICAL_ATTACK:
		_currentAction = new MagicalAttack(this);
		break;
	case ActionType::MAGICAL_DEFENSE:
		_currentAction = new MagicalDefend(this);
		break;
	default:
		break;
	}
}

Action* Warrior::ActionTypeToAction(ActionType actionType)
{
	Action* action = nullptr;

	switch (actionType)
	{
	case ActionType::ATTACK:
		action = new Attack(this);
		break;
	case ActionType::DEFEND:
		action = new Defend(this);
		break;
	case ActionType::MAGICAL_ATTACK:
		action = new MagicalAttack(this);
		break;
	case ActionType::MAGICAL_DEFENSE:
		action = new MagicalDefend(this);
		break;
	default:
		break;
	}

	return action;
}

void Warrior::HandleInput()
{
	std::cout << "What do you want to do with " << name << "?" << std::endl;
	std::cout << "[0] Attack " << " | " << "[1] Defend";

	if (_currentEnergy > 0)
		cout << " | " << "[2] Magical Attack" << " | " << "[3] Magical Defense";

	cout << std::endl;

	ActionType actionType;
	int actionInt;
	std::cin >> actionInt;
	cout << endl;
	actionType = (ActionType)(actionInt);
	_currentAction = ActionTypeToAction(actionType);

	if (actionType == ActionType::ATTACK || actionType == ActionType::MAGICAL_ATTACK)
	{
		int enemyIndex = 0;

		do
		{
			std::cout << "Who is the target of the action? " << std::endl;
			for (size_t i = 0; i < _enemies->size(); i++)
			{
				std::cout << "[" << i << "]" << " " << _enemies->at(i).name << " | ";
			}
			cout << endl;
			std::cin >> enemyIndex;

		} while (enemyIndex < 0 && enemyIndex > _enemies->size() - 1);

		_enemy = &_enemies->at(enemyIndex);
	}
}

void Warrior::ExecuteAction()
{
	_currentAction->Execute();
	delete _currentAction;
}

Warrior* Warrior::ChooseEnemy(ChooseTargetStrategy strategy)
{
	if (_enemies->size() == 0)
		return nullptr;

	if (strategy == ChooseTargetStrategy::RANDOM)
	{
		_enemy = ChooseRandomEnemy();
	}
	else if (strategy == ChooseTargetStrategy::OPTIMIZED)
	{
		_enemy = ChooseOptimizedEnemy();
	}
}

Warrior* Warrior::ChooseRandomEnemy()
{
	int enemyIndex = rand() % _enemies->size();

	return &_enemies->at(enemyIndex);
}

Warrior* Warrior::ChooseOptimizedEnemy()
{
	if (_currentAction->type == ActionType::ATTACK)
	{
		Warrior* lowestDefenseWarrior = &_enemies->at(0);
		Warrior* lowestHealthWarrior = &_enemies->at(0);

		for (size_t i = 0; i < _enemies->size(); i++)
		{
			//get the enemy with the lowest defense
			if (_enemies->at(i)._defense < lowestDefenseWarrior->_defense)
			{
				lowestDefenseWarrior = &_enemies->at(i);
			}

			//get the enemy with the lowest health
			if (_enemies->at(i)._currentHealth < lowestHealthWarrior->_currentHealth)
			{
				lowestHealthWarrior = &_enemies->at(i);
			}

			return lowestHealthWarrior;
		}
	}
	else if (_currentAction->type == ActionType::MAGICAL_ATTACK)
	{
		Warrior* lowestMagicalDefenseWarrior = &_enemies->at(0);
		Warrior* lowestHealthWarrior = &_enemies->at(0);

		for (size_t i = 0; i < _enemies->size(); i++)
		{
			//get the enemy with the lowest magical defense
			if (_enemies->at(i)._magicalDefense < lowestMagicalDefenseWarrior->_magicalDefense)
			{
				lowestMagicalDefenseWarrior = &_enemies->at(i);
			}

			//get the enemy with the lowest health
			if (_enemies->at(i)._currentHealth < lowestHealthWarrior->_currentHealth)
			{
				lowestHealthWarrior = &_enemies->at(i);
			}

			return lowestHealthWarrior;
		}
	}
}

void Warrior::TakeDamage(float damage)
{
	_currentHealth -= std::max((damage - _defense * _defenseMultiplier), 0.0f);

	if (_defenseMultiplier != 0.0f) {
		_defenseMultiplier = 0.0f;
	}

	if (_currentHealth <= 0)
		_gameManager->CheckAliveWarriors();
}

void Warrior::TakeMagicalDamage(float magicalDamage)
{
	_currentHealth -= std::max((magicalDamage - _magicalDefense * _magicalDefenseMultiplier), 0.0f);

	if (_magicalDefenseMultiplier != 0.0f) {
		_magicalDefenseMultiplier = 0.0f;
	}

	if (_currentHealth <= 0)
		_gameManager->CheckAliveWarriors();
}

void Warrior::IncreaseEnergy()
{
	_currentEnergy += _maxEnergy * 0.25;

	if (_currentEnergy > _maxEnergy)
		_currentEnergy = _maxEnergy;
}

void Warrior::IncreaseStats(float attackIncrease, float defenseIncrease, float magicalAttackIncrease, float magicalDefenseIncrease, float healthIncrease, float energyIncrease)
{
	_attack = round(_attack + (20.0f * (attackIncrease / 100.0f)));
	_defense = round(_defense + (20.0f * (defenseIncrease / 100.0f)));
	_magicalAttack = round(_magicalAttack + (20.0f * (magicalAttackIncrease / 100.0f)));
	_magicalDefense = round(_magicalDefense + (20.0f * (magicalDefenseIncrease / 100.0f)));
	_maxHealth = round(_maxHealth + (20.0f * (healthIncrease / 100.0f)));
	_maxEnergy = round(_maxEnergy + (20.0f * (energyIncrease / 100.0f)));
}

void Warrior::ShowStats()
{
	std::cout << name << " [HP]" << _currentHealth << std::endl;
}
