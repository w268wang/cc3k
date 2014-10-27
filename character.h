#ifndef _CHARACTER_H_
#define _CHARACTER_H_


class Character {
	protected:
	int hp;
	int atk;
	int def;
	int gold; // amount will drop for monster
	Character(int hp, int atk, int def);
	public:
	int getGold() const;
	int getHp() const;
	virtual ~Character();
};

#endif
