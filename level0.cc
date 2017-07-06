#include "level0.h"
#include "board.h"

using namespace std;

Level0::Level0(string filename, int start_score, bool hasText){
	index = 0;
	this->filename = filename;
	ifstream input(filename.c_str());
	for(int i=0;i<10;i++){
		getline(input,store);
	}
	int checker;
	if(input>>checker){
		ifstream input(filename.c_str());
		for(int i=0;i<11;i++){
			getline(input,store);
		}
		read = true;
		length = store.length();
	}else{
		read=false;
	}
	
	this->start_score = start_score;
	NoWindow = hasText;
}

void Level0::Generator(int r, int c){
	if(!read){
		ifstream input_seq("sequence.txt");
		for(int i=0;i<11;i++){
			getline(input_seq,store);
		}
		length = store.length();
	}
	theBoard[r][c].setType('_');
	char state_from_seq;
	state_from_seq = store[index];
	index++;
	if(index==length){index=0;}
	theBoard[r][c].setState(state_from_seq);
}

void Level0::init(){
	theBoard = new Cell*[10];
	td = new TextDisplay(); // waiting for exact signature
	if(NoWindow){
		theWindow = 0;
	}else {
		theWindow = new Xwindow;
	}
	int x, y;
	int w = 50;
	int h = 50;
	score = start_score;
	td->notifyLevel(0);
	td->notifyscore(start_score);
	for(int i=0; i<10; i++){
		theBoard[i] = new Cell[10];
	}
	ifstream input(filename.c_str());
	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			x = j * h;
			y = i * w;
			theBoard[i][j].setCoords(i,j,x,y,w,h,theWindow);
			string s;
			input>>s;
			theBoard[i][j].setCell(s[0],s[1],s[2]);
	
			td->notify(i,j,theBoard[i][j].getLock(),theBoard[i][j].getType(),theBoard[i][j].getState());
		
		}
	}
	if(!NoWindow){
		drawBoard();
	}
}

void Level0::levelupCheck(){
	if(score - start_score>=200){
		levelInc = true;
	}
}


			
