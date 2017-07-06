#include "board.h"
using namespace std;

Board::Board(){
	theBoard = 0;
	td = 0;
	theWindow=0;
	score = 0;
	levelInc = false;
	NoWindow = false;
	NomoreMatch = 0;
	chaincounter=-1;
	numsquares=0;
	score1=0;
	power = 0;
}
Board::~Board(){
	clearBoard();
}
bool Board::getNomoreMatch(){
	return NomoreMatch;
}

int Board::getScore(){
        return score;
}
bool Board::getlevelInc(){
   	return levelInc;
}
void Board::setlevelInc(){
	levelInc=false;
}


void Board::swap(int r, int c, int dir){
	chaincounter=-1;
        numsquares=0;
	score1=0;
	power = 0;
	int row = r;
	int col = c;
	if(dir==0){row--;}
	else if(dir==1){row++;}
	else if(dir==2){col--;}
	else if(dir==3){col++;}
	else {cout << "Invalid Swap!" << endl;}
	if(row<0 || row>9 || col<0 || col>9){
		 cout << "Invalid Swap!" << endl;
		 return;
	}

	if(theBoard[row][col].IsHole() || theBoard[r][c].IsHole()){
		theBoard[row][col].setType(theBoard[r][c].getType());
		theBoard[row][col].setState(theBoard[r][c].getState());
		theBoard[r][c].LocklessHole();
		td->notify(r,c,theBoard[r][c].getLock(),theBoard[r][c].getType(),theBoard[r][c].getState());


		td->notify(row,col,theBoard[row][col].getLock(),theBoard[row][col].getType(),theBoard[row][col].getState());

	}else{
		// make tmp fields in stack and then swap
		char tmp_type = theBoard[r][c].getType();
		char tmp_state = theBoard[r][c].getState();

		//theBoard[r][c] => theBoard[row][col]
		theBoard[r][c].setType(theBoard[row][col].getType());
		theBoard[r][c].setState(theBoard[row][col].getState());

		//theBoard[row][col] => theBoard[r][c]
		theBoard[row][col].setType( tmp_type);
		theBoard[row][col].setState( tmp_state);

		td->notify(r,c,theBoard[r][c].getLock(),theBoard[r][c].getType(),theBoard[r][c].getState());

		td->notify(row,col,theBoard[row][col].getLock(),theBoard[row][col].getType(),theBoard[row][col].getState());

	}
	checkmatch(r,c,dir);
}

void Board::fall(){
	numsquares=0;
	score1=0;
	bool Hole = 0;
	int r, c;
	for(int i=9; i>=0; i--){
		for(int j=0; j<10; j++){
			Hole = theBoard[i][j].IsHole();
			if(Hole){
				r = i;
				c = j; 
				goto outside;
			}
		}
	}
outside:
	if(Hole){
		if(theBoard[r][c].IsHole()){
			for(int i=r-1; i>=0;i--){
				if(!theBoard[i][c].IsHole()){
					theBoard[r][c].setType(theBoard[i][c].getType());
					theBoard[r][c].setState(theBoard[i][c].getState());
					theBoard[i][c].LocklessHole();
					td->notify(r,c,theBoard[r][c].getLock(),theBoard[r][c].getType(),theBoard[r][c].getState());


					td->notify(i,c,theBoard[i][c].getLock(),theBoard[i][c].getType(),theBoard[i][c].getState());

					break;
				}
			}
			if(theBoard[r][c].IsHole()){
				Generator(r,c);
				td->notify(r,c,theBoard[r][c].getLock(),theBoard[r][c].getType(),theBoard[r][c].getState());

			}
		}
		fall();
	}else {
		return;
	}
}

bool Board::ContainHole(){
	for(int i=0; i<10; i++){
		for(int j=0; j<10; j++){
			if(theBoard[i][j].IsHole()){
				return true;
			}
		}
	}
	return false;
}

