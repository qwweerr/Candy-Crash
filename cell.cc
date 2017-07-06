#include "cell.h"
using namespace std;

Cell::Cell(){
	lock= '_';
	type= '_';
	state='x';
	r=0;
	c=0;
	x=0;
	y=0;
	width=0;
	height=0;
	window=0;
        checking=false;
        
}

char Cell::getState(){
	return state;
}

char Cell::getType(){
	return type;
}

int Cell::getr(){
	return r;
}

int Cell::getc(){
	return c;
}

char Cell::getLock(){
	return lock;
}
bool Cell::getchecking(){
  	return checking;
}
void Cell::setchecking(){
   	checking=false;
}
void Cell::setLock(char lock){
	this->lock=lock;
}


void Cell::setType(char type){
	this->type=type;
}

void Cell::setState(char state){
	this->state=state;
}

void Cell::setCell(char lock,char type,char change){
	this->state=change;
	this->lock=lock;
	this->type=type;
}
void Cell::setCoords(int r,int c, int x,int y,int width, int height, Xwindow *w){
	this->r=r;
	this->c=c;
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;
	this->window=w;
}


bool Cell::IsHole(){
	if(type == '_'&& state == 'x'){
		return true;
	}else{
		return false;
	}
}
void Cell::MakeHole(){
	lock='_';
	type='_';
	state='x';
}

void Cell::LocklessHole(){
	type='_';
	state='x';
}
void Cell::unstablesquare(){
        checking=true;
	char s = state;
	MakeHole();
	setCell(lock,'b',s);
}

void Cell::uprightsquare(){
        checking=true;
	char s = state;
	MakeHole();
	setCell(lock,'v',s);
}

void Cell::lateralsquare(){
	checking=true;
	char s = state;
	MakeHole();
	setCell(lock,'h',s);
}

void Cell::psychedelicsquare(){
	checking=true;
	char s = state;
	MakeHole();
	setCell(lock,'p',s);
}

void Cell::draw(){
	window->fillRectangle(x,y,width,height,state - '0');
}

void Cell::printString(){
	if(type == 'h'){
		window->fillRectangle(x+2.5,y+22.5,45,5,7);
	}else if(type == 'v'){
		window->fillRectangle(x+22.5,y+2.5,5,45,7);
	}else if(type == 'p'){
		window->fillRectangle(x+2.5,y+22.5,45,5,7);
		window->fillRectangle(x+22.5,y+2.5,5,45,7);	
		window->fillCircle(x+12.5,y+12.5,25,7);	
	}else if(type== 'b'){
		window->fillCircle(x+12.5,y+12.5,25,7);
	}
}



void Cell::printBigString(){
	window->fillRectangle(x+2.5,y+2.5,45,5,9);
	window->fillRectangle(x+2.5,y+42.5,45,5,9);
	window->fillRectangle(x+2.5,y+2.5,5,45,9);
	window->fillRectangle(x+42.5,y+2.5,5,45,9);
}
