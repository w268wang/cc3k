#include "affect.h"
using namespace std;

Affect::~Affect() {}

Decorator::Decorator(Affect *component): component(component) {}
Decorator::~Decorator() {delete component;}

Base::Base() {}
int Base::getAtk() {return 0;}
int Base::getDef() {return 0;}
Base::~Base() {}

Atkaffect::Atkaffect(Affect *component, int i): Decorator(component), exAtk(i) {}
int Atkaffect::getAtk() {return component->getAtk()+exAtk;}
int Atkaffect::getDef() {return component->getDef();}
Atkaffect::~Atkaffect() {}

Defaffect::Defaffect(Affect *component, int i): Decorator(component), exDef(i) {}
int Defaffect::getAtk() {return component->getAtk();}
int Defaffect::getDef() {return component->getDef()+exDef;}
Defaffect::~Defaffect() {}


