#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#include<iostream>
#include<vector>
#include<math.h>
#define SIZE 50
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

void adjustWindowSize(int w, int h)
{
	HWND wh = GetConsoleWindow();
 
    // Move window to required position
    MoveWindow(wh, 100, 100, w, h, TRUE);
//    SMALL_RECT test; 
//
//    HANDLE hStdout;
//    COORD coord;
//    BOOL ok;
//
//    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
//    coord.X = 2200;
//    coord.Y = 2200;
//    ok = SetConsoleScreenBufferSize(hStdout, coord);
//
//    test.Left = 0;
//    test.Top = 0;
//    test.Right = coord.X-1;
//    test.Bottom = coord.Y-1;
//
//    SetConsoleWindowInfo(hStdout, ok, &test);

} //end adjustWindowSize 
using namespace std;
void clear(){
	system("cls");
}
struct Position{
	int y;
	int x;
};

struct Position *newPosition(int y, int x){
	struct Position *temp=(struct Position *)malloc(sizeof(Position));
	temp->y=y;
	temp->x=x;
	return temp;
}
class Mortal{
	public: 
		int hp;
		char symbol;
		Position *position;
		bool entityObstacle;
		bool projectileObstacle;
};

struct Tile{
	char symbol;
	Mortal *mortal;
};
struct Tile map[SIZE][SIZE]={0};
void initMap(){
	for(int i=0;i<SIZE;i++){
		for(int t=0;t<SIZE;t++){
			map[i][t].symbol=' ';
			map[i][t].mortal=NULL;
//			
		}
	}
	for(int i=0;i<SIZE;i++){
		map[i][0].symbol='#';
		map[i][SIZE-1].symbol='#';
		map[0][i].symbol='#';
		map[SIZE-1][i].symbol='#';
		
	}
}
int keypress(){
	int benar=0;
	while(benar==0){
		int hit = kbhit();
		int ch;
		if (hit){	
			ch = getch();
			benar=1;
			return ch;
		}	
	}
}
class Projectile: public Mortal{
	public:
	Position *direction;
	char animationSymbol;
	int damage;
	Projectile(Position *position, Position *direction,char symbol, int damage){
		this->hp=1;
		this->position=position;
		this->direction=direction;
		this->symbol=symbol;
		this->damage=damage;
		this->entityObstacle=false;
	}
	
	bool move(){ 
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;
		
		position->x=position->x+direction->x;
		position->y=position->y+direction->y;
		int dir=(rand()%2==0?-1:1);
		int dir2=(rand()%2==0?-1:1);
		
		while(map[this->position->y][this->position->x].mortal!=NULL&&map[this->position->y][this->position->x].mortal->symbol=='*'){
			
			this->position->y=this->position->y+(rand()%2*this->direction->x*dir);
			
			this->position->x=this->position->x+(rand()%2*this->direction->y*dir2);
//			keypress();
			
		}

		
		if(map[position->y][position->x].mortal){
			map[position->y][position->x].mortal->hp=map[position->y][position->x].mortal->hp-damage;
			if(map[position->y][position->x].mortal->hp<=0){
				map[position->y][position->x].mortal=NULL;
				map[position->y][position->x].symbol=' ';
			}
			return false;
		}
		else if(position->x>=1&&position->y>=1&&position->x<SIZE-1&&position->y<SIZE-1&&map[position->y][position->x].symbol!='#'&&this->hp>0){
			map[position->y][position->x].symbol=this->symbol;
			map[position->y][position->x].mortal=this;
					
			return true;	
		}

		return false;
	}
	bool validate(){ 
		
		
		return position->x>=1&&position->y>=1&&position->x<SIZE-1&&position->y<SIZE-1&&map[position->y][position->x].symbol!='#';
	}
};


class Missile: public Mortal{
	public:
	Position *direction;
	char animationSymbol;
	int damage;
	// Creating a shortcut for int, int pair type
	typedef pair<int, int> Pair;
	 
	// Creating a shortcut for pair<int, pair<int, int>> type
	typedef pair<double, pair<int, int> > pPair;
	
	Missile(Position *position, Position *direction,char symbol, int hp){
		this->hp=1;
		this->position=position;
		this->direction=direction;
		this->symbol=symbol;
		this->damage=1;
	}
	