void Board::checkmatch(int r,int c, int dir){
	
	if(dir==0){
		check(theBoard[r-1][c]);
	}else if(dir==1){
		check(theBoard[r+1][c]);
	}else if(dir==2){
		check(theBoard[r][c-1]);
	}else if(dir==3){
		check(theBoard[r][c+1]);
	}

	check(theBoard[r][c]);
	
	for(int p=9;p>=0;p--){
		for(int q=0;q<10;q++){
			check(theBoard[p][q]);
		}
	}
	if(ContainHole()){
		chaincounter++;

                for(int i=0; i<10; i++){
			for(int j=0; j<10; j++){
      			       if(theBoard[i][j].IsHole()){
                              		numsquares++;
             		  	}
				else if(theBoard[i][j].getchecking()){
					numsquares++;
 					theBoard[i][j].setchecking();
				}
			}
 		}
                if(numsquares > 5){
   			score1 =4 * numsquares;
     		}if(numsquares == 5){
   			score1 =15;
   		}if(numsquares== 4){
  			score1=8;
 		}if(numsquares == 3){
 			score1=3;
     		}
 		if(chaincounter > 0){
            	  	power=pow(2,chaincounter);
  			score += score1*power;
 		}else{
 			score += score1;
		}
		fall();
		checkmatch(9,0,3);


	}else{
		td->notifyscore(score);	
		levelupCheck();
		chaincounter=-1;
		power = 0;
		for(int a=0; a<10; a++){
			for(int b=0; b<10; b++){
				td->notify(a,b,theBoard[a][b].getLock(),theBoard[a][b].getType(),theBoard[a][b].getState());
			}
		}
		return;
	}
}


void Board::checkmatch_level(){
        for(int p=9;p>=0;p--){
                for(int q=0;q<10;q++){
                        check(theBoard[p][q]);
                }
        }
        if(ContainHole()){
                fall();
                checkmatch_level();
                while(ContainHole()){
                        checkmatch_level();
                        fall();
                }
        }else{
                for(int a=0; a<10; a++){
                        for(int b=0; b<10; b++){
                                td->notify(a,b,theBoard[a][b].getLock(),theBoard[a][b].getType(),theBoard[a][b].getState());
				theBoard[a][b].setchecking();

                        }
                }
                return;
        }
}

void Board::drawBoard(){
        for(int i=0;i<10;i++){
                for(int j=0;j<10;j++){
                        theBoard[i][j].draw();
                        if(theBoard[i][j].getLock()=='l') {
                                theBoard[i][j].printBigString();
                        }
                        if(theBoard[i][j].getType()!='_'){
                                theBoard[i][j].printString();
                        }
                }
        }	
}

void Board::checkmatch_level1_wrapper(){
	checkmatch_level();
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			td->notify(i,j,theBoard[i][j].getLock(),theBoard[i][j].getType(),theBoard[i][j].getState());
		}
	}
	if(!NoWindow){
		drawBoard();
	}
}


void Board::checkmatch_level2_wrapper(){
	checkmatch_level();
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			theBoard[i][j].setType('_');
			td->notify(i,j,theBoard[i][j].getLock(),theBoard[i][j].getType(),theBoard[i][j].getState());
		}
	}
	if(!NoWindow){
		drawBoard();
	}
}

void Board::printBoard(){
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			cout<<theBoard[i][j].getLock()<<theBoard[i][j].getType()<<theBoard[i][j].getState()<<" ";
		}
		cout << endl;
	}
	cout << endl;
	
}


ostream &operator<<(ostream &out, const Board &b){
	out<<*(b.td);
	return out;
}

void Board::clearBoard(){
	for(int i=0; i<10; i++){
		delete [] theBoard[i];
	}
	delete [] theBoard;
	delete td;
	delete theWindow;
}

void Board::lateral(Cell &cell){
	for(int i=0; i<= 9;i++){
		if(i==cell.getc()){
			cell.MakeHole();
		}
		activate(theBoard[cell.getr()][i],3);
		theBoard[cell.getr()][i].MakeHole();
	}
}

void Board::upright(Cell &cell){
	for(int i=0; i<=9; i++){
		if(i==cell.getr()){
			cell.MakeHole();
		}
		activate(theBoard[i][cell.getc()],3);
		theBoard[i][cell.getc()].MakeHole();
	}
}



