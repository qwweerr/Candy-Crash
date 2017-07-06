#include "level1.h"
#include "board.h"

using namespace std;

Level1::Level1(string filename, int start_score, bool hasText){
	if(filename != "No Scriptfile"){
		hasFile = true;
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
                    	read =true;
			length = store.length();
		}else{
                    	read =false;
                }
	} else {
		hasFile = false;
		read = false;
	}
	
	this->start_score = start_score;
	NoWindow = hasText;
}

void Level1::Generator(int r, int c){
	if(!read){
		int randstate = rand() % 10 + 1;
		if(randstate==1 || randstate==2){
			theBoard[r][c].setState('0');
		}else if(randstate==3 || randstate==4){
			theBoard[r][c].setState('1');
		}else if(randstate==5 || randstate==6){
			theBoard[r][c].setState('2');
		}else if(randstate==7 || randstate==8){
			theBoard[r][c].setState('3');
		}else if(randstate==9){
			theBoard[r][c].setState('4');
		}else if(randstate==10){
			theBoard[r][c].setState('5');
		}

		theBoard[r][c].setType('_');
		int randxxx = rand() % 6;
		if(randxxx==1){
			int randtype = rand() % 4;
			if(randtype==0){
				theBoard[r][c].setType('h');
			}else if(randtype==1){
				theBoard[r][c].setType ('v');
			}else if(randtype==2){
				theBoard[r][c].setType('b');
			}else {
				theBoard[r][c].setType('p');
			}
		}
	}else {
		theBoard[r][c].setType('_');
		char state_from_seq;
		state_from_seq = store[index];
		index++;
		if(index==length){index=0;}
		theBoard[r][c].setState(state_from_seq);		
	}
}

void Level1::init(){
	theBoard = new Cell*[10];
	td = new TextDisplay(); // waiting for exact signature
	theWindow = NoWindow ? NULL : new Xwindow;
	int x, y;
	int w = 50;
	int h = 50;
	score = start_score;
	td->notifyLevel(1);
	td->notifyscore(start_score);
	for(int i=0; i<10; i++){
		theBoard[i] = new Cell[10];
	}
        if(hasFile){
	  	ifstream input(filename.c_str());
	  	for(int i=0; i<10; i++){
			for(int j=0; j<10; j++){
				x = j*h;
				y = i*w;
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
        }else{
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
                                x = j*h;
                                y = i*w;
                                theBoard[i][j].setCoords(i,j,x,y,w,h,theWindow);
	
				Generator(i,j);

			}
		}
		
		checkmatch_level1_wrapper();
	}
      
}

void Level1::levelupCheck(){
	if(score - start_score>=300){
		levelInc = true;
	}
}
