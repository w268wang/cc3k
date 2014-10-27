#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include "floor.h"
#include "textdisplay.h"

using namespace std;

Floor::Floor(int f, bool canBuy, bool canRecover, bool special, bool canStorePotion, bool useCurses) :
 over(0), pr(0), pc(0), canBuy(canBuy), canRecover(canRecover), special(special),
 canStorePotion(canStorePotion), useCurses(useCurses) {
	td = new TextDisplay(f, canStorePotion);
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) theFloor[i][j]=0;
	}
	for(int i=0; i<20; i++) {
		for(int j=0; j<2; j++) moved[i][j]=0;
	}
}

Floor::~Floor() {
	delete td;
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++)  delete theFloor[i][j];
	}
}

int Floor::isOver() const {return over;}

void Floor::init() {
	string fname = "map.in";
	int numGold = 10;
	ifstream in(fname.c_str());
	int tnum=0;
	const int NUMROOM=5;
	int roomnum[NUMROOM];
	for(int i=0; i<NUMROOM; i++) roomnum[i]=0;
	int tmpnum=0;
	int tmpr=0;
	int tmpc=0;
	int locnum=0;
	char dec;
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) {
			in >> dec;
			if(dec>='0'&&dec<'5') {
				tnum++;
				roomnum[dec-'0']++;
			}
			crea(i, j, dec);
		}
	}
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) {
			init(i, j);
		}
	}
	//intial player location
	srand(time(NULL));
	tmpnum = (rand()%NUMROOM);
	locnum = (rand()%roomnum[tmpnum])+1;
	locat(&tmpr, &tmpc, locnum, tmpnum);
	theFloor[tmpr][tmpc]->pushPlayer(Player::getPlayer());
	pr = tmpr;
	pc = tmpc;
	//initial doorway location
	int tmptmpnum = tmpnum;
	tmpnum = (rand()%NUMROOM);
	while(tmpnum==tmptmpnum){tmpnum = (rand()%NUMROOM);}
	locnum = (rand()%roomnum[tmpnum])+1;
	locat(&tmpr, &tmpc, locnum, tmpnum);
	bool canDropWay = true;
	for(int i=0; i<8; i++) {
		char tmpcharDOOR = (theFloor[tmpr][tmpc]->getNeighbours())[i]->getSym();
		if(tmpcharDOOR=='+'||(theFloor[tmpr][tmpc]->getNeighbours())[4]->getSym()=='|')
		{canDropWay=false; break;}
	}
	while(!theFloor[tmpr][tmpc]->available()||!canDropWay){
		locnum = (rand()%tnum)+1;
		locat(&tmpr, &tmpc, locnum, tmpnum);
		canDropWay = true;
		for(int i=0; i<8; i++) {
			char tmpcharDOOR = (theFloor[tmpr][tmpc]->getNeighbours())[i]->getSym();
			if(tmpcharDOOR=='+'||(theFloor[tmpr][tmpc]->getNeighbours())[4]->getSym()=='|')
			{canDropWay=false; break;}
		}
	}
	theFloor[tmpr][tmpc]->setDoorway();
	//initial potion location
	for(int i=0; i<10; i++) {
		locnum = (rand()%tnum)+1;
		locat(&tmpr, &tmpc, locnum, 10);
		while(!theFloor[tmpr][tmpc]->available()){
			locnum = (rand()%tnum)+1;
			locat(&tmpr, &tmpc, locnum, 10);
		}
		locnum = (rand()%6);
	if(locnum==0) theFloor[tmpr][tmpc]->pushItem(new RH());
	else if(locnum==1) theFloor[tmpr][tmpc]->pushItem(new BA());
	else if(locnum==2) theFloor[tmpr][tmpc]->pushItem(new BD());
	else if(locnum==3) theFloor[tmpr][tmpc]->pushItem(new WA());
	else if(locnum==4) theFloor[tmpr][tmpc]->pushItem(new WD());
	else theFloor[tmpr][tmpc]->pushItem(new BA());
	}
	//initial gold position
	for(int i=0; i<numGold; i++) {
		locnum = (rand()%tnum)+1;
		locat(&tmpr, &tmpc, locnum, 10);
		while(!theFloor[tmpr][tmpc]->available()){
			locnum = (rand()%tnum)+1;
			locat(&tmpr, &tmpc, locnum, 10);
		}
		locnum = (rand()%8);
	if(locnum<5) theFloor[tmpr][tmpc]->pushItem(new Treasure(1));
	else if(locnum==5) {
		theFloor[tmpr][tmpc]->pushItem(new DragonHoard());
		Cell **tmp = theFloor[tmpr][tmpc]->getNeighbours();
		locnum = (rand()%8);
		while(!tmp[locnum]->available()) {locnum = (rand()%8);}
		Dragon *tmpdra = new Dragon(theFloor[tmpr][tmpc]->getItem());	
		theFloor[tmpr][tmpc]->getItem()->setDragon(tmpdra);
		tmp[locnum]->pushEnemy(tmpdra);
	}
	else theFloor[tmpr][tmpc]->pushItem(new Treasure(2));
	}
	//initial enemy location
	for(int i=0; i<20; i++) {
		locnum = (rand()%tnum)+1;
		locat(&tmpr, &tmpc, locnum, 10);
		while(!theFloor[tmpr][tmpc]->available()){
			locnum = (rand()%tnum)+1;
			locat(&tmpr, &tmpc, locnum, 10);
		}
		locnum = (rand()%18);
	if(locnum<4) theFloor[tmpr][tmpc]->pushEnemy(new Werewolf());
	else if(locnum<7) theFloor[tmpr][tmpc]->pushEnemy(new Vampire());
	else if(locnum<12) theFloor[tmpr][tmpc]->pushEnemy(new Goblin());
	else if(locnum<14) theFloor[tmpr][tmpc]->pushEnemy(new Troll());
	else if(locnum<16) theFloor[tmpr][tmpc]->pushEnemy(new Phoenix());
	else theFloor[tmpr][tmpc]->pushEnemy(new Merchant());
	}
	//notify the display
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) {
			theFloor[i][j]->notifyDisplay(*td);
		}
	}
}

