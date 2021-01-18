#ifndef _ACTION_H_
#define _ACTION_H_

class Warrior;

enum ActionType
{
	//NONE,
	ATTACK,
	DEFEND,
	MAGICAL_ATTACK,
	MAGICAL_DEFENSE,
	MAX_ACTIONS,
};

class Action
{
public:
	Action(Warrior* warrior);
	~Action();

	virtual void Execute() = 0;

protected:
	Warrior* _executor;
	const int miss_chance;

public:
	ActionType type;
};


class Attack : public Action {
public:
	Attack(Warrior* executor);
	~Attack();

	void Execute() override;
};

class Defend : public Action {
public:
	Defend(Warrior* executor);
	~Defend();

	void Execute() override;
};

class MagicalAttack : public Action {
public:
	MagicalAttack(Warrior* executor);
	~MagicalAttack();

	void Execute() override;
};

class MagicalDefend : public Action {
public:
	MagicalDefend(Warrior* executor);
	~MagicalDefend();

	void Execute() override;
};

#endif // !_ACTION_H_

