#include "item.h"
#include "enemy.h"
#include <string>

using namespace std;

Item::Item(string type, string name) : type(type), name(name) {}
std::string Item::getType() const {return type;}
std::string Item::getName() const {return name;}

int Item::getHp() const {}
int Item::getAtk() const {}
int Item::getDef() const {}
bool Item::getVisible() const {}
void Item::know() {}

void Item::unguard() {}
bool Item::getGuard() const{}
int Item::getGold() const {}
void Item::setDragon(Dragon *a) {}
void Item::findPlayer() {}
Item::~Item() {}

Potion::Potion(string type, string name, int hp, int exAtk, int exDef) : 
Item(type, name), hp(hp), exAtk(exAtk), exDef(exDef) {}

int Potion::getHp() const {return hp;}
int Potion::getAtk() const {return exAtk;}
int Potion::getDef() const {return exDef;}

Potion::~Potion() {}

bool RH::visible=false;
RH::RH() : Potion("potion", "RH", 10, 0, 0) {}
RH::~RH() {}
bool RH::getVisible() const {return visible;}
void RH::know() {visible = true;}

bool BA::visible=false;
BA::BA() : Potion("potion", "BA", 0, 5, 0) {}
BA::~BA() {}
bool BA::getVisible() const {return visible;}
void BA::know() {visible = true;}

bool BD::visible=false;
BD::BD() : Potion("potion", "BD", 0, 0, 5) {}
BD::~BD() {}
bool BD::getVisible() const {return visible;}
void BD::know() {visible = true;}

bool PH::visible=false;
PH::PH() : Potion("potion", "PH", -10, 0, 0) {}
PH::~PH() {}
bool PH::getVisible() const {return visible;}
void PH::know() {visible = true;}

bool WA::visible=false;
WA::WA() : Potion("potion", "WA", 0, -5, 0) {}
WA::~WA() {}
bool WA::getVisible() const {return visible;}
void WA::know() {visible = true;}

bool WD::visible=false;
WD::WD() : Potion("potion", "WD", 0, 0, -5) {}
WD::~WD() {}
bool WD::getVisible() const {return visible;}
void WD::know() {visible = true;}

//******************************************************

Treasure::Treasure(int amount) : Item("treasure", ""), gold(amount) {
	if(amount==1) name = "Normal horde";
	else if(amount==2) name = "Small horde";
	else if(amount==4) name = "merchant hoard";
	else if(amount==6) name = "Dragon hoard";
}

Treasure::~Treasure() {}

int Treasure::getGold() const {return gold;}

DragonHoard::DragonHoard() : Treasure(6), guard(true) {
	d=0;
}

DragonHoard::~DragonHoard() {}

void DragonHoard::unguard() {guard=false;}
bool DragonHoard::getGuard() const {return guard;}
void DragonHoard::setDragon(Dragon *a) {d=a;}

void DragonHoard::findPlayer() {if(d) d->angry();}

