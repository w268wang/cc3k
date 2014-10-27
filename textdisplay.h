#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <iostream>
#include <sstream>
#include <string>

#include <ncurses.h>
#include "player.h"

const int MAXR=25;
const int MAXC=79;

class TextDisplay {
	char theDisplay[MAXR][MAXC]; //save the display version of a floor
	std::string action; //save the information that will be shown after "action:"
	Player *p; //an pointer to player to gain information
	int f; //show which floor this is
	bool canStorePotion; //determine whether to show the potion list

	public:
	TextDisplay(int f, bool canStorePotion); //ctor
	void notify(int r, int c, char ch); //cells will call this to let the display know which char it should display
	void setAction(std::string ac); //methods in Floor will call this to let the display know what action to show
	void clearAction();
	~TextDisplay();

	void print();
	void coodPrint(char c);
	
	friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
