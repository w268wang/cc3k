#include <string>
#include <iostream>
#include "enemy.h"
#include "item.h"

using namespace std;

Enemy::Enemy(int hp, int atk, int def, string type) : Character(hp, atk, def), type(type) {}
string Enemy::getType() const {return type;}
void Enemy::addHp(int i) {hp+=i;}
int Enemy::getAtk() const {return atk;}
int Enemy::getDef() const {return def;}
int Enemy::returnGold() const {}
void Enemy::angry() {}
void Enemy::notify() {}
void Enemy::addGold(int i) {}
void Enemy::reborn(){}
bool Enemy::getState() const {}
Enemy::~Enemy() {}

Vampire::Vampire() : Enemy(50, 25, 25, "vampire") {gold=1;}
Vampire::~Vampire() {}

Werewolf::Werewolf() : Enemy(120, 30, 5, "werewolf"), wolf(false) {gold=1;}
Werewolf::~Werewolf() {}
bool Werewolf::getState() const {return wolf;}
void Werewolf::angry() {atk+=def; def=0; wolf=true;}

Troll::Troll() : Enemy(120, 25, 15, "troll") {gold=1;}
Troll::~Troll() {}

Goblin::Goblin() : Enemy(70, 5, 10, "goblin") {gold=1; pgold=0;}
Goblin::~Goblin() {}
void Goblin::addGold(int i) {pgold+=i;}
int Goblin::returnGold() const {return pgold;}

bool Merchant::state = false;
Merchant::Merchant() : Enemy(30, 75, 5, "merchant") {}
Merchant::~Merchant() {}
void Merchant::angry() {state = true;}
bool Merchant::getState() const {return state;}

Dragon::Dragon(Item *padh) : Enemy(150, 20, 20, "dragon"), dh(padh) {
	state = false;
}
Dragon::~Dragon() {}
void Dragon::notify() {dh->unguard();}
void Dragon::angry() {state = true;}
bool Dragon::getState() const {return state;}

Phoenix::Phoenix() : Enemy(50, 35, 20, "phoenix") {gold=1;}
Phoenix::~Phoenix() {}
void Phoenix::reborn() {hp=50;}