int Floor::pmove(char d1, char d2) {
	Cell **tmp = theFloor[pr][pc]->getNeighbours();
	Cell *ctmp = 0;
	string d="";
	transdir(d1, d2, d);
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];

	if(!ctmp) {td->setAction("Invalid command! ");}
	else if(ctmp->getType()=="E") {td->setAction("Cannot go there! ");}
	else if(ctmp->getType()=="D"&&d=="we") {
		over=2;
		td->setAction("Arrive doorway. ");
		return 1;
	}
	else if(ctmp->getType()=="T"&&ctmp->getContain()=='G') {
		if(ctmp->getItem()->getGold()==6) 
		{if(ctmp->getItem()->getGuard())
			{td->setAction("You must first kill the dragon! "); return 0;}}
		obtainGold(theFloor[pr][pc], ctmp); return 1;
	}
	else if(ctmp->getType()=="P"||(ctmp->getType()=="T"&&ctmp->available())) {doMove(theFloor[pr][pc], ctmp, d);return 1;}
	else {td->setAction("Target is occupied. "); return 0;}
}

int Floor::pmove(string d) {
	Cell **tmp = theFloor[pr][pc]->getNeighbours();
	Cell *ctmp = 0;
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];

	if(!ctmp) {td->setAction("Invalid command! ");}
	else if(ctmp->getType()=="E") {td->setAction("Cannot go there! ");}
	else if(ctmp->getType()=="D"&&d=="we") {
		over=2;
		td->setAction("Arrive doorway. ");
		return 1;
	}
	else if(ctmp->getType()=="T"&&ctmp->getContain()=='G') {
		if(ctmp->getItem()->getGold()==6) 
		{if(ctmp->getItem()->getGuard())
			{td->setAction("You must first kill the dragon! "); return 0;}}
		obtainGold(theFloor[pr][pc], ctmp); return 1;
	}
	else if(ctmp->getType()=="P"||(ctmp->getType()=="T"&&ctmp->available())) {doMove(theFloor[pr][pc], ctmp, d);return 1;}
	else {td->setAction("Target is occupied. "); return 0;}
}