void Board::unstable(Cell &cell,int num){
	if(num == 3){
		for(int i=cell.getr()-1;i<=cell.getr()+1;++i){
			for(int j=cell.getc()-1;j<=cell.getc()+1; ++j){
				if((i>=0) &&  (i<= 9) && (j>=0) && (j<=9)){
					if(i==cell.getr() && j==cell.getc()){ 
						cell.MakeHole();
					}
					activate(theBoard[i][j],3);
					theBoard[i][j].MakeHole();
				}  
			}
		}
	}if(num == 4){
		for(int i=cell.getr()-2;i<=cell.getr()+2;++i){
			for(int j=cell.getc()-2;j<=cell.getc()+2; ++j){
				if((i>=0) &&  (i<= 9) && (j>=0) && (j<=9)){
					if(i==cell.getr() && j==cell.getc()){
						cell.MakeHole();
					}
					activate(theBoard[i][j],3);	
					theBoard[i][j].MakeHole();
				}
			}
		}
	}
}

void Board::psychedelic(Cell &cell){
	char state_psychedelic = cell.getState();
	if(state_psychedelic=='x'){return;}
	for(int i=0;i<=9;i++){
		for(int j=0; j<=9; j++){
			if(i==cell.getr() && j==cell.getc()){
				cell.MakeHole();
			}else if(theBoard[i][j].getState() == state_psychedelic){
				activate(theBoard[i][j],3);	
				theBoard[i][j].MakeHole();
			}
		}
	}
}

void Board::activate(Cell & cell,int num){
	if(cell.getType() == 'h'){
		lateral(cell);
	} else if(cell.getType() == 'v'){
		upright(cell);
	}else if (cell.getType() == 'b'){
		unstable(cell,num);
	}else if (cell.getType() == 'p'){
		psychedelic(cell);
	}else{}  
}