	bool move(){
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;
		
		position->x=position->x+direction->x;
		position->y=position->y+direction->y;
		int dir=(rand()%2==0?-1:1);
		int dir2=(rand()%2==0?-1:1);
		
		
		while(map[this->position->y][this->position->x].mortal!=NULL&&map[this->position->y][this->position->x].mortal->symbol=='o'){
			
			this->position->y=this->position->y+(rand()%2*this->direction->x*dir);
			
			this->position->x=this->position->x+(rand()%2*this->direction->y*dir2);
//			keypress();
			
		}


		if(map[position->y][position->x].mortal){
			map[position->y][position->x].mortal->hp=map[position->y][position->x].mortal->hp-damage;
			return false;
		}
		
		else if(position->x>=1&&position->y>=1&&position->x<SIZE-1&&position->y<SIZE-1&&this->hp>0){
			map[position->y][position->x].symbol=this->symbol;
			map[position->y][position->x].mortal=this;
			return true;
		}
	}	
};
class Enemy: public Mortal{
	public:
	std::vector<Missile*> missiles;	
	Position *direction;
	
	Enemy(int hp, struct Position *position, struct Position *direction,char symbol){
		this->position=position;
		this->direction=direction;
		this->hp=hp;
		this->symbol=symbol;
		this->entityObstacle=true;
		map[this->position->y][this->position->x].symbol=symbol;
		map[this->position->y][this->position->x].mortal=this;
		
	}	
	bool isAlive(){
		return hp>0;
	}
	void shoot(){
		Missile *shootedMissile=new Missile(newPosition(position->y+direction->y,position->x+direction->x),newPosition(direction->y,direction->x),'o',1);
		missiles.push_back(shootedMissile);

	}	
	void validateMissiles(){
		for(int i=0;i<missiles.size();i++){
			Missile *tempe=missiles[i];
			if(!tempe->move()){   
				
				missiles.erase(missiles.begin()+i);
				i--;
			}
			else{
				if(!tempe->move()){   
					missiles.erase(missiles.begin()+i);
					i--;
				}
			}
		}
	}

	void terminated(){
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;

		
	}
};
class Player: public Mortal{
	public:
	std::vector<Projectile*> projectiles;	
	Position *direction;
	
	Player(int hp, struct Position *position, struct Position *direction,char symbol){
		this->position=position;
		this->direction=direction;
		this->hp=hp;
		this->symbol=symbol;
		map[this->position->y][this->position->x].symbol=symbol;
		map[this->position->y][this->position->x].mortal=this;
		
	}
	void move(int y, int x){
		if(map[this->position->y+y][this->position->x+x].symbol=='#'||(map[this->position->y+y][this->position->x+x].mortal&&map[this->position->y+y][this->position->x+x].mortal->entityObstacle)){
			map[this->position->y][this->position->x].symbol=this->symbol;
			return;
		}
		map[this->position->y][this->position->x].symbol=' ';
		map[this->position->y][this->position->x].mortal=NULL;
		
		this->position->y+=y;
		this->position->x+=x;
		map[this->position->y][this->position->x].symbol=symbol;
		map[this->position->y][this->position->x].mortal=this;
		
	}
	void face(int y, int x, char symbol){
		this->direction->y=y;
		this->direction->x=x;		
		this->symbol=symbol;
		map[this->position->y][this->position->x].symbol=symbol;
		
	}
	void shoot(){
		Projectile *shootedProjectile=new Projectile(newPosition(position->y,position->x),newPosition(direction->y,direction->x),'*',1);
		projectiles.push_back(shootedProjectile);
		
//		if(map[shootedProjectile->position->y][shootedProjectile->position->x].mortal!=NULL&&map[shootedProjectile->position->y][shootedProjectile->position->x].mortal->symbol=='*'){
//			shootedProjectile->position->y=shootedProjectile->position->y+rand()%1;
//			shootedProjectile->position->x=shootedProjectile->position->x+rand()%1;
//			keypress();
//			
//		}

		if(!shootedProjectile->move()){   
			projectiles.pop_back();
		}
		
	}
	void blinkProjectiles(bool blink){
		for(int i=0;i<projectiles.size();i++){
			Projectile *tempe=projectiles[i];
			if(blink)map[tempe->position->y][tempe->position->x].symbol=tempe->symbol;
			else map[tempe->position->y][tempe->position->x].symbol=tempe->animationSymbol;
			
		}
	}
	void validateProjectiles(){
		for(int i=0;i<projectiles.size();i++){
			Projectile *tempe=projectiles[i];
			if(!tempe->move()){   
				projectiles.erase(projectiles.begin()+i);
				i--;
			}
			else{
				if(!tempe->move()){   
					projectiles.erase(projectiles.begin()+i);
					i--;
				}
			}
		}
	}
	
};