void Floor::havePotion(string d) {
	Cell *tmpp = theFloor[pr][pc];
	Cell **tmp = tmpp->getNeighbours();
	Cell *ctmp = 0;
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(!ctmp||ctmp->getSym()!='P') {td->setAction("Invalid command! "); return;}
	Item *tmpi = ctmp->popItem();
	td->setAction("PC uses "+tmpi->getName()+". ");
	tmpi->know();
	Player *tmppp = tmpp->popPlayer();
	tmppp->addHp(tmpi->getHp());
	tmppp->addAtk(tmpi->getAtk());
	tmppp->addDef(tmpi->getDef());
	delete tmpi;
	ctmp->pushPlayer(tmppp);
	int m, n;
	ctmp->getPos(&m, &n);
	pr=m;
	pc=n;
}
void Floor::storePotion(string d) {
	Cell *tmpp = theFloor[pr][pc];
	Cell **tmp = tmpp->getNeighbours();
	Cell *ctmp = 0;
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(!ctmp||ctmp->getSym()!='P') {td->setAction("Invalid command! "); return;}
	if(!Player::getPlayer()->storePotion(0)) {td->setAction("Your bag is full! "); return;}
	Item *tmpi = ctmp->popItem();
	Player::getPlayer()->storePotion(tmpi);
	string content = "n Unknown potion";
	if(tmpi->getVisible()) {
		content = content+" "+tmpi->getName();
	}
	td->setAction("PC stores a"+content+". ");
	Player *tmppp = tmpp->popPlayer();
	ctmp->pushPlayer(tmppp);
	int m, n;
	ctmp->getPos(&m, &n);
	pr=m;
	pc=n;
}

int Floor::attack(string d) {
	Cell **tmp = theFloor[pr][pc]->getNeighbours();
	Cell *ctmp = 0;
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(ctmp->getType()=="T"&&((ctmp->getContain()=='E')||
	(ctmp->getContain()=='M')
	||(ctmp->getContain()=='D'))) {
		if(ctmp->getContain()!='E') ctmp->getEnemy()->angry();
		fight(ctmp);
		return 1;
	} else {td->setAction("Invalid command! "); return 0;}
}

void Floor::havePotion(char d1, char d2) {
	Cell *tmpp = theFloor[pr][pc];
	Cell **tmp = tmpp->getNeighbours();
	Cell *ctmp = 0;
	string d="";
	transdir(d1, d2, d);
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(!ctmp||ctmp->getSym()!='P') {td->setAction("Invalid command! "); return;}
	Item *tmpi = ctmp->popItem();
	td->setAction("PC uses "+tmpi->getName()+". ");
	tmpi->know();
	Player *tmppp = tmpp->popPlayer();
	tmppp->addHp(tmpi->getHp());
	tmppp->addAtk(tmpi->getAtk());
	tmppp->addDef(tmpi->getDef());
	delete tmpi;
	ctmp->pushPlayer(tmppp);
	int m, n;
	ctmp->getPos(&m, &n);
	pr=m;
	pc=n;
}
void Floor::storePotion(char d1, char d2) {
	Cell *tmpp = theFloor[pr][pc];
	Cell **tmp = tmpp->getNeighbours();
	Cell *ctmp = 0;
	string d="";
	transdir(d1, d2, d);
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(!ctmp||ctmp->getSym()!='P') {td->setAction("Invalid command! "); return;}
	if(!Player::getPlayer()->storePotion(0)) {td->setAction("Your bag is full! "); return;}
	Item *tmpi = ctmp->popItem();
	Player::getPlayer()->storePotion(tmpi);
	string content = "n Unknown potion";
	if(tmpi->getVisible()) {
		content = content+" "+tmpi->getName();
	}
	td->setAction("PC stores a"+content+". ");
	Player *tmppp = tmpp->popPlayer();
	ctmp->pushPlayer(tmppp);
	int m, n;
	ctmp->getPos(&m, &n);
	pr=m;
	pc=n;
}
void Floor::usePotion(int i) {
	Item *tmpi = Player::getPlayer()->usePotion(i);
	if(!tmpi) return;
	td->setAction("PC uses "+tmpi->getName()+". ");
	tmpi->know();
	Player::getPlayer()->addHp(tmpi->getHp());
	Player::getPlayer()->addAtk(tmpi->getAtk());
	Player::getPlayer()->addDef(tmpi->getDef());
	delete tmpi;
}