void Board::check(Cell &cell){
	if(cell.IsHole()){
		return;
	}
	int up=0;
	int right=0;
	int down=0;
	int left=0;
	char uparr[2]={'0'};
	char downarr[2]={'0'};
	char rightarr[2] = {'0'};
	char leftarr[2] = {'0'};
	char central=cell.getType();
	for (int i=cell.getr()-1;i>=0;i--){
		if(cell.getState() == theBoard[i][cell.getc()].getState()){
			up++;
			if(cell.getr()-1-i<=1){
				uparr[cell.getr()-1-i]=theBoard[i][cell.getc()].getType();
			}
		}else{
			break;
		}
	}
	for (int i=cell.getr()+1;i<=9;i++){
		if(cell.getState() == theBoard[i][cell.getc()].getState()){
			down++;
			if(i-cell.getr()-1 <=1){
				downarr[i-cell.getr()-1]=theBoard[i][cell.getc()].getType();
			}
		}else{
			break;
		}
	}
	for (int i=cell.getc()-1;i>=0;i--){
		if(cell.getState() == theBoard[cell.getr()][i].getState()){
			left++;
			if(cell.getc()-1-i <=1){
				leftarr[cell.getc()-1-i]=theBoard[cell.getr()][i].getType();
			} 
		} else{
			break;
		}
	}
	for (int i=cell.getc()+1;i<=9;i++){
		if(cell.getState() == theBoard[cell.getr()][i].getState()){
			right++;
			if(i-cell.getc()-1 <=1){
				rightarr[i-cell.getc()-1]=theBoard[cell.getr()][i].getType();
			}
		}else{
			break;
		}
	}
	if(up+down==4 && up == 2 && down == 2){

		for(int i=cell.getr()-1;i>=0;--i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getr()+1;i<=9;++i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		cell.psychedelicsquare();
		if(uparr[0] == 'h'){
			lateral(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'h'){
			lateral(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'v'){
			upright(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'v'){
			upright(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'b'){
			unstable(theBoard[cell.getr()-1][cell.getc()],4);
		}
		if(uparr[1] == 'b'){
			unstable(theBoard[cell.getr()-2][cell.getc()],4);
		}
		if(uparr[0] == 'p'){
			psychedelic(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'p'){
			psychedelic(theBoard[cell.getr()-2][cell.getc()]);
		} 
		if(downarr[0] == 'h'){
			lateral(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'h'){
			lateral(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'v'){
			upright(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'v'){
			upright(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'b'){
			unstable(theBoard[cell.getr()+1][cell.getc()],4);
		}
		if(downarr[1] == 'b'){
			unstable(theBoard[cell.getr()+2][cell.getc()],4);
		} 
		if(downarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()+2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if (left == 2 && right == 2){
                for(int i=cell.getc()-1; i >= 0;--i){
          		if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()+1;i<=9;++i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.psychedelicsquare();
		if(leftarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-1]);
		} 
		if(leftarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-1],4);
		}
		if(leftarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-2],4);
		}
		if(leftarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(rightarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+1],4);
		}
		if(rightarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+2],4);
		}
		if(rightarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+2]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if (up+down ==3 && ((up == 1 && down == 2 ) || (up == 2 && down == 1))){
		for(int i=cell.getr()-1;i>=0;--i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getr()+1;i<=9;++i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		cell.uprightsquare();
		if(uparr[0] == 'h'){
			lateral(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'h'){
			lateral(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'v'){
			upright(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'v'){
			upright(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'b'){
			unstable(theBoard[cell.getr()-1][cell.getc()],4);
		}
		if(uparr[1] == 'b'){
			unstable(theBoard[cell.getr()-2][cell.getc()],4);
		}
		if(uparr[0] == 'p'){
			psychedelic(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'p'){
			psychedelic(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(downarr[0] == 'h'){
			lateral(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'h'){
			lateral(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'v'){
			upright(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'v'){
			upright(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'b'){
			unstable(theBoard[cell.getr()+1][cell.getc()],4);
		}
		if(downarr[1] == 'b'){
			unstable(theBoard[cell.getr()+2][cell.getc()],4);
		}
		if(downarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()+2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	} 
	if(left+right ==3 && ((left == 1 && right == 2)|| (left == 2 && right == 1))){
		for (int i=cell.getc()-1;i>=0;--i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()+1;i<=9;++i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.lateralsquare(); 
		if(leftarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-1],4);
		}
		if(leftarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-2],4);
		}
		if(leftarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(rightarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+1],4);
		}
		if(rightarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+2],4);
		}
		if(rightarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+2]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if(up+right == 4 && up == 2 && right == 2){
		for(int i=cell.getr()-1;i>=0;--i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}for (int i=cell.getc()+1;i<=9;++i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.unstablesquare();
		if(rightarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+1]);
		} 
		if(rightarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+1],4);
		}
		if(rightarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+2],4);
		}
		if(rightarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(uparr[0] == 'h'){
			lateral(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'h'){
			lateral(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'v'){
			upright(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'v'){
			upright(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'b'){
			unstable(theBoard[cell.getr()-1][cell.getc()],4);
		}
		if(uparr[1] == 'b'){
			unstable(theBoard[cell.getr()-2][cell.getc()],4);
		}
		if(uparr[0] == 'p'){
			psychedelic(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'p'){
			psychedelic(theBoard[cell.getr()-2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if(up +left == 4 && left == 2 && up == 2){
		for(int i=cell.getr()-1;i>=0;--i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()-1;i>=0;--i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.unstablesquare();
		if(leftarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-1],4);
		}
		if(leftarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-2],4);
		}
		if(leftarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(uparr[0] == 'h'){
			lateral(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'h'){
			lateral(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'v'){
			upright(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'v'){
			upright(theBoard[cell.getr()-2][cell.getc()]);
		}
		if(uparr[0] == 'b'){
			unstable(theBoard[cell.getr()-1][cell.getc()],4);
		}
		if(uparr[1] == 'b'){
			unstable(theBoard[cell.getr()-2][cell.getc()],4);
		}
		if(uparr[0] == 'p'){
			psychedelic(theBoard[cell.getr()-1][cell.getc()]);
		}
		if(uparr[1] == 'p'){
			psychedelic(theBoard[cell.getr()-2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if(right+down == 4 && right == 2 && down == 2){
		for (int i=cell.getr()+1;i<=9;++i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()+1;i<=9;++i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.unstablesquare();

		if(rightarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+1]);
		} 
		if(rightarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(rightarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+1],4);
		}
		if(rightarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+2],4);
		}
		if(rightarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+1]);
		}
		if(rightarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+2]);
		}
		if(downarr[0] == 'h'){
			lateral(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'h'){
			lateral(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'v'){
			upright(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'v'){
			upright(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'b'){
			unstable(theBoard[cell.getr()+1][cell.getc()],4);
		} 
		if(downarr[1] == 'b'){
			unstable(theBoard[cell.getr()+2][cell.getc()],4);
		} 
		if(downarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()+2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	} if(down+left == 4 && down == 2 && left == 2){
		for (int i=cell.getr()+1;i<=9;++i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()-1;i>=0;--i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		cell.unstablesquare();
		if(leftarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-2]);
		}
		if(leftarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-1],4);
		}
		if(leftarr[1] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-2],4);
		}
		if(leftarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-1]);
		}
		if(leftarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-2]);
		}   if(downarr[0] == 'h'){
			lateral(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'h'){
			lateral(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'v'){
			upright(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'v'){
			upright(theBoard[cell.getr()+2][cell.getc()]);
		}
		if(downarr[0] == 'b'){
			unstable(theBoard[cell.getr()+1][cell.getc()],4);
		} 
		if(downarr[1] == 'b'){
			unstable(theBoard[cell.getr()+2][cell.getc()],4);
		} 
		if(downarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()+1][cell.getc()]);
		}
		if(downarr[1] == 'p'){
			psychedelic(theBoard[cell.getr()+2][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],4);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
	}
	if(up + down == 2 && up == 1 && down == 1){
		for(int i=cell.getr()-1; i>=0; --i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getr()+1;i<=9;++i){
			if (cell.getState() == theBoard[i][cell.getc()].getState() && theBoard[i][cell.getc()].getType()!='p'){
				theBoard[i][cell.getc()].MakeHole();
			}
			else {break;}
		}
		
		if(uparr[0] == 'h'){
			lateral(theBoard[cell.getr()-1][cell.getc()]);
		} if(uparr[0] == 'v'){
			upright(theBoard[cell.getr()-1][cell.getc()]);
		} if(uparr[0] == 'b'){
			unstable(theBoard[cell.getr()-1][cell.getc()],3);
		}if(uparr[0] == 'p'){
			psychedelic(theBoard[cell.getr()-1][cell.getc()]);
		} if(downarr[0] == 'h'){
			lateral(theBoard[cell.getr()+1][cell.getc()]);
		} if(downarr[0] == 'v'){
			upright(theBoard[cell.getr()+1][cell.getc()]);
		}if(downarr[0] == 'b'){
			unstable(theBoard[cell.getr()+1][cell.getc()],3);
		} if(downarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()+1][cell.getc()]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],3);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
                cell.MakeHole();
	}
	if(left+right == 2 && left == 1 && right == 1){
		for(int i=cell.getc()-1;i>=0;--i){	
 			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
		for (int i=cell.getc()+1;i<=9;++i){
			if (cell.getState() == theBoard[cell.getr()][i].getState() && theBoard[cell.getr()][i].getType()!='p'){
				theBoard[cell.getr()][i].MakeHole();
			}
			else {break;}
		}
	
		if(leftarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()-1]);
		} if(leftarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()-1]);
		} if(leftarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()-1],3);
		} if(leftarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()-1]);
		} if(rightarr[0] == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()+1]);
		} if(rightarr[0] == 'v'){
			upright(theBoard[cell.getr()][cell.getc()+1]);
		} if(rightarr[0] == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()+1],3);
		}if(rightarr[0] == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()+1]);
		}if(central == 'p'){
			psychedelic(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'b'){
			unstable(theBoard[cell.getr()][cell.getc()],3);
		}if(central == 'v'){
			upright(theBoard[cell.getr()][cell.getc()]);
		}if(central == 'h'){
			lateral(theBoard[cell.getr()][cell.getc()]);
		}
                cell.MakeHole();
	}
}   


