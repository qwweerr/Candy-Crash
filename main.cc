#include "textdisplay.h"
#include "window.h"
#include "board.h"
#include "cell.h"
#include "level0.h"
#include "level1.h"
#include "level2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;


void opening(){
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<"      @@@@@          @@@@@       @@@@@@@@@@@@@      @@       @@     "<<endl;
	cout<<"    @@     @@      @@     @@     @@                 @@     @@       "<<endl;
	cout<<"   @@       @@    @@       @@    @@                 @@    @@        "<<endl;
	cout<<"   @@             @@             @@                 @@  @@          "<<endl;
	cout<<"    @@             @@            @@@@@@@@@@@        @@@@@           "<<endl;
	cout<<"     @@@@           @@@@                   @@       @@@@@           "<<endl;
	cout<<"        @@@@           @@@@                 @@      @@  @@          "<<endl;
	cout<<"           @@             @@                 @@     @@   @@         "<<endl;
	cout<<"           @@             @@                 @@     @@    @@        "<<endl;
	cout<<"   @@      @@    @@       @@     @@         @@      @@     @@       "<<endl;
	cout<<"    @@    @@      @@     @@       @@@     @@@       @@      @@      "<<endl;
	cout<<"     @@@@@          @@@@@          @@@@@@@@        @@@       @@@    "<<endl;
	cout<<endl;
        cout<<endl;
        cout<<endl;
        cout<<endl;
}

void close(){
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<"    @@               @@          @@@@@@          @@        @@        @@   "<<endl;
	cout<<"    @@               @@        @@      @@        @@     @@ @@       @@@@   "<<endl;
	cout<<"    @@      @@@      @@       @@        @@       @@    @@  @@       @@@@   "<<endl;
        cout<<"    @@     @@ @@     @@       @@        @@       @@   @@   @@       @@@@   "<<endl;
        cout<<"     @@   @@   @@   @@        @@        @@       @@  @@    @@        @@   "<<endl;
        cout<<"      @@ @@     @@ @@          @@      @@        @@ @@     @@             "<<endl;
        cout<<"       @@        @@              @@@@@@          @@        @@        @@   "<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
}

int main(int argc, char* argv[]){
	opening();

	// Take care of the command line argument
	int text_index=0;
	int seed_index=0;
	int script_index=0;
	int startlevel_index=0;
	Board* lv;

	for(int i=0; i<argc;i++){
		string s(argv[i]);
		if(s=="-text"){
			text_index = i;
		}else if(s=="-seed"){
			seed_index = i;
		}else if(s=="-scriptfile"){
			script_index = i;
		}else if(s=="-startlevel"){
			startlevel_index = i;
		}
	}

	bool hasText = (text_index != 0 ? true : false);
	bool hasSeed = (seed_index != 0 ? true : false);
	bool hasScript = (script_index !=0 ? true : false);
	bool hasStartlevel = (startlevel_index !=0 ? true : false);

	string s;
	int x,y,z;
	int level=0;
	int score=0;
	string scriptfile;
	if(hasSeed){
		istringstream iss(argv[seed_index+1]);
		int n;
		iss >> n;	
		srand(n);
	}

	if(hasStartlevel){
		istringstream iss(argv[startlevel_index+1]);
		int n;
		iss >> n;
		level = n;
	}

	if(hasScript){
		scriptfile = argv[script_index+1];
	}else if(level==0){
		scriptfile = "sequence.txt";
	}else if(level==1 || level==2){
		scriptfile= "No Scriptfile";
	}
	if(level==0){
		lv = new Level0(scriptfile, score, hasText);
		lv->init();
		cout << "Level 0 Game Board Created!!"  <<endl;
	}else if(level==1){
		lv = new Level1(scriptfile, score, hasText);
		lv->init();
		cout << "Level 1 Game Board Created!!" <<endl;
	}else if(level==2){
		lv = new Level2(scriptfile, score, hasText);
		lv->init();
		cout << "Level 2 Game Board Created!!"  <<endl;
	}
	cout << *lv << endl;

	while(cin >> s){
		if(hasScript){
			scriptfile = argv[script_index+1];
		}else if(level==0){
			scriptfile = "sequence.txt";
		}else if(level==1 || level==2){
			scriptfile= "No Scriptfile";
		}

		if (s == "swap"){
			cin >> x >> y >> z;
			lv->swap(x,y,z);
			if(!hasText){
				lv->drawBoard();
			}
			score = lv->getScore();
			cout << *lv << endl;
		}

		if(s == "hint"){
			lv->hint();
		}
		if(s == "scramble"){
			if(lv->checkNomoreMatch()){
				lv->scramble();
				if(!hasText){
					lv->drawBoard();
				}
				cout << *lv << endl;
			}else{
				cout<< "Moves Available!"<<endl;
			}
		}
		if( s == "restart"){
			score = lv->getScore();
			delete lv;
			if(level==0){
				lv = new Level0(scriptfile, score,hasText);
				lv->init();
				cout << "Level 0 Game Board Created!!"  <<endl;
			}else if(level==1){
				lv = new Level1(scriptfile,score,hasText);
				lv->init();
				cout << "Level 1 Game Board Created!!"  <<endl;
			}else if(level==2){
				lv = new Level2(scriptfile, score,hasText);
				lv->init();
				cout << "Level 2 Game Board Created!!"  <<endl;
			}
			cout << *lv << endl;			
		}
		if(s=="levelup" || lv->getlevelInc()){
			score = lv->getScore();
			if(level==0){
				level=1;
				delete lv;
				scriptfile = "No Scriptfile";
				lv = new Level1(scriptfile, score,hasText);
				lv->init();
				cout << "Level 1 Game Board Created!!"   <<endl;
				cout << *lv <<endl;
			}else if(level==1){
				level = 2;
				delete lv;
				scriptfile = "No Scriptfile";
				lv = new Level2(scriptfile, score, hasText);
				lv->init();
				cout << "Level 2 Game Board Created!!"  <<endl;
				cout << *lv <<endl;
			}else if(level==2){
				cout << "Level 2 completed! Game Won!" <<endl;
				close();
				break;
			}
			lv->setlevelInc();		
		}
		if(s=="leveldown"){
			score = lv->getScore();
			if(level==2){
				level=1;
				delete lv;
				scriptfile = "No Scriptfile";
				lv = new Level1(scriptfile, score, hasText);
				lv->init();
				cout << "Level 1 Game Board Created!!"  <<endl;
				cout << *lv <<endl;
			}else if(level==1){
				level = 0;
				delete lv;
				scriptfile = "sequence.txt";
				lv = new Level0(scriptfile, score, hasText);
				lv->init();
				level = 0;
				cout << "Level 0 Game Board Created!!" <<endl;
				cout << *lv <<endl;
			}else if(level==0){
				cout << "Lowest Level! No leveldown possible."<<endl;
			}

		}
		if(s== "quit"){
			break;
		}
	}
	delete lv;
}

