#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include "enemy.h"

class Dragon;
class Item {
	protected:
	std::string type; //potion or Treasure
	std::string name; //spcific name
	Item(std::string type, std::string name);
	public:
	std::string getType() const;
	std::string getName() const;
	
	virtual int getHp() const;
	virtual int getAtk() const;
	virtual int getDef() const;
	virtual bool getVisible() const;
	virtual void know();

	virtual void unguard();
	virtual bool getGuard() const;
	virtual int getGold() const;
	virtual void setDragon(Dragon *a);
	virtual void findPlayer();
	virtual ~Item()=0;
};

//************************************************
//************************************************

class Potion : public Item {
	protected:	
	const int hp;
	const int exAtk;
	const int exDef;
	Potion(std::string type, std::string name, 
	int hp, int exAtk, int exDef);
	public:
	int getHp() const;
	int getAtk() const;
	int getDef() const;
	virtual ~Potion()=0;
};

class RH : public Potion {
	static bool visible;
	
	public:
	RH(); // Hp=10, exAtk=0, exDef=0
	~RH();
	
	void know();
	bool getVisible() const;
};

class BA : public Potion {
	static bool visible;
	
	public:
	BA(); // Hp=0, exAtk=5, exDef=0
	~BA();
	
	void know();
	bool getVisible() const;
};

class BD : public Potion {
	static bool visible;
	
	public:
	BD(); // Hp=0, exAtk=0, exDef=5
	~BD();
	
	void know();
	bool getVisible() const;
};

class PH : public Potion {
	static bool visible;
	
	public:
	PH(); // Hp=-10, exAtk=0, exDef=0
	~PH();
	
	void know();
	bool getVisible() const;
};

class WA : public Potion {
	static bool visible;
	
	public:
	WA(); // Hp=0, exAtk=-5, exDef=0
	~WA();
	
	void know();
	bool getVisible() const;
};

class WD : public Potion {
	static bool visible;
	
	public:
	WD(); // Hp=0, exAtk=0, exDef=-5
	~WD();
	
	void know();
	bool getVisible() const;
};

//************************************************
//************************************************

class Treasure : public Item {
	Dragon *d;
	const int gold;

	public:
	Treasure(int amount);
	~Treasure();

	int getGold() const;
};

class DragonHoard : public Treasure {
	Dragon *d;
	bool guard;

	public:
	DragonHoard();
	~DragonHoard();

	
	void unguard(); //used when dragon is slan
	bool getGuard() const; //used to determine whether the dragon is still alive
	void setDragon(Dragon *a); //set the dragon who guards this hoard
	void findPlayer(); //if player is come, tell the dragon
};

#endif