int waitKeyPress(int x){
	int milsec = 100 *x;
		clock_t startTime = clock();
		while(clock() < (startTime + milsec)){
			int hit = kbhit();
			int ch;
			if (hit){	
				ch = getch();
				return ch;
			}	
		}
		return -1;
}

int realTimePress(){
		int ch=-1;

			int hit = kbhit();
			if (hit){	
				ch = getch();
				
			}	
		return ch;
}








void printMap(Player *currPlayer){
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {0,0};
	SetConsoleCursorPosition(hConsole,pos); 
	for(int i=0;i<SIZE;i++){
		for(int t=0;t<SIZE;t++){
			printf("%c ",map[i][t].symbol);
		}
		printf("\n");
		
	}
	printf("hp: %d",currPlayer->hp);
}











void playerController(Player *currPlayer,bool blink){
	if(blink)currPlayer->shoot();
	
	currPlayer->validateProjectiles();
	int key=-1;
	key=realTimePress();
	switch(key){
		case 'w':
			currPlayer->move(-1,0);
			break;
		case 'W':
			currPlayer->move(-1,0);
			break;
		case 'a':
			currPlayer->move(0,-1);
			break;
		case 'A':
			currPlayer->move(0,-1);
			break;
		case 's':
			currPlayer->move(1,0);
			break;
		case 'S':
			currPlayer->move(1,0);
			break;
		case 'd':
			currPlayer->move(0,1);
			break;
		case 'D':
			currPlayer->move(0,1);
			break;
		case 'i':
			currPlayer->face(-1,0,'^');
			break;
		case 'I':
			currPlayer->face(-1,0,'^');
			break;
		case 'j':
			currPlayer->face(0,-1,'<');
			break;
		case 'J':
			currPlayer->face(0,-1,'<');
			break;
		case 'k':
			currPlayer->face(1,0,'v');
			break;
		case 'K':
			currPlayer->face(1,0,'v');
			break;
		case 'l':
			currPlayer->face(0,1,'>');
			break;
		case 'L':
			currPlayer->face(0,1,'>');
			break;
		default:
			currPlayer->move(0,0);
	}
	
}





vector <Enemy*> *initEnemy(){
	vector<Enemy*> *enemyList=new vector<Enemy*>();
	
	enemyList->push_back(new Enemy(10,newPosition(4,4), newPosition(1,0),'E'));

	enemyList->push_back(new Enemy(10,newPosition(4,SIZE-5), newPosition(1,0),'E'));

	enemyList->push_back(new Enemy(10,newPosition(SIZE-5,4), newPosition(1,0),'E'));

	enemyList->push_back(new Enemy(10,newPosition(SIZE-5,SIZE-5), newPosition(1,0),'E'));
	
	return enemyList;
}
void enemyController(vector <Enemy*> *enemyList, bool blink){
	for(int i=0;i<enemyList->size();i++){
		Enemy *tempEnemy=enemyList->at(i);
		
		tempEnemy->validateMissiles();
		if(rand()%4==0&&blink)tempEnemy->shoot();
		if(!tempEnemy->isAlive()){   
			
			tempEnemy->terminated();
			enemyList->erase(enemyList->begin()+i);
			i--;


		}else{
			map[tempEnemy->position->y][tempEnemy->position->x].symbol=tempEnemy->symbol;
			printf("%c",tempEnemy->symbol);
		}
	}
}
void playGame(){
	adjustWindowSize(1000,1000);
	clear();
	
	initMap();
	int gameStatus=0;
	bool blink=true;
	Player *currPlayer=new Player(3, newPosition(9,14),newPosition(-1,0),'^');
	vector<Enemy*> *enemyList= initEnemy();
	while(gameStatus==0){
		printMap(currPlayer);
		enemyController(enemyList,blink);
		playerController(currPlayer,blink);		
		blink=!blink;
	}	
}