void Board::hint(){
	int x;
	int y;
	int hstate;
	for(int i=0; i<=9; ++i){
		for(int j=0; j<=8; ++j){
			if(theBoard[i][j].getState() == theBoard[i][j+1].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();

				if((y-2>=0) && (theBoard[x][y-2].getState() == hstate)){
					cout << x <<" " << y-2 << " " << 3<<endl;
					return;
				}else if ((y-1 >= 0) && ( x-1 >= 0) && (theBoard[x-1][y-1].getState() == hstate)){
					cout << x-1 <<" "<< y-1<<" " << 1<<endl;
					return;
				}else if ((y-1 >= 0) && (x+1 <= 9)&& (theBoard[x+1][y-1].getState() == hstate)){
					cout << x+1 <<" "<< y-1 <<" " << 0<<endl;
					return;
				}else if ((y+3<=9) && (theBoard[x][y+3].getState() == hstate)){
					cout << x <<" " << y+3 << " " << 2<<endl;
					return;  
				}else if ((x-1>= 0) && (y+2<= 9)&& (theBoard[x-1][y+2].getState() == hstate)){
					cout << x-1 << " "<< y+2 << " "<< 1<<endl;
					return;
				}else if ((x+1 <= 9) && (y+2 <= 9) && (theBoard[x+1][y+2].getState() == hstate)){
					cout << x+1 << " "<<y+2 <<" "<< 0<<endl;
					return;
				}
			}
		}
	}
	for(int j=0; j<=9; ++j){
		for(int i=0; i<=8; ++i){
			if(theBoard[i][j].getState() == theBoard[i+1][j].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
				if((x-2>=0) && (theBoard[x-2][y].getState() == hstate)){
					cout << x-2 << " "<< y << " "<< 1<<endl;
					return;
				}else if ((y-1 >= 0) && ( x-1 >= 0) && (theBoard[x-1][y-1].getState() == hstate)){
					cout << x-1 << " " << y-1 <<" " << 3<<endl;
					return;
				}else if ((y+1 <= 9) && (x-1 >= 0)&& (theBoard[x-1][y+1].getState() == hstate)){
					cout << x-1 <<" "<< y+1<< " " << 2<<endl;
					return;
				}else if ((x+3<=9) && (theBoard[x+3][y].getState() == hstate)){
					cout << x+3 <<" "<< y << " " << 0<<endl;
					return;
				}else if ((x+2<= 9) && (y-1 >= 0)&& (theBoard[x+2][y-1].getState() == hstate)){
					cout << x+2 << " "<< y-1 << " "<< 3<<endl;
					return;
				}else if ((x+2 <= 9) && (y+1 <= 9) && (theBoard[x+2][y+1].getState() == hstate)){
					cout << x+2 << " " << y+1 << " " << 2<<endl;
					return;
				}
			}
		}
	}
	for(int i=0; i<=9;i++){
		for(int j=0; j<=7; j++){
			if(theBoard[i][j].getState() == theBoard[i][j+2].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
			}
			if(x-1 >=0 && y+1 <=9 &&(theBoard[x-1][y+1].getState() == hstate)){
				cout<< x-1<<" " <<y+1<<" "<< 1<<endl;
				return;
			}else if(x+1<=9 && y+1<=9 &&(theBoard[x+1][y+1].getState()==hstate)){
				cout<< x+1<<" "<<y+1<<" "<<0<<endl;
				return;
			}
		}
	}
	for(int j=0; j<=9;j++){
		for(int i=0; i<=7; i++){
			if(theBoard[i][j].getState() == theBoard[i+2][j].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
			}
			if(x+1<=9 && y-1 >=9 &&(theBoard[x+1][y-1].getState() == hstate)){
				cout<< x+1<<" " <<y-1<<" "<< 3<<endl;
				return;
			}else if(x+1<=9 && y+1<=9 &&(theBoard[x+1][y+1].getState()==hstate)){
				cout<< x+1<<" "<<y+1<<" "<<2<<endl;
				return;
			}
		}
	}
	cout<<"Scramble Available"<<endl;
	NomoreMatch=true;
}


