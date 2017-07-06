#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
class TextDisplay {
	std::string  **theDisplay;          //the n x n display 
	int Level;
	int score;
public:
	TextDisplay(); //one arg constructor where the parameter is the gridSize
	void notify(int r, int c, char lock, char type, char state);  
	void notifyscore(int s);
	void notifyLevel(int l);
	~TextDisplay(); //dtor

	friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
