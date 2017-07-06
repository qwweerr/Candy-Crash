#ifndef __LEVEL1_H__
#define __LEVEL1_H__
#include "board.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

class Level1 : public Board{
	int index;
	std::string store;
	int length;
	std::string filename;
	int start_score;
	bool read;
	bool hasFile;

	public:
	Level1(std::string filename, int start_score, bool hasText);
	void Generator(int r, int c);
	void init();
	void levelupCheck();
};

#endif