bool Board::checkNomoreMatch(){
	int x;
	int y;
	int hstate;
	for(int i=0; i<=9; ++i){
		for(int j=0; j<=8; ++j){
			if(theBoard[i][j].getState() == theBoard[i][j+1].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();


				if((y-2>=0) && (theBoard[x][y-2].getState() == hstate)){
					return false;
				}else if ((y-1 >= 0) && ( x-1 >= 0) && (theBoard[x-1][y-1].getState() == hstate)){
					return false;
				}else if ((y-1 >= 0) && (x+1 <= 9)&& (theBoard[x+1][y-1].getState() == hstate)){
					return false;
				}else if ((y+3<=9) && (theBoard[x][y+3].getState() == hstate)){
					return false;  
				}else if ((x-1>= 0) && (y+2<= 9)&& (theBoard[x-1][y+2].getState() == hstate)){
					return false;
				}else if ((x+1 <= 9) && (y+2 <= 9) && (theBoard[x+1][y+2].getState() == hstate)){
					return false;
				}
			}
		}
	}
	for(int j=0; j<=9; ++j){
		for(int i=0; i<=8; ++i){
			if(theBoard[i][j].getState() == theBoard[i+1][j].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
				if((x-2>=0) && (theBoard[x-2][y].getState() == hstate)){
					return false;
				}else if ((y-1 >= 0) && ( x-1 >= 0) && (theBoard[x-1][y-1].getState() == hstate)){
					return false;
				}else if ((y+1 <= 9) && (x-1 >= 0)&& (theBoard[x-1][y+1].getState() == hstate)){
					return false;
				}else if ((x+3<=9) && (theBoard[x+3][y].getState() == hstate)){
					return false;
				}else if ((x+2<= 9) && (y-1 >= 0)&& (theBoard[x+2][y-1].getState() == hstate)){
					return false;
				}else if ((x+2 <= 9) && (y+1 <= 9) && (theBoard[x+2][y+1].getState() == hstate)){
					return false;
				}
			}
		}
	}
	for(int i=0; i<=9;i++){
		for(int j=0; j<=7; j++){
			if(theBoard[i][j].getState() == theBoard[i][j+2].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
			}
			if(x-1 >=0 && y+1 <=9 &&(theBoard[x-1][y+1].getState() == hstate)){
				return false;
			}else if(x+1<=9 && y+1<=9 &&(theBoard[x+1][y+1].getState()==hstate)){
				return false;
			}
		}
	}
	for(int j=0; j<=9;j++){
		for(int i=0; i<=7; i++){
			if(theBoard[i][j].getState() == theBoard[i+2][j].getState()){
				x=i;
				y=j;
				hstate=theBoard[i][j].getState();
			}
			if(x+1<=9 && y-1 >=9 &&(theBoard[x+1][y-1].getState() == hstate)){
				return false;
			}else if(x+1<=9 && y+1<=9 &&(theBoard[x+1][y+1].getState()==hstate)){
				return false;
			}
		}
	}
	NomoreMatch=true;
	return true;
}


