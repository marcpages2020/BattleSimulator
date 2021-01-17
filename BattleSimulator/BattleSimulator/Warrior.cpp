#include "Warrior.h"
#include "GameManager.h"
#include <stdlib.h>
#include <time.h>

Warrior::Warrior(const char* g_name, float health, float attack, float defense, float magicalAttack, float magicalDefense, GameManager* gameManager)
	: name(g_name), _health(health), _attack(attack), _defense(defense), _magicalAttack(magicalAttack), _magicalDefense(magicalDefense), _gameManager(gameManager),
	_defenseMultiplier(1.0f), _magicalDefenseMultiplier(1.0f), _currentAction(nullptr), _enemies(nullptr)
{}

Warrior::~Warrior() {}

void Warrior::SetEnemies(std::vector<Warrior>* enemies)
{
	_enemies = enemies;
}

void Warrior::ChooseAction(Strategy strategy)
{
	switch (strategy)
	{
	case Strategy::RANDOM:
		ChooseRandomAction();
		break;
	case Strategy::ALWAYS_ATTACK:
		_currentAction = new Attack(this);
		break;
	default:
		break;
	}
}

void Warrior::ChooseRandomAction()
{
	ActionType actionType =(ActionType)(rand() % MAX_ACTIONS);

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

void Warrior::ExecuteAction()
{
	_currentAction->Execute();
	delete _currentAction;
}

Warrior* Warrior::ChooseRandomEnemy()
{
	if (_enemies->size() == 0)
		return nullptr;

	int enemyIndex = rand() % _enemies->size();

	return &_enemies->at(enemyIndex);
}

void Warrior::TakeDamage(float damage)
{
	_health -= std::max((damage - _defense * _defenseMultiplier), 0.0f);

	if (_defenseMultiplier != 0.0f) {
		_defenseMultiplier = 0.0f;}

	if (_health <= 0)
		_gameManager->CheckAliveWarriors();
}

void Warrior::TakeMagicalDamage(float magicalDamage)
{
	_health -= std::max((magicalDamage - _magicalDefense * _magicalDefenseMultiplier), 0.0f);

	if (_magicalDefenseMultiplier != 0.0f) {
		_magicalDefenseMultiplier = 0.0f;}

	if (_health <= 0)
		_gameManager->CheckAliveWarriors();
}

void Warrior::IncreaseStats(float attackIncrease, float defenseIncrease, float magicalAttackIncrease, float magicalDefenseIncrease, float healthIncrease)
{
	_attack = round(_attack + (20.0f * (attackIncrease / 100.0f)));
	_defense = round(_defense + (20.0f * (defenseIncrease / 100.0f)));
	_magicalAttack = round(_magicalAttack + (20.0f * (magicalAttackIncrease / 100.0f)));
	_magicalDefense = round(_magicalDefense + (20.0f * (magicalDefenseIncrease / 100.0f)));
	_health = round(_health + (20.0f * (healthIncrease / 100.0f)));
}
