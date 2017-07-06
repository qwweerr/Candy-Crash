#ifndef __BOARD_H__
#define __BOARD_H__
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <math.h>
#include "cell.h"
#include "textdisplay.h"
#include "window.h"

class Board {
	protected:
	Cell **theBoard;                   // The actual n x n grid. 
	bool NomoreMatch;
	TextDisplay *td;                  // The text display.
	int score;
	int score1;
	bool levelInc;
	bool NoWindow;
	int chaincounter;// for couting the times of chain reaction
        int numsquares;
	int power;
	
	Xwindow * theWindow;

	void clearBoard();                 // Frees the theGrid field.

	// Add private members, if necessary.


	public:
	Board();
	~Board();  
	
     	bool getlevelInc();
        void setlevelInc();
	void printBoard();
	virtual void init();                 // Sets up an n x n grid.  Clears old grid, if necessary.
	void swap(int r, int c, int dir); 
	void fall();
        int getScore();
	bool ContainHole();
	virtual void Generator(int r, int c);
	void checkmatch(int r, int c, int dir);
	void checkmatch_level();
	void drawBoard();
	void checkmatch_level1_wrapper();
	void checkmatch_level2_wrapper();
	void check(Cell& cell);
	virtual void RandLocker();
	void hint();
	bool checkNomoreMatch();
	void lateral(Cell& cell);
	void upright(Cell& cell);
	void unstable(Cell& cell, int num);
	void psychedelic(Cell& cell);
	void activate(Cell& cell,int num);	
	friend std::ostream &operator<<(std::ostream &out, const Board &g);
	void scramble();
	virtual void levelupCheck();
	void levelup();
	void leveldown();
	bool getNomoreMatch();
};

#endif