int Floor::attack(char d1, char d2) {
	Cell **tmp = theFloor[pr][pc]->getNeighbours();
	Cell *ctmp = 0;
	string d="";
	transdir(d1, d2, d);
	if(d=="nw") ctmp=tmp[0];
	else if(d=="no") ctmp=tmp[1];
	else if(d=="ne") ctmp=tmp[2];
	else if(d=="we") ctmp=tmp[3];
	else if(d=="ea") ctmp=tmp[4];
	else if(d=="sw") ctmp=tmp[5];
	else if(d=="so") ctmp=tmp[6];
	else if(d=="se") ctmp=tmp[7];
	if(ctmp->getType()=="T"&&((ctmp->getContain()=='E')||
	(ctmp->getContain()=='M')
	||(ctmp->getContain()=='D'))) {
		if(ctmp->getContain()!='E') ctmp->getEnemy()->angry();
		fight(ctmp);
		return 1;
	} else {td->setAction("Invalid command! "); return 0;}
}

void Floor::endStep() {
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++){
			Cell *ctmp = theFloor[i][j];
			bool occupy=false;
			if(ctmp->getType()=="T"&&(ctmp->getContain()=='E'||
			(ctmp->getContain()=='M'&&ctmp->getEnemy()->getState())
			||(ctmp->getContain()=='D'&&ctmp->getEnemy()->getState()))) {
				Cell **tmp = ctmp->getNeighbours();
				for(int k=0; k<8; k++) {if(tmp[k]->getSym()=='@') {fight(ctmp); occupy=true; break;}}
			}
			if((!occupy)&&ctmp->getType()=="T"&&(ctmp->getContain()=='E'
			||ctmp->getContain()=='M')) {
				bool m = true;
				for(int l=0; l<20; l++) {
					if(moved[l][0]==i&&moved[l][1]==j) {m=false; break;}
				}
				if(m) {emove(ctmp);}
			}
		}
	}
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++){
			Cell *ctmp = theFloor[i][j];
			if(ctmp->getSym()=='G'&&ctmp->getItem()->getGold()==6){
			Cell **tmp = ctmp->getNeighbours();
			for(int k=0; k<8; k++) {
			if(tmp[k]->getSym()=='D') {
			for(int l=0; l<8; l++) {
				if(tmp[l]->getSym()=='@') {
					ctmp->getItem()->findPlayer();
					int dpos=(k>=0&&k<3)? k:(k==3)? 7:(k==4)? 3:(k==5)? 6:(k==6)? 5:4;
					int ppos=(l>=0&&l<3)? l:(l==3)? 7:(l==4)? 3:(l==5)? 6:(l==6)? 5:4;
					int dmov = 0;
					if(ppos-dpos<=4&&ppos-dpos>-4) {
						dmov=(dpos==7)? 0:dpos+1;
						dmov=(dmov>=0&&dmov<3)? dmov:(dmov==3)? 4:(dmov==4)? 7:(dmov==5)? 6:(dmov==6)? 5:3;
						if(tmp[dmov]->getSym()=='|'||tmp[dmov]->getSym()=='-') dmov=(dpos==0)? 7:dpos-1;
					}
					else {
						dmov=(dpos==0)? 7:dpos-1;
						dmov=(dmov>=0&&dmov<3)? dmov:(dmov==3)? 4:(dmov==4)? 7:(dmov==5)? 6:(dmov==6)? 5:3;
						if(tmp[dmov]->getSym()=='|'||tmp[dmov]->getSym()=='-') dmov=(dpos==0)? 7:dpos-1;
					}
					if(tmp[dmov]->getSym()=='.') {
						Enemy *tmpEne = tmp[k]->popEnemy();
						tmp[dmov]->pushEnemy(tmpEne);
					}
					break;
				}
			}
			break;}}}
		}
	}
	for(int i=0; i<20; i++) {
		for(int j=0; j<2; j++) moved[i][j]=0;
	}
	if(canRecover) {
		if(Player::getPlayer()->getOriHp()>Player::getPlayer()->getHp()) 
		 {Player::getPlayer()->addAcc();}
		else Player::getPlayer()->clearAcc();
		if(Player::getPlayer()->getAcc()>Player::getPlayer()->getMaxAcc()) {
		Player::getPlayer()->addHp(2);
		Player::getPlayer()->clearAcc();
		Player::getPlayer()->addMaxAcc();
		td->setAction("PC recovers 2 hp. ");
		}
	}
	//notify the display
	for(int i=0; i<MAXR; i++) {
		for(int j=0; j<MAXC; j++) {
			theFloor[i][j]->notifyDisplay(*td);
		}
	}
	if(useCurses) td->print();
}

