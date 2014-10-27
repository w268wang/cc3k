#ifndef __AFFECT_H__
#define __AFFECT_H__
#include <iostream>
#include <string>

class Affect {
  public:
    virtual int getAtk()=0;
    virtual int getDef()=0;
    virtual ~Affect();
};

class Base : public Affect {
	public:
	Base();
	int getAtk();
    	int getDef();
	~Base();
};

class Decorator : public Affect {
	protected:
		Affect *component;
	public:
		Decorator(Affect *component);
		virtual ~Decorator();
};

class Atkaffect : public Decorator {
	int exAtk;
	public:
	Atkaffect(Affect *component, int i);
	int getAtk();
    	int getDef();
	~Atkaffect();
};

class Defaffect : public Decorator {
	int exDef;
	public:
	Defaffect(Affect *component, int i);
	int getAtk();
    	int getDef();
	~Defaffect();
};

#endif