void printIntro(bool blink){
	clear();
    printf("                   .. ......................................''..............                     \n");  
	printf("              ..  ... ......................................:c'.................                 \n");  
	printf(" ....         ..   ..  ........ ....  ......................................   .    .            \n");  
	printf(".....   ............. ...........................................................  .... .        \n");  
	printf("..      ......... ... ...........................................................  ........   .  \n");  
	printf("     . ........................................................................... ........   ...\n");  
	printf(".. ...........''.................................................................................\n");  
	printf("....................''','........................................................................\n");  
	printf("....................,;:::;;;;,''.................................................................\n");  
	printf("........................',;;;,,,,,',,,'..........................................................\n");  
	printf("...............................'''',,,,''''......................................................\n");  
	printf(".......,'...............................'','....................''...............................\n");  
	printf("......,c;......              _                                                     ,.............\n");  
	printf(".......'.......           | |_)   o    /\\     _|_  _  ._ _   _. _|_  _.            ;.............\n");  
	printf(".........'....'         \\_| |     o   /--\\ |_| |_ (_) | | | (_|  |_ (_|            :.............\n");  
	printf("'...'''.','''''                                                                    ;.............\n");  
	printf(",,,,,,,,,,,,'',,;,'....''....',,''',;'',clldxdl:;:::::c:;;c:;,;,,;,'';;,,;',,'','','''...........\n");  
	printf(";;;:::;;;;,,,,,,,,''......'''''',,,;;                         :ccc::::::;;;;;;,,,,,'''''''''''''.\n");  
	printf("llooddooxdc:;;::::,'''''''',,;:ccccld    Press Any Button");
	if(blink)printf("_");
	else printf(" ");
	printf("    00000OOOOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printf("dooodxxxxocc::cllc;,,,,,,,,;;:coollkK                         0000OkxkOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printf("olc::::::cc:;;;;;,,,,,;;;;;:::clxOkOOxkOxxxddddxkOOxodOKkxxdooxdooooollcoxxxdxollodllllcclddoc:::\n");  
	printf("lcc:cllllooooolc:;:;;;::;:clc:cloxxkkddddoloolccoxkdclxklclccccccccc:::cldllllcclcccc:cccclolcccd\n");  
	printf("llllclollcclooddloolc:::c:clccloollxOOdllcloolocldxl;::c:;;::;;::;;;;;;;::::;;;:::::;:cc:::colclx\n");  
	printf("oddolcc::::lodddddddddllccclloollcldkOdccccldxkocooc,;;;;;,;;,,,;;;;;;;;;;::;;;;:::::clc:::clc::l\n");  
	printf("lolllcccccllodolcclodoc:clooccc:::cloolcccoxxoll:;,;;;;,,,,,,,,:c:,,;;::::colc::lllccccc::clollll\n");  
	printf("clllllloodxdoool:;:cooc:lddlcccc:::cccclccldoc;;,,';;,,,,,'''',;:;,;c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printf("looooooodddolc:clloolclccllcccllc::cllc::::loc;,;;:;,'''''',,'''''',;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printf("oolccloodolccccccllolllc:::cc:::;,,,;:;;,,,,;;,',;;,''',,,;,,,,,'''''''..','''''........';::;,,;;\n");  
	printf(";;:c:cccllooollc::cclolc:::::;,,,,,,,,,,,,,,''''''''',;;;:c:;;:;,,,''''....'.............'...';:,\n");  
	printf(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printf(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printf("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printf(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");   
}

void intro(){

	bool blink=true;
	int input=-1;
	while(input==-1){
		blink=!blink;
		printIntro(blink);
		input=waitKeyPress(8);
	}
}


void printMenu(bool blink, int menu){
	clear();
	printf("                   .. ......................................''..............                     \n");  
	printf("              ..  ... ......................................:c'.................                 \n");  
	printf(" ....         ..   ..  ........ ....  ......................................   .    .            \n");  
	printf(".....   ............. ...........................................................  .... .        \n");  
	printf("..      ......... ... ...........................................................  ........   .  \n");  
	printf("     . ........................................................................... ........   ...\n");  
	printf(".. ...........''.................................................................................\n");  
	printf("....................''','........................................................................\n");  
	printf("....................,;:::;;;;,''.................................................................\n");  
	printf("........................',;;;,,,,,',,,'..........................................................\n");  
	printf("...............................'''',,,,''''......................................................\n");  
	printf(".......,'...............................'','....................''...............................\n");  
	printf("......,c;......              _                                                     ,.............\n");  
	printf(".......'.......           | |_)   o    /\\     _|_  _  ._ _   _. _|_  _.            ;.............\n");  
	printf(".........'....'         \\_| |     o   /--\\ |_| |_ (_) | | | (_|  |_ (_|            :.............\n");  
	printf("'...'''.','''''                                                                    ;.............\n");  
	printf(",,,,,,,,,,,,'',,;,'....''....',,''',;'',clldxdl:;:::::c:;;c:;,;,,;,'';;,,;',,'','','''...........\n");  
	printf(";;;:::;;;;,,,,,,,,''......'''''',,,;;                         :ccc::::::;;;;;;,,,,,'''''''''''''.\n");  
	printf("llooddooxdc:;;::::,'''''''',,;:ccccld      ");
	if(menu==1)printf("> ");
		else printf("  ");

	printf("New Game");
	if(menu==1&&!blink)printf("_");
	else printf(" ");  

	printf("        0000OOOOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printf("dooodxxxxocc::cllc;,,,,,,,,;;:coollkK                         0000OkxkOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printf("olc::::::cc:;;;;;,,,,,;;;;;:::clxOkOO      ");
	if(menu==2)printf(">   ");
		else printf("    ");

	printf("About");
	if(menu==2&&!blink)printf("_");
	else printf(" ");  

	printf("         xdooooollcoxxxdxollodllllcclddoc:::\n");  
	printf("lcc:cllllooooolc:;:;;;::;:clc:cloxxkk                         cccccc:::cldllllcclcccc:cccclolcccd\n");  
	printf("llllclollcclooddloolc:::c:clccloollxO     ");
	if(menu==3)printf(">     ");
	else printf("      ");
	printf("Exit");
	if(menu==3&&!blink)printf("_");
	else printf(" ");  

	printf("          ;::;;;;;;;::::;;;:::::;:cc:::colclx\n");  
	printf("oddolcc::::lodddddddddllccclloollcldk                         ,,;;;;;;;;;;::;;;;:::::clc:::clc::l\n");  
	printf("lolllcccccllodolcclodoc:clooccc:::cloolcccoxxoll:;,;;;;,,,,,,,,:c:,,;;::::colc::lllccccc::clollll\n");  
	printf("clllllloodxdoool:;:cooc:lddlcccc:::cccclccldoc;;,,';;,,,,,'''',;:;,;c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printf("looooooodddolc:clloolclccllcccllc::cllc::::loc;,;;:;,'''''',,'''''',;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printf("oolccloodolccccccllolllc:::cc:::;,,,;:;;,,,,;;,',;;,''',,,;,,,,,'''''''..','''''........';::;,,;;\n");  
	printf(";;:c:cccllooollc::cclolc:::::;,,,,,,,,,,,,,,''''''''',;;;:c:;;:;,,,''''....'.............'...';:,\n");  
	printf(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printf(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printf("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printf(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
}


int menuInputController(int *menu){
	
	char input=waitKeyPress(8);
	switch(input){
		case 'w':
			*menu=*menu-1;
		break;
		case 'W':
			*menu=*menu-1;
		break;
		case 's':
			*menu=*menu+1;
		break;
		case 'S':
			*menu=*menu+1;
		break;
		case 13: 
			return *menu;
		break;	
	}
	if(*menu<1)*menu=1;
	else if(*menu>3)*menu=3;
	return -1; 
}
void menu(){

	bool blink=true;
	int input=-1;
	int menu=1;
	int choice=0;
	while(choice!=3){
		blink=!blink;
		printMenu(blink,menu);
		choice=menuInputController(&menu);
		switch(choice){
			case 1:
				playGame();
				break;
				
			case 2:
				
				break;
		}
		
		
	}
}






int main(){
	srand(time(NULL));
	adjustWindowSize(1000,600);
	intro();	
	menu();
}