void Floor::clearAction() {td->clearAction();}

void Floor::locat(int *r, int *c, int locnum, int room) {
	int tmp = locnum;
	if(room==10){
		for(int i=0; i<MAXR; i++) {
			for(int j=0; j<MAXC; j++){
				if(theFloor[i][j]->getType()=="T") tmp--;
				if(tmp==0) {*r=i; *c=j; return;}
			}
		}
	} else {
		for(int i=0; i<MAXR; i++) {
			for(int j=0; j<MAXC; j++){
				if(theFloor[i][j]->getType()=="T"&&
				theFloor[i][j]->getRoom()==room) tmp--;
				if(tmp==0) {*r=i; *c=j; return;}
			}
		}
	}
}

void Floor::obtainGold(Cell *p, Cell *g) {
	Player *tmpp = p->popPlayer();
	Item *tmpi = g->popItem();
	int amount = tmpi->getGold();
	delete tmpi;
	tmpp->addGold(amount);
	g->pushPlayer(tmpp);
	int m, n;
	g->getPos(&m, &n);
	pr=m;
	pc=n;
	ostringstream os;
	os << amount;
	td->setAction("PC obtain "+os.str()+" gold(s). ");
}

void Floor::doMove(Cell *s, Cell *e, string d) {
	Cell **tmp = s->getNeighbours();
	string ma[8] = {"North West", "North", "North East",
	 "West", "East", "South West", "South", "South East"}; 
	if(canBuy){for(int i=0; i<8; i++) {
	if (tmp[i]->getSym()=='M'&&(!tmp[i]->getEnemy()->getState())&&Player::getPlayer()->getGold()>4) {
		if(useCurses) {
		printw(("Do you want to trade with the Merchant on your "+ma[i]+"?\n(Y)es   (N)o\n").c_str());
		char input=0;
		input=getch();
		if(cin.eof()) {break;}
		if(input!='Y'&&input!='y') {printw("No? Ok, then. Bye.\n"); continue;}
		if(Player::getPlayer()->getGold()>9) {
			printw("You can buy\n (h) Heart of Tarrasque(+25 hp) --10 gold\n (a) Divine Rapier(+15 atk) --10 gold\n or (d) David's Guard(+15 def) --10 gold\nOr (l) so you want to have the primary options(which are all 5 gold)\n");
			input=getch();
			if(input!='h'&&input!='a'&&input!='d'&&input!='l')
			 {printw("No? Ok, then. Bye.\n"); continue;}
			if(input!='l') {
				string thing="";
				Player::getPlayer()->lossGold(10);
				if(input=='h') {Player::getPlayer()->addHp(25); thing="Heart of Tarrasque(+25 hp)";}
				else if(input=='a') {Player::getPlayer()->buyAtk(15); thing="Divine Rapier(+15 atk)";}
				else if(input=='d') {Player::getPlayer()->buyDef(15); thing="David's Guard(+15 def)";}
				td->setAction("You just bought "+thing+"\n");
				continue;
			}
		}
		printw("You can buy\n (h) Healing Salve(+10 hp) --5 gold\n (a) Claws of Attack(+5 atk) --5 gold\n or (d) Chain Mail(+5 def) --5 gold\n");
		input=getch();
		if(input!='h'&&input!='a'&&input!='d')
		 {printw("No? Ok, then. Bye.\n"); continue;}
		Player::getPlayer()->lossGold(5);
		string lthing="";
		if(input=='h') {Player::getPlayer()->addHp(10); lthing="Healing Salve(+10 hp)";}
		else if(input=='a') {Player::getPlayer()->buyAtk(5); lthing="Claws of Attack(+5 atk)";}
		else if(input=='d') {Player::getPlayer()->buyDef(5); lthing="Chain Mail(+5 def)";}
		td->setAction("You just bought "+lthing+"\n");
		} else {
		cout<<"Do you want to trade with the Merchant on your "<<ma[i]<<"?\n(Y)es   (N)o"<<endl;
		string input="";
		cin>>input;
		if(cin.eof()) {break;}
		if(input!="Y"&&input!="y") {cout<<"No? Ok, then. Bye."<<endl; continue;}
		if(Player::getPlayer()->getGold()>9) {
			cout<<"You can buy\n (H) Heart of Tarrasque(+25 hp) --10 gold\n"<<
			" (A) Divine Rapier(+15 atk) --10 gold\n"<<
			" or (D) David's Guard(+15 def) --10 gold"<<
			"Or (L) so you want to have the primary options(which are all 5 gold)"<<endl;
			cin>>input;
			if(input!="H"&&input!="A"&&input!="D"&&input!="L")
			 {cout<<"No? Ok, then. Bye."<<endl; continue;}
			if(input!="L") {
				string thing="";
				Player::getPlayer()->lossGold(10);
				if(input=="H") {Player::getPlayer()->addHp(25); thing="Heart of Tarrasque(+25 hp)";}
				else if(input=="A") {Player::getPlayer()->buyAtk(15); thing="Divine Rapier(+15 atk)";}
				else if(input=="D") {Player::getPlayer()->buyDef(15); thing="David's Guard(+15 def)";}
				cout << "You just bought "<<thing<<endl;
				continue;
			}
		}
		cout<<"You can buy\n (H) Healing Salve(+10 hp) --5 gold\n"<<
			" (A) Claws of Attack(+5 atk) --5 gold\n"<<
			" or (D) Chain Mail(+5 def) --5 gold"<<endl;
		cin>>input;
		if(input!="H"&&input!="A"&&input!="D")
		 {cout<<"No? Ok, then. Bye."<<endl; continue;}
		Player::getPlayer()->lossGold(5);
		string lthing="";
		if(input=="H") {Player::getPlayer()->addHp(10); lthing="Healing Salve(+10 hp)";}
		else if(input=="A") {Player::getPlayer()->buyAtk(5); lthing="Claws of Attack(+5 atk)";}
		else if(input=="D") {Player::getPlayer()->buyDef(5); lthing="Chain Mail(+5 def)";}
		cout << "You just bought "<<lthing<<endl;
		}
	}}}
	Player *tmpp = s->popPlayer();
	e->pushPlayer(tmpp);
	int m, n;
	e->getPos(&m, &n);
	pr=m;
	pc=n;
	td->setAction("PC moves "+d+" ");
	tmp = e->getNeighbours();
	for(int i=0; i<8; i++) {
		//Search for potion and/or Treasure
		string content = "";
		if(tmp[i]->getSym()=='P') {
			content = "Unknown potion";
			if(tmp[i]->getItem()->getVisible()) {
				content = tmp[i]->getItem()->getName();
			}
			td->setAction("and see a "+content+" on the "+ma[i]+". ");
		} else if (tmp[i]->getSym()=='G') {
			content = tmp[i]->getItem()->getName();
			td->setAction("and see a "+content+" on the "+ma[i]+". ");
		} else if (tmp[i]->getSym()=='D') {
			content = "Dragon";
			td->setAction("and see a "+content+" on the "+ma[i]+". ");
		}
	}
	
}

