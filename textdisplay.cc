#include "textdisplay.h"
using namespace std;

TextDisplay::TextDisplay(){
	theDisplay = new string* [10];
	for(int i=0; i<10; i++){
		theDisplay[i] = new string[10];
		for(int j=0; j<10; j++){
			theDisplay[i][j] = '0';
		}
	}
	Level = 0;
	score = 0;
}

void TextDisplay::notify(int r, int c, char lock, char type, char state){
        char arr[4];
        arr[0]=lock;
        arr[1]=type;
        arr[2]=state;
        arr[3]=0;
        string s(arr);
	theDisplay[r][c] = s;
}

void TextDisplay::notifyscore(int s){ score = s;}

void TextDisplay::notifyLevel(int l){ Level = l;}

TextDisplay::~TextDisplay(){
	for(int i=0; i<10; i++){
		delete [] theDisplay[i];
	}
	delete [] theDisplay;
}

ostream &operator<<(ostream &out, const TextDisplay &td){
	out<<endl;
	out<<"{{ ======================================= }}"<<endl;
	out<<"{{ ======================================= }}"<<endl;
	out<<"{{                                         }}"<<endl;
	for(int i=0; i<10; i++){
		out<<"{{ ";
		for(int j=0; j<10; j++){
			out << td.theDisplay[i][j] << " ";
		}
		out <<"}}"<< endl;
	}
	out<<"{{                                         }}"<<endl;
	out<<"{{ ======================================= }}"<<endl;
	out<<"{{ I    Level:  " << left<< setw(1) << td.Level <<"     "<<"Score:  "<<left<<setw(11)<< td.score <<"I }}"<< endl;
        out<<"{{ ======================================= }}"<<endl;	
	return out;
}
