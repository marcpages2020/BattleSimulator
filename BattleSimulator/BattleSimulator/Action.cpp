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
		std::cout << _executor->name << "(HP " << _executor->_health << ") " << " attacked " << enemy->name << " and missed" <<
			" || " << enemy->name << "'s health: " << enemy->_health << std::endl;
		return;
	}

	_executor->_enemy->TakeDamage(_executor->_attack);

	std::cout << _executor->name << "(HP " << _executor->_health << ") " << " attacked " << enemy->name <<
		" || " << enemy->name << "'s health: " << enemy->_health << std::endl;
}

Defend::Defend(Warrior* executor) : Action(executor)
{
	type = ActionType::DEFEND;
}

Defend::~Defend() {}

void Defend::Execute()
{
	_executor->_defenseMultiplier = 1.0f;
	std::cout << _executor->name << "(HP " << _executor->_health << ") " << " defended" << std::endl;
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
		std::cout << _executor->name << "(HP " << _executor->_health << ") " << " magically attacked " << enemy->name << " and missed" << " || " << enemy->name << "'s health: " << enemy->_health << std::endl;
		return;
	}

	enemy->TakeMagicalDamage(_executor->_magicalAttack);
	std::cout << _executor->name << "(HP " << _executor->_health << ") " << " magically attacked " << enemy->name << " || " << enemy->name << "'s health: " << enemy->_health; 
	
	_executor->_energy -= _executor->_magicalAttack * 0.5f;
	if (_executor->_energy <= 0)
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

	std::cout << _executor->name << "(HP " << _executor->_health << ") " << " magically defended ";
	_executor->_energy -= _executor->_magicalDefense * 0.5f;

	if (_executor->_energy <= 0)
		std::cout << " | " << _executor->name << " run out of energy ";

	std::cout << std::endl;
}