void Floor::init(int r, int c) {
	if(r>0) {
		if(c>0) theFloor[r][c]->addNeighbour(0, theFloor[r-1][c-1]);
		theFloor[r][c]->addNeighbour(1, theFloor[r-1][c]);
		if(c<MAXC-1) theFloor[r][c]->addNeighbour(2, theFloor[r-1][c+1]);
	}
	if(c>0) {theFloor[r][c]->addNeighbour(3, theFloor[r][c-1]);}
	if(c<MAXC-1) {theFloor[r][c]->addNeighbour(4, theFloor[r][c+1]);}
	if(r<MAXR-1) {
		if(c>0) theFloor[r][c]->addNeighbour(5, theFloor[r+1][c-1]);
		theFloor[r][c]->addNeighbour(6, theFloor[r+1][c]);
		if(c<MAXC-1) theFloor[r][c]->addNeighbour(7, theFloor[r+1][c+1]);
	}
}

void Floor::crea(int r, int c, char ch) {
	if(ch=='|') theFloor[r][c] = new ECell(r, c, "E", "wall1");
	else if(ch=='-') theFloor[r][c] = new ECell(r, c, "E", "wall2");
	else if(ch=='E') theFloor[r][c] = new ECell(r, c, "E", "empty");
	else if(ch=='+') theFloor[r][c] = new Passenger(r, c, "P", "pass1");
	else if(ch=='#') theFloor[r][c] = new Passenger(r, c, "P", "pass2");
	else if(ch=='0') theFloor[r][c] = new Tile(r,c,0);
	else if(ch=='1') theFloor[r][c] = new Tile(r,c,1);
	else if(ch=='2') theFloor[r][c] = new Tile(r,c,2);
	else if(ch=='3') theFloor[r][c] = new Tile(r,c,3);
	else if(ch=='4') theFloor[r][c] = new Tile(r,c,4);
}



