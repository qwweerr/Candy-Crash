#ifndef __LEVEL0_H__
#define __LEVEL0_H__
#include "board.h"
#include <sstream>
#include <fstream>

class Level0 : public Board{
	int index;
	std::string store;
	int length;
	std::string filename;
	int start_score;
	bool read;

	public:
	Level0(std::string filename, int start_score, bool hasText);
	void Generator(int r, int c);
	void init();
	void levelupCheck();
};

#endif

