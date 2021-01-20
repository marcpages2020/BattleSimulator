#include "Action.h"
#include "Warrior.h"
#include <iostream>

Action::Action(Warrior* executor) : _executor(executor), miss_chance(20) {}

Action::~Action() { _executor = nullptr; }

Attack::Attack(Warrior* executor) : Action(executor)
{
	type = ActionType::ATTACK;
}

Attack::~Attack() {}

void Attack::Execute()
{
	Warrior* enemy = _executor->_enemy;

	if (enemy == nullptr)
		return;

	int probability = rand() % 100;

	if (probability < miss_chance)
	{
		std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] " << " attacked " << enemy->name << " and missed" <<
			" || " << enemy->name << "'s health: " << enemy->_currentHealth << std::endl;
		return;
	}

	_executor->_enemy->TakeDamage(_executor->_attack);

	std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] " << " attacked " << enemy->name <<
		" || " << enemy->name << "'s health: " << enemy->_currentHealth << std::endl;
}

Defend::Defend(Warrior* executor) : Action(executor)
{
	type = ActionType::DEFEND;
}

Defend::~Defend() {}

void Defend::Execute()
{
	_executor->_defenseMultiplier = 1.0f;
	float energy = _executor->_currentEnergy;
	_executor->IncreaseEnergy();
	energy = _executor->_currentEnergy - energy;

	std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] " << " defended and recuperated " << energy
		<< " energy" << std::endl;
}

MagicalAttack::MagicalAttack(Warrior* executor) : Action(executor)
{
	type = ActionType::MAGICAL_ATTACK;
}

MagicalAttack::~MagicalAttack() {}

void MagicalAttack::Execute()
{
	Warrior* enemy = _executor->_enemy;

	if (enemy == nullptr)
		return;

	int probability = rand() % 100;

	if (probability < miss_chance)
	{
		std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] "
			<< " magically attacked " << enemy->name << " and missed" << " || " << enemy->name << "'s health: " << enemy->_currentHealth << std::endl;
		return;
	}

	enemy->TakeMagicalDamage(_executor->_magicalAttack);

	float energyExpense = _executor->_magicalAttack * 0.5f;
	_executor->_currentEnergy -= energyExpense;

	std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] "
		<< " magically attacked " << enemy->name << " and spent " << energyExpense << " energy || " << enemy->name << "'s health: " << enemy->_currentHealth; 
	
	if (_executor->_currentEnergy <= 0)
		std::cout << " | " << _executor->name << " run out of energy ";
	
	std::cout << std::endl;
}

MagicalDefend::MagicalDefend(Warrior* executor) : Action(executor)
{
	type = ActionType::MAGICAL_DEFENSE;
}

MagicalDefend::~MagicalDefend() {}

void MagicalDefend::Execute()
{
	_executor->_magicalDefenseMultiplier = 1.0f;

	float energyExpense = _executor->_magicalDefense * 0.5f;
	_executor->_currentEnergy -= energyExpense;

	std::cout << _executor->name << "[HP " << _executor->_currentHealth << "][EN " << _executor->_currentEnergy << "] " << " magically defended and spent " << energyExpense << " energy";

	if (_executor->_currentEnergy <= 0)
		std::cout << " | " << _executor->name << " run out of energy ";

	std::cout << std::endl;
}
