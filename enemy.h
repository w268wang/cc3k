#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <string>
#include "character.h"
#include "item.h"

/* 
 * Enemies can be one of vampire(50 HP, 25 Atk, 25 Def), werewolf(120 HP, 30 Atk, 5 Def), troll(120 HP,
 * 25 Atk, 15 Def), goblin (70 HP, 5 Atk, 10 Def), merchant (30 HP, 70 Atk, 5 Def), dragon (150 HP, 20 Atk,
 * 20 Def, always guards a treasure horde), phoenix (50 HP, 35 Atk, 20 Def).
 * 
 * Enemies are denoted on the map as follows: V(ampire), W(erewolf), N(goblin), M(erchant), D(ragon),
 * X(Phoenix), T(roll), M(erchant).
 */


class Enemy : public Character {
	protected:
	std::string type;
	Enemy(int hp, int atk, int def, std::string type);
	public:
	std::string getType() const;
	void addHp(int i);
	int getAtk() const;
	int getDef() const;
	
	virtual int returnGold() const;
	virtual void notify();
	virtual void angry();
	virtual bool getState() const;
	virtual void addGold(int i);
	virtual void reborn();
	virtual ~Enemy()=0;
};

class Vampire : public Enemy {
	public:
	Vampire(); //50 HP, 25 Atk, 25 Def
	~Vampire();

	void addHp(int i);
};

class Werewolf : public Enemy {
	bool wolf;
	public:
	Werewolf(); //120 HP, 30 Atk, 5 Def
	~Werewolf();

	bool getState() const;
	void angry();
};

class Troll : public Enemy {
	public:
	Troll(); //120 HP, 25 Atk, 15 Def
	~Troll();
};

class Goblin : public Enemy {
	int pgold;
	public:
	Goblin(); //70 HP, 5 Atk, 10 Def
	~Goblin();

	void addGold(int i);
	int returnGold() const;
};

class Merchant : public Enemy {
	static bool state;
	public:
	Merchant(); //30 HP, 70 Atk, 5 Def // state = false
	~Merchant();
	
	void angry();
	bool getState() const; //determine whether aggressive
};

class Item;

class Dragon : public Enemy {
	bool state;
	Item *dh;

	public:
	Dragon(Item *padh); //150 HP, 20 Atk, 20 Def // state = false
	~Dragon();
	
	void notify(); //tell the hoard that this dragon is slain
	void angry(); //set angry with true
	bool getState() const; //determine whether this dragon is angry
};
class Phoenix : public Enemy {
	public:
	Phoenix(); //50 HP, 35 Atk, 20 Def
	~Phoenix();

	void reborn();
};

#endif
