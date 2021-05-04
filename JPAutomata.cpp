#include<stdio.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#include<iostream>
#include<vector>
#define SIZE 20           


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
		
};



class Projectile {
	public:
	Position *position;
	Position *direction;
	Projectile(Position *position, Position *direction){
		
	}
	
	bool move(){
		position->x=position->x+direction->x;
		position->y=position->y+direction->y;
		return position->x>=0&&position->y>=0&&position->x<SIZE&&position->y<SIZE;
	}
};

class Player: public Mortal{
	std::vector<Projectile> projectiles;	
	Player(int hp, int y, int x){
		this->position=newPosition(y,x);
		this->hp=hp;
	}
	void validateProjectiles(){
		for(int i=0;i<projectiles.size();i++){
			Projectile tempe=projectiles[i];
			if(!tempe.move()){   
				projectiles.erase(projectiles.begin()+i);
				i--;
				
			}
		}
	}
};

struct Tile{
	char symbol;
	Mortal mortal;
};



Tile map[SIZE][SIZE]={0};



void clear(){
	system("CLS");
}
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










void printMap(){
	for(int i=0;i<SIZE;i++){
		for(int t=0;t<SIZE;t++){
		
		}	
	}
}

void playGame(){
	int gameStatus=0;
	while(gameStatus==0){
		
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
				break;
				
			case 2:
				
				break;
		}
		
		
	}
}






int main(){
	intro();	
	menu();
}
