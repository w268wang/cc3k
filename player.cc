#include "player.h"
#include "item.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
using namespace std;

//Singleton Pattern
Player *Player::pc=0;
void Player::cleanup() {delete pc;}
void Player::setPlayer(Player *p) {pc=p; atexit (cleanup);}
Player *Player::getPlayer() {return pc;}

//Public part
Player::Player(int hp, int atk, int def, string race) :
 Character(hp, atk, def), race(race), oriHp(hp), maxAcc(1), acc(0), exAtk(0), exDef(0) {
	for(int i=0; i<3; i++) potions[i]=0;
	af = new Base;
}

int Player::storePotion(Item *i) {
	for(int j=0; j<3; j++) {
		if(!potions[j]) {potions[j]=i; return 1;}
	}
	return 0;
}
Item *Player::usePotion(int i) {
	if(!potions[i]) return 0;
	else {
		Item *tmp = potions[i];
		potions[i]=0;
		return tmp;
	}
}
string Player::printPotion() {
	string tmp="";
	for(int j=0; j<3; j++) {
		ostringstream oss;
		oss << j;
		if(potions[j]) {
			string temp="Unknown";
			if(potions[j]->getVisible()) {temp=potions[j]->getName();}
			tmp=tmp+oss.str()+". "+temp+" ";
		} else {tmp=tmp+oss.str()+".     ";}
	}
	return tmp;
}

//int Player::getAtk() const {return atk+exAtk;}
//int Player::getDef() const {return def+exDef;}
int Player::getAtk() const {return atk+af->getAtk();}
int Player::getDef() const {return def+af->getDef();}
void Player::addGold(int i) {gold+=i;cout << i<<" "<<gold<<endl;}
//void Player::addAtk(int i) {exAtk+=i;}
//void Player::addDef(int i) {exDef+=i;}
void Player::addAtk(int i) {af=new Atkaffect(af, i);}
void Player::addDef(int i) {af=new Defaffect(af, i);}
void Player::returnGold(int i) {gold+=i;}
void Player::buyAtk(int i) {atk+=i;}
void Player::buyDef(int i) {def+=i;}
void Player::addHp(int i) {hp+=i; if(hp<=0) hp=1;}
void Player::addAcc() {acc++;}
void Player::addMaxAcc() {maxAcc++;}
int Player::getMaxAcc() const {return maxAcc;}
int Player::getOriHp() const {return oriHp;}
int Player::getAcc() const {return acc;}
void Player::clearAcc() {acc=0;}
string Player::getRace() const {return race;}
void Player::lossHp(int i) {hp-=i;}
void Player::lossGold(int i) {gold-=i;}
/*void Player::clear() {
	exAtk=0;
	exDef=0;
}*/
void Player::clear() {
	delete af;
	af = new Base;
}
Player::~Player() {for(int i=0; i<3; i++) {delete potions[i];} delete af;}

Human::Human() : Player(140, 20, 20, "Human"){}
Human::~Human(){}

Dwarf::Dwarf() : Player(100, 20, 30, "Dwarf"){}
Dwarf::~Dwarf(){}
void Dwarf::addGold(int i) {gold+=i*2;}

Orc::Orc() : Player(180, 30, 25, "Orc"){}
Orc::~Orc(){}
void Orc::addGold(int i) {
	if(i%2==0) gold+=(i/2);
	else gold=gold+(i/2)+1;
}

Elves::Elves() : Player(140, 30, 10, "Elves"){}
Elves::~Elves(){}
void Elves::addAtk(int i) {int tmp=((i>0)? i : -i); af=new Atkaffect(af, tmp);}
void Elves::addDef(int i) {int tmp=((i>0)? i : -i); af=new Defaffect(af, tmp);}
void Elves::addHp(int i) {hp+=((i>0)? i : -i);}
//void Elves::addAtk(int i) {exAtk+=((i>0)? i : -i);}
//void Elves::addDef(int i) {exDef+=((i>0)? i : -i);}
//void Elves::addHp(int i) {hp+=((i>0)? i : -i);}

Prof::Prof() : Player(15000, 2000, 2000, "Prof") {gold=15000;}
Prof::~Prof() {}

Ta::Ta() : Player(10000, 1000, 1000, "Ta") {gold=10000;}
Ta::~Ta() {}
