#ifndef __CELL_H_
#define __CELL_H_
#include <iostream>
#include "window.h"


class Cell{
	char lock;
	char type;
	char state;
	int r;
	int c;
	int x;
	int y;
	int width;
	int height;
   	bool checking;
	Xwindow *window;
	public:
	Cell();
	int getr();
	int getc();
	char getState();
	char getType();
	char getLock();
        bool getchecking();
	void setchecking();
	void draw();
	void printString();
	void printBigString();
	void setState(char state);
	void setType(char type);
	void setLock(char lock);
	void setCell(char lock,char type,char change);
	void setCoords(int r,int c,int x,int y,int width, int height, Xwindow *w);
	bool IsHole();
	void MakeHole();
	void LocklessHole();
	void unstablesquare();
	void uprightsquare();
	void lateralsquare();
	void psychedelicsquare();
};

#endif

