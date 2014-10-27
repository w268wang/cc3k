#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ncurses.h>
#include "floor.h"	
#include "character.h"
#include "player.h"
using namespace std;

void initCurses() {
	initscr();
	noecho();
	nonl();
	cbreak();
	if(has_colors()) {
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_YELLOW, COLOR_BLACK);
		init_pair(3, COLOR_MAGENTA, COLOR_CYAN);
		init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
		init_pair(5, COLOR_MAGENTA, COLOR_WHITE);
		init_pair(6, COLOR_MAGENTA, COLOR_YELLOW);
	}
	timeout(-1);
}

int main(int argc,char *argv[]) {
	bool canBuy = false;
	bool canRecover = false;
	bool canStorePotion = false;
	bool special = false;
	bool useCurses = false;
	for(int i=1; i<argc; i++) {
		string tempstr = argv[i];
		if(tempstr=="-b") canBuy = true;
		else if(tempstr=="-r") canRecover = true;
		else if(tempstr=="-p") canStorePotion = true;
		else if(tempstr=="-s") special = true;
		else if(tempstr=="-C") useCurses = true;
	}
	bool endOfGame = false;
	if(useCurses) {
	initCurses();
	printw("Welcome to the world of CC3K.\n");
	while(!endOfGame) {
		int floor = 1;
		Floor *f = 0;
		Player *pc = 0;
		char s;
		bool SDGKR = false; //SDGKR
		clear();
		attron(COLOR_PAIR(1));
		printw("First, you need to specify the race you wish to be.\n");
		attron(COLOR_PAIR(2));
		printw("h: human (140 HP, 20 Atk, 20 Def, have a 50%% increase to their score.\n");
		printw("d: dwarf (100 HP, 20 Atk, 30 Def, gold is doubled in value\n");
		printw("e: elves (140 HP, 30 Atk, 10 Def, negative potions have positive effect\n");
		printw("o: orc (180 HP, 30 Atk, 25 Def, gold is worth half value\n");
		printw("Please choose(h, d, e or o):\n");
		attron(COLOR_PAIR(1));
		s=getch();
		string tmprace="";
		if(s=='h') {pc=new Human(); tmprace="human";}
		else if(s=='d') {pc=new Dwarf(); tmprace="dwarf";}
		else if(s=='e') {pc=new Elves(); tmprace="elves";}
		else if(s=='o') {pc=new Orc(); tmprace="orc";}
		else if(s=='p') {pc=new Prof(); tmprace="professor (just for fun)";}
		else if(s=='t') {pc=new Ta(); tmprace="ta (just for fun)";}
		else if(s=='q') {endOfGame=true; continue;}
		else if(s=='r') {continue;}
		else {printw("What are you doing!\n"); break;}
		attron(COLOR_PAIR(1));
		printw(("You just chose "+tmprace+" as your character.\n").c_str());
		printw("The following commands can be supplied to your command interpreter:\nw,a,s,d,wa,ws,wd,sd,sa: moves the player character one block in the appropriate cardinal direction\nu <direction>: uses the potion indicated by the direction\nf <direction>: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must be one block north of the @)\nr: restarts the game. All stats, inventory, and gold are reset. A new race should be selected.\nq: allows the player to admit defeat and exit the game.\n");
		if(canStorePotion) printw("DLC commands:\nt <direction>: stores the potion indicated by the direction (e.g. no, so, ea)\ne < 0<=int<=2 > use the potion in pos <int>.\n");
		printw("\nTap any key to continue...\n");
		s=getch();
		Player::setPlayer(pc);
		while(floor<6){
			bool dead = false;
			f = new Floor(floor, canBuy, canRecover, special, canStorePotion, useCurses);
			f->init();
			//no,so,ea,we,ne,nw,se,sw
			f->clearAction();
			f->print();
			while(true) {
				s=getch();
				string deathwords="As you bravely fight your way through the seemingly endless dungeon, it is clear that your health is not catching up. Suddenly an attack knocks you to the ground, but unfortunately you do not have the energy to get up again. 'This is the end for you.' The enemy laughed as it strikes you with the final blow. You've made it so far, but it was not enough. You lie lifelessly on the stone cold ground. Perhaps if given another chance you will be able to make it, but for now it is Game Over for you. The End.";
				if(s=='r') {break;}
				else if(s=='q') {endOfGame=true; break;}
				else if(s=='g') {Player::getPlayer()->returnGold(100);}
				else if(s=='p') {Player::getPlayer()->addHp(100);}
				else if(canStorePotion&&s=='t') {
					s=getch();
					timeout(100);
					char s2='E';
                			s2=getch();
					timeout(-1);
					f->storePotion(s, s2);
					f->endStep();
					//cout << (*f);
				} else if(canStorePotion&&s=='e') {
					int tmpint=0;
					string tmpstr0;
					tmpstr0 = getch();
					istringstream iss(tmpstr0);
					iss >> tmpint;
					if(tmpint<0||tmpint>2) {
						printw("invalid input!\n");
						continue;
					}
					f->usePotion(tmpint);
					f->endStep();
					//cout << (*f);
				} else if(s=='u') {
					s=getch();
					timeout(100);
                			char s2='E';
                			s2=getch();
					timeout(-1);
					f->havePotion(s, s2);
					f->endStep();
					//cout << (*f);
				} else if(s=='f') { //a
					s=getch();
					timeout(100);
                			char s2='E';
                			s2=getch();
					timeout(-1);
					f->attack(s, s2);
					f->endStep();
					if((!SDGKR)&&f->isOver()==1) {printw((deathwords+"\n").c_str()); dead=true; break;}
					//cout << (*f);
				} else {
					timeout(100);
                			char s2='E';
                			s2=getch();
					timeout(-1);
					f->pmove(s, s2);
					f->endStep();
					if((!SDGKR)&&f->isOver()==1) {printw((deathwords+"\n").c_str()); dead=true; break;}
					//cout << (*f);
					if(f->isOver()==2){break;}
				}
				printw("\ncommand?");
				f->clearAction();
			}
			if(s=='r'||s=='q') {break;}
			else if(dead) {
				printw("Do you want to:\n(r) play again\n(q) or leave\n");
				s=getch();
				while(s!='r'&&s!='q') {//&&s!="david"&&s!="lucy") {
					printw("Please choose again.\n");
					s=getch();
				}
				if(s=='r') break;
				/*else if(s=="david"||s=="lucy") {SDGKR=true;
				printw("Super Duper Godizilla Kitties and Rainbows mode is activated!\n");}*/
				else {endOfGame=true; break;}
			}
			delete f;
			f=0;
			if(cin.eof()) {endOfGame=true; break;}
			Player::getPlayer()->clear();
			floor++;
			ostringstream oss;
			oss<<floor;
			if(floor!=6) printw(("Wow that was close, but you have made your way up the stairs without dying, now you are at floor "+oss.str()+".\n").c_str());
		}
		if(floor==6) {
			clear();
			printw("Unbelievable! You have just made it to the top of the tower, very impressive indeed. Congratulations on such an achievement! Wait what? What did you say? Why is there no final boss waiting for you at the end of the tower? Well, we figured that to simple make it through the dungeon without using any type of *cough* extra help *cough* is quite impossible, and to put a boss at the end would be brutal, that's why we will stop here, and let you celebrate your well deserved victory! Of course you can always try to escape this dungeon again with a different race or try to obtain more gold...but that's your choice. But for now, enjoy the fresh air and the comforting wind at your face, the feeling of freedom sure is good, isn't it?\n");
			int score = Player::getPlayer()->getGold();
			if(Player::getPlayer()->getRace()=="Human") {
				int ex = (score%2==0)? 0 : 1;
				score=(score/2)+ex+score;
			}
			ostringstream oss;
			oss<<score;
			printw(("Your score is "+oss.str()+". Good job! Thank you for playing!\n").c_str());
			printw("\nDo you want to restart the game? (y/n)\n");
			s=getch();
			if(s!='y') endOfGame = true;
		}
		delete f;
	}
	clear();
	printw("\n\n© DavidandLucy 2014 All Rights Reserved.\n");
	printw("\nTap any key to continue...\n");
	char s=getch();
	endwin();
	//################################################################################################
	} else {
	cout<<"'Welcome to the world of CC3K.' That is the phrase you heard when you woke in an unfamiliar dungeon. This seems to be a tower, god knows how tall it is, maybe 5? You don't know that. But what you do know is that this dungeon is filled with monsters who will try to kill you when you pass by them. Sounds terrible doesn't it? But there's a good part to it, these monsters drop GOLD!! Yes, as a university student you are obviously poor and wants all the money/gold you can get, so as you make your way through the dungeon, maybe you can kill a few of these monsters to obtain some extra cash! With that in mind you got up to your feet. "<<endl;
	while(!endOfGame) {
		int floor = 1;
		Floor *f = 0;
		Player *pc = 0;
		string s;
		bool SDGKR = false; //SDGKR
		cout<<"But before you start your adventure, since this is a game, you need to specify the race you wish to be."<<endl;
		cout<<"h: human (140 HP, 20 Atk, 20 Def, have a 50% increase to their score.)"<<endl;
		cout<<"d: dwarf (100 HP, 20 Atk, 30 Def, gold is doubled in value)"<<endl;
		cout<<"e: elves (140 HP, 30 Atk, 10 Def, negative potions have positive effect)"<<endl;
		cout<<"o: orc (180 HP, 30 Atk, 25 Def, gold is worth half value)"<<endl;
		cout<<"Please choose(h, d, e or o):"<<endl;
		cin>>s;
		string tmprace="";
		if(s=="h") {pc=new Human(); tmprace="human";}
		else if(s=="d") {pc=new Dwarf(); tmprace="dwarf";}
		else if(s=="e") {pc=new Elves(); tmprace="elves";}
		else if(s=="o") {pc=new Orc(); tmprace="orc";}
		else if(s=="p") {pc=new Prof(); tmprace="professor";}
		else if(s=="t") {pc=new Ta(); tmprace="ta";}
		else if(s=="q") {endOfGame=true; continue;}
		else if(s=="r") {continue;}
		else {cout<<"What are you doing! Don't do stuff stupid, we'll catch it ; )"<<endl; break;}
		cout<<"You have chosen to be a(n) "<<tmprace<<", wise choice. Your icon on the map is @"<<endl;
		cout<<"The following commands can be supplied to your command interpreter:\n"<<
"no,so,ea,we,ne,nw,se,sw: you walk one block in the appropriate cardinal direction\n"<<
"u <direction>: you use the potion indicated by the direction (e.g. no, so, ea)\n"<<
"a <direction>: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must be one block north of the @)\n"<<
"r: restarts the game. All stats, inventory, and gold are reset. A new race should be selected and you will have to hear the same boring story again blah blah blah.\n"<<
"q: allows the player to commit suicide and hence exit the game, now why would you do that..."<<endl;
		if(canStorePotion) cout<<"DLC commands:\ns <direction>: stores the potion indicated by the direction (e.g. no, so, ea)\n"<<
"us < 0<=int<=2 > use the potion in pos <int>."<<endl;
		Player::setPlayer(pc);
		while(floor<6){
			bool dead = false;
			f = new Floor(floor, canBuy, canRecover, special, canStorePotion, useCurses);
			f->init();
			cout << (*f);
			//no,so,ea,we,ne,nw,se,sw
			f->clearAction();
			cout << "Command: "<<endl;
			while(cin>>s) {
				string deathwords="As you bravely fight your way through the seemingly endless dungeon, it is clear that your health is not catching up. Suddenly an attack knocks you to the ground, but unfortunately you do not have the energy to get up again. 'This is the end for you.' The enemy laughed as it strikes you with the final blow. You've made it so far, but it was not enough. You lie lifelessly on the stone cold ground. Perhaps if given another chance you will be able to make it, but for now it is Game Over for you. The End.";
				if(s=="r") {break;}
				else if(s=="q") {endOfGame=true; break;}
				else if(s=="greenisgood") {int tmpgold=0; cin>>tmpgold; Player::getPlayer()->returnGold(tmpgold);}
				else if(s=="over9000") {Player::getPlayer()->returnGold(9001);}
				else if(s=="iamprof") {Player::getPlayer()->addHp(100);}
				else if(canStorePotion&&s=="s") {
					cin>>s;
					f->storePotion(s);
					f->endStep();
					cout << (*f);
				} else if(canStorePotion&&s=="us") {
					int tmpint=0;
					if((!(cin>>tmpint))||tmpint<0||tmpint>2) {
						cout<<"invalid input!"<<endl;
						continue;
					}
					f->usePotion(tmpint);
					f->endStep();
					cout << (*f);
				} else if(s=="u") {
					cin>>s;
					f->havePotion(s);
					f->endStep();
					cout << (*f);
				} else if(s=="a") {
					cin>>s;
					f->attack(s);
					f->endStep();
					if((!SDGKR)&&f->isOver()==1) {cout<<deathwords<<endl; dead=true; break;}
					cout << (*f);
				} else {
					f->pmove(s);
					f->endStep();
					if((!SDGKR)&&f->isOver()==1) {cout<<deathwords<<endl; dead=true; break;}
					cout << (*f);
					if(f->isOver()==2){break;}
				}
				f->clearAction();
				cout << "Command: "<<endl;
			}
			if(s=="r"||s=="q") {break;}
			else if(dead) {
				cout<<"Do you want to:\n(r) play again\n(q) or leave"<<endl;
				cin >> s;
				while(s!="r"&&s!="q"&&s!="david"&&s!="lucy") {
					cout<<"Please choose again."<<endl;
					cin >> s;
					if(cin.eof()) break;
				}
				if(s=="r") break;
				else if(s=="david"||s=="lucy") {SDGKR=true;
				cout << "Super Duper Godizilla Kitties and Rainbows mode is activated!"<<endl;}
				else {endOfGame=true; break;}
			}
			delete f;
			f=0;
			if(cin.eof()) {endOfGame=true; break;}
			Player::getPlayer()->clear();
			floor++;
			if(floor!=6) cout<<"You have reached the floor "<<floor<<"."<<endl;
		}
		if(floor==6) {
			cout<<"Unbelievable! You have just made it to the top of the tower, very impressive indeed. Congratulations on such an achievement! Wait what? What did you say? Why is there no final boss waiting for you at the end of the tower? Well, we figured that to simple make it through the dungeon without using any type of *cough* extra help *cough* is quite impossible, and to put a boss at the end would be brutal, that's why we will stop here, and let you celebrate your well deserved victory! Of course you can always try to escape this dungeon again with a different race or try to obtain more gold...but that's your choice. But for now, enjoy the fresh air and the comforting wind at your face, the feeling of freedom sure is good, isn't it?"<<endl;
			int score = Player::getPlayer()->getGold();
			if(Player::getPlayer()->getRace()=="Human") {
				int ex = (score%2==0)? 0 : 1;
				score=(score/2)+ex+score;
			}
			cout<<"Your score is "<<score<<". Good job! Thank you for playing!"<<endl;
			cout<<"Do you want to restart the game? (y/n)"<<endl;
			cin>>s;
			if(s!="y") endOfGame = true;
		}
		cout<<"\n\n© DavidandLucy 2014 All Rights Reserved."<<endl;
		delete f;
	}
	}
}

