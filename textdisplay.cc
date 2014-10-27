#include <iostream>
#include <sstream>
#include "textdisplay.h"
#include "player.h"
using namespace std;

TextDisplay::TextDisplay(int f, bool canStorePotion) : p(Player::getPlayer()), f(f),
 canStorePotion(canStorePotion){
	action = "Player character has spawned.";
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) theDisplay[i][j]=' ';
	}
}

void TextDisplay::notify(int r, int c, char ch) {
	theDisplay[r][c] = ch;
}

void TextDisplay::coodPrint(char c) {
	if(c=='@'||c=='\\'){
		attron(COLOR_PAIR(5));
		addch(c);
		attron(COLOR_PAIR(1));
	} else if(c=='G'){
		attron(COLOR_PAIR(2));
		addch(c);
		attron(COLOR_PAIR(1));
	} else if (c=='P'){
		attron(COLOR_PAIR(3));
		addch(c);
		attron(COLOR_PAIR(1));
	} else if(c=='D'){
		attron(COLOR_PAIR(4));
		addch(c);
		attron(COLOR_PAIR(1));
	} else if(c=='M'){
		attron(COLOR_PAIR(6));
		addch(c);
		attron(COLOR_PAIR(1));
	} else {
		attron(COLOR_PAIR(1));
		addch(c);
	}
}

void TextDisplay::setAction(string ac) {action+=ac;}

void TextDisplay::clearAction() {action="";}

TextDisplay::~TextDisplay() {}

void TextDisplay::print() {
	clear();
	attron(COLOR_PAIR(1));
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) coodPrint(theDisplay[i][j]);
		addch('\n');
	}
	ostringstream ossa;
	ossa<<p->getGold();
	string a=ossa.str();
	ostringstream ossb;
	ossb<<f;
	string b=ossb.str();
	ostringstream ossc;
	if(p->getHp()>=0) ossc<<p->getHp();
	else ossc<<0;
	string c=ossc.str();
	ostringstream ossd;
	ossd<<p->getAtk();
	string d=ossd.str();
	ostringstream osse;
	osse<<p->getDef();
	string e=osse.str();
	printw(("Race: "+p->getRace()+" Gold: "+a+"                                              floor "+b+"\n").c_str());
	printw(("HP: "+c+"\n").c_str());
	printw(("Atk: "+d+"\n").c_str());
	printw(("Def: "+e+"\n").c_str());
	printw(("Action: "+action+"\n").c_str());
	if(canStorePotion) printw(("Potion list: "+p->printPotion()+"\n").c_str());
}

ostream &operator<<(ostream &out, const TextDisplay &td) {
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) out<<td.theDisplay[i][j];
		out<<"\n";
	}
	out<<"Race: "<<td.p->getRace()<<" "<<"Gold: "<<
	td.p->getGold()<<"                        "<<
	"                      "<<"floor "<<td.f<<"\n";
	if(td.p->getHp()>=0) out<<"HP: "<<td.p->getHp()<<"\n";
	else out<<"HP: "<<0<<"\n";
	out<<"Atk: "<<td.p->getAtk()<<"\n";
	out<<"Def: "<<td.p->getDef()<<"\n";
	out<<"Action: "<<td.action<<"\n";
	if(td.canStorePotion) out<<"Potion list: "<<td.p->printPotion()<<"\n";
	
	return out;
}
