#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <string>
#include "character.h"
#include "item.h"
#include "affect.h"
//#include "decorator.h" //creatly

/* 
 * By default, the player character is a human (who starts with 140 HP, 20 Atk, 20 Def). However, the player
 * has the option of changing their race to something more fantastical (e.g. offer different attributes). The
 * options include dwarf (100 HP, 20 Atk, 30 Def, gold is doubled in value), elves (140 HP, 30 Atk, 10 Def,
 * negative potions have positive effect), and orc (180 HP, 30 Atk, 25 Def, gold is worth half value).
 * In our game board, the player character is always denoted by the ’@’ symbol.
 */

class Player : public Character {
	static Player *pc; //singleton
	static void cleanup(); //singleton
	protected:
	std::string race; //race of the player
	Item *potions[3]; //save the potion list***
	const int oriHp; //save the original Hp**
	int maxAcc; //used to determine when to recover Hp**
	int acc; //used to determine when to recover Hp**
	int exAtk; //actualAtk = exAtk+atk
	int exDef; //actualDef = exDef+def
	Affect *af;
	Player(int hp, int atk, int def, std::string race);
	
	public:
	static Player *getPlayer(); //singleton
	static void setPlayer(Player *p); //singleton
	
	int storePotion(Item *i); //keep a new potion in pc's bag (0 means the bag is full, 1 otherwise)***
	Item *usePotion(int i); //use the potion with index i***
	std::string printPotion(); //print what PC has in bag***

	virtual void addGold(int i); //half for orc;;double for dwarf
	virtual void addAtk(int i); //add to exAtk;; ABS for elf
	virtual void addDef(int i); //add to exDef;; ABS for elf
	virtual void addHp(int i); //add to Hp;; ABS for elf
	void returnGold(int i); //used to gain the returned gold from goblin*
	void buyAtk(int i); //used to gain permanent atk (by buying weapon from Merchants)****
	void buyDef(int i); //used to gain permanent def (by buying armor from Merchants)****
	void addAcc(); //used to determine when to recover Hp**
	int getOriHp() const; //used to determine when to recover Hp**
	int getAcc() const; //used to determine when to recover Hp**
	void clearAcc(); //used to determine when to recover Hp**
	void addMaxAcc(); //used to determine when to recover Hp**
	int getMaxAcc() const; //used to determine when to recover Hp**
	std::string getRace() const; //get the race
	void lossHp(int i); //used when be attacked
	void lossGold(int i); //used when gold is stealen by goblin*
	void clear(); //used when player go to the next floor
	int getAtk() const; //atk+exAtk
	int getDef() const; //def+exDef
	virtual ~Player()=0;
};

class Human : public Player {
	public:
	Human(); //140HP, 20Atk, 20Def
	~Human();
};

class Dwarf : public Player {
	public:
	Dwarf(); //100HP, 20Atk, 30Def
	~Dwarf();

	void addGold(int i);
};

class Orc : public Player {
	public:
	Orc();
	~Orc();

	void addGold(int i);
};

class Elves : public Player {
	public:
	Elves(); //140HP, 30Atk, 10Def
	~Elves();

	void addAtk(int i); // ABS
	void addDef(int i); // ABSi
	void addHp(int i);//ABS
};

class Prof : public Player {
	public:
	Prof(); //god
	~Prof();
};

class Ta : public Player {
	public:
	Ta(); //god
	~Ta();
};

#endif