void Floor::emove(Cell *e) {
	int m,n;
	e->getPos(&m, &n);
	Cell **tmp = e->getNeighbours();
	bool canmove=false;
	for(int i=0; i<8; i++) {if(tmp[i]->getSym()=='.') {canmove=true; break;}}
	if(!canmove) return;
	int rannum = rand()%8;
	while(!tmp[rannum]||(tmp[rannum]->getSym()!='.')) rannum = rand()%8;
	Enemy *tmpEne = e->popEnemy();
	tmp[rannum]->pushEnemy(tmpEne);
	tmp[rannum]->getPos(&m, &n);
	for(int i=0; i<20; i++) {
		if(moved[i][0]==0) {moved[i][0]=m; moved[i][1]=n; break;}
	}
}

void Floor::fight(Cell *e) {
	if(canRecover) Player::getPlayer()->clearAcc();
	int ex1 = ((100*Player::getPlayer()->getAtk())%(100+e->getEnemy()->getDef())==0)? 0 : 1;
	int damage1 = (100*Player::getPlayer()->getAtk())/(100+e->getEnemy()->getDef())+ex1;
	e->getEnemy()->addHp(-damage1);
	ostringstream oss1;
	oss1 << damage1;
	int enemyHp = e->getEnemy()->getHp();
	string killstate = "";
	if(enemyHp<=0) {enemyHp=0; killstate = " and kill it";}
	ostringstream os1;
	os1 << enemyHp;
	td->setAction(" PC deals "+oss1.str()+" damage to "+e->getSym()+" ("+os1.str()+")"+killstate+". ");
	//werewolf changes into wolf state
	if(special&&e->getSym()=='W'&&enemyHp<=50&&!e->getEnemy()->getState()) {e->getEnemy()->angry();
	td->setAction("W changed into wolf state. ");}
	if(enemyHp==0) {
		//dragon notify the hoard that it is dead
		if(e->getEnemy()->getType()=="dragon") e->getEnemy()->notify();
		//Phoenix reborn
		int tmprand = rand()%3;
		if(special&&e->getSym()=='X'&&!tmprand) {
			e->getEnemy()->reborn();
			td->setAction("But X is reborn. ");
			return;
		}
		Player::getPlayer()->addGold(e->getEnemy()->getGold());
		if(special&&e->getSym()=='N') Player::getPlayer()->returnGold(e->getEnemy()->returnGold());
		Enemy *tmp=e->popEnemy();
		//merchant gives a hoard
		if(tmp->getType()=="merchant"){
			e->pushItem(new Treasure(4));
			td->setAction("M leaves a merchant hoard. ");
		}
		delete tmp;
		return;
	}
	int locnum = rand()%2;
	if(locnum==0) {td->setAction(e->getEnemy()->getType()+" hits PC, but missed. "); return;}
	int ex2 = ((100*e->getEnemy()->getAtk())%(100+Player::getPlayer()->getDef())==0)? 0 : 1;
	int damage2 = (100*e->getEnemy()->getAtk())/(100+Player::getPlayer()->getDef())+ex2;
	//vampire get extra hp
	if(special&&e->getSym()=='V') {e->getEnemy()->addHp(damage2/4); td->setAction("V gets extra hp. ");}
	//troll bonus hit
	if(special&&e->getSym()=='T') {
		locnum = rand()%2; 
		if(locnum) {
			damage2*=2;
			td->setAction("T gets a bonus hit.");
		}
	} else Player::getPlayer()->lossHp(damage2);
	ostringstream oss2;
	oss2 << damage2;
	td->setAction(e->getEnemy()->getType()+" deals "+oss2.str()+" damage to PC. ");
	//goblin steal gold
	if(special&&e->getSym()=='N'&&Player::getPlayer()->getGold()>0) {
		Player::getPlayer()->lossGold(1);
		e->getEnemy()->addGold(1);
		td->setAction("N steals one gold. ");
	}
	if(Player::getPlayer()->getHp()<=0) over = 1;
}

void Floor::transdir(char a, char b, string &dir) {
	if ((a=='w'&&b=='a')||(b=='w'&&a=='a')) dir = "nw";
	else if ((a=='w'&&b=='d')||(b=='w'&&a=='d')) dir = "ne";
	else if ((a=='s'&&b=='a')||(b=='s'&&a=='a')) dir = "sw";
	else if ((a=='s'&&b=='d')||(b=='s'&&a=='d')) dir = "se";
	else {
		if(a=='w') dir = "no";
		else if(a=='a') dir = "we";
		else if(a=='d') dir = "ea";
		else if(a=='s') dir = "so";
		else dir = "E";
	}
}

void Floor::print() {td->print();}

ostream &operator<<(ostream &out, const Floor &f) {
	out << (*f.td);
	return out;
}
