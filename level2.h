#ifndef __LEVEL2_H__
#define __LEVEL2_H__
#include "board.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

class Level2 : public Board{
	int index;
	std::string store;
	int length;
	std::string filename;
	int start_score;
	bool read;
	bool hasFile;

	public:
	Level2(std::string filename, int start_score, bool hasText);
	void Generator(int r, int c);
	void init();
	void RandLocker();
	void levelupCheck();
};

#endif