void Board::scramble(){
	score1 = 0;
	numsquares = 0;
	chaincounter=-1;
	bool success=false;
	for(int p=0; p<=3; p++){
		for(int i=0; i<= 9; i++){
			for(int j=0; j<=9; j++){
				if(theBoard[i][j].getState() == '0'+p){
					char temp=theBoard[i][j].getState();
					char temp2=theBoard[i][j].getType();
					theBoard[i][j].setState(theBoard[5][5].getState());
					theBoard[i][j].setType(theBoard[5][5].getType());
					theBoard[5][5].setState(temp);
					theBoard[5][5].setType(temp2);

					goto Stop;
				}
			}

		}
Stop:
		for(int i=0;i<=9;i++){
			for(int j=0; j<=9; j++){
				if((theBoard[i][j].getState() == '0'+p) && ((i != 5) || (j != 5))){
					char temp=theBoard[i][j].getState();
					char temp2=theBoard[i][j].getType();
					theBoard[i][j].setState(theBoard[5][6].getState());
					theBoard[i][j].setType(theBoard[5][6].getType());;
					theBoard[5][6].setState(temp);
					theBoard[5][6].setType(temp2);
					goto Stop2;
				}
			}
		}
Stop2:
		for(int i=0;i<=9;i++){
			for(int j=0; j<=9; j++){
				if((theBoard[i][j].getState() =='0'+ p) && ((i != 5) || ((j != 6) && (j != 5)))){
					char temp=theBoard[i][j].getState();
					char temp2=theBoard[i][j].getType();
					theBoard[i][j].setState(theBoard[5][8].getState());
					theBoard[i][j].setType(theBoard[5][8].getType());
					theBoard[5][8].setState(temp);
					theBoard[5][8].setType(temp2);
					success=true;
					NomoreMatch=false;
					goto Stop3;
				}
			}
		}
Stop3:
		if(success){
			break;
		}
	}
}


void Board::init(){}
void Board::Generator(int r, int c){}
void Board::RandLocker(){}
void Board::levelupCheck(){}














