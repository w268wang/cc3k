#ifndef _CELL_H_
#define _CELL_H_

#include <string>
#include "item.h"
#include "player.h"
#include "enemy.h"
#include "textdisplay.h"

const int maxNeighbours = 8;

class Cell {
	protected:	
	std::string type; // wall1("|"), wall2("-"), passenger1("+"), passenger2("#"), doorway("\"), tile(".")
	const std::string name;
	char sym; // @ + # | - \ . 
	int numNeighbours;
	Cell **neighbours;
	int r, c;
	Cell(int r, int c, std::string type, std::string name);
	
	public:
	std::string getType() const;
	char getSym() const;
	void getPos(int *a, int *b);
	void addNeighbour(int po, Cell *neighbour);
	Cell **getNeighbours() const;
	
	virtual int getRoom() const;
	virtual void notifyDisplay(TextDisplay &t);
	virtual bool available() const;
	virtual void setDoorway();
	virtual char getContain() const;
	virtual void pushItem(Item *i);
	virtual Item *getItem() const;
	virtual Item *popItem();
	virtual void pushPlayer(Player *p);
	virtual Player *getPlayer() const;
	virtual Player *popPlayer();
	virtual void pushEnemy(Enemy *e);
	virtual Enemy *getEnemy() const;
	virtual Enemy *popEnemy();
	virtual ~Cell()=0;
};

class ECell : public Cell { // this is an empty cell which is nothing, wall or doorway
	public:
	ECell(int r, int c, std::string type, std::string name);
	~ECell();

	void notifyDisplay(TextDisplay &t);
};

class Passenger : public Cell { // this 
	Player *p;
	public:
	Passenger(int r, int c, std::string type, std::string name);
	~Passenger();
	
	void notifyDisplay(TextDisplay &t);

	void pushPlayer(Player *p);
	Player *getPlayer() const;
	Player *popPlayer();
};

class Tile : public Cell {
	const int room;
	Item *i;
	Player *p;
	Enemy *e;
	bool avail;
	char contain;
	public:
	Tile(int r, int c, int room);
	~Tile();

	void notifyDisplay(TextDisplay &t);

	bool available() const;
	void setDoorway();
	char getContain() const;
	int getRoom() const;
	void pushItem(Item *i);
	Item *getItem() const;
	Item *popItem();
	void pushPlayer(Player *p);
	Player *getPlayer() const;
	Player *popPlayer();
	void pushEnemy(Enemy *e);
	Enemy *getEnemy() const;
	Enemy *popEnemy();
};

#endif
