#include "level2.h"
#include "board.h"

using namespace std;

Level2::Level2(string filename, int start_score, bool hasText){
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

void Level2::Generator(int r, int c){
	if(!read){
		int randstate = rand() % 6;
		theBoard[r][c].setState('0' + randstate);
		theBoard[r][c].setType('_');

	}else {
		theBoard[r][c].setType('_');
		char state_from_seq;
		state_from_seq = store[index];
		index++;
		if(index==length){index=0;}
		theBoard[r][c].setState(state_from_seq);		
	}
}

void Level2::init(){
	theBoard = new Cell*[10];
	td = new TextDisplay(); 
	theWindow = NoWindow ? NULL : new Xwindow;
	int x, y;
	int w = 50;
	int h = 50;
	score = start_score;
	td->notifyLevel(2);
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
		checkmatch_level2_wrapper();
		RandLocker();
	}

}

void Level2::RandLocker(){
	int numlocked;
	int RowArr[20];
	int ColArr[20];
	int index =0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			if(theBoard[i][j].getLock() =='l'){
				numlocked++;
				RowArr[index]=i;
				ColArr[index]=j;
				index++;
			}
		}
	}
	for(int i=index; i<20; i++){
		RowArr[i]=-1;
	}
	for(int i=index; i<20; i++){
		ColArr[i]=-1;
	}
	while(RowArr[19]==-1 || ColArr[19]==-1){
		bool TrueFalse = true;
		int row = rand() % 10;
		int col = rand() % 10;
		for(int i=0;i<=index;i++){
			if(RowArr[i]==row && ColArr[i]==col){
				TrueFalse = false;
				break;
			}
		}
		if(TrueFalse){
			RowArr[index] = row;
			ColArr[index] = col;
			index++;
		}
	}
	for(int i=0;i<=19;i++){
		int r = RowArr[i];
		int c = ColArr[i];
		theBoard[r][c].setLock( 'l');
		td->notify(r,c,theBoard[r][c].getLock(),theBoard[r][c].getType(),theBoard[r][c].getState());
	}
	if(!NoWindow){
		drawBoard();
	}
}
void Level2::levelupCheck(){
	if(score-start_score>=500){
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				if(theBoard[i][j].getLock()=='l'){
					return;
				}
			}
		}
		levelInc= true;
	}
}
