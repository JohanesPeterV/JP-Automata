#include<stdio.h>
#include<string.h>
#include<time.h>
#include<limits.h>
#include<windows.h>
#include<conio.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
#define SIZE 20
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




class Node{
	public:
	int y;
	int x;
	bool passable;
	Node *parent;
	int gCost;
	int hCost;
	int heapIndex;
	int fCost(){
		return gCost+hCost;
	}
	Node(int y, int x, bool passable){
		this->passable=passable;
		this->y=y;
		this->x=x;
		this->parent=NULL;
	}
	Node(){
		
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
			free(shootedProjectile);
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
				free(tempe);
				i--;
			}
			else{
				if(!tempe->move()){   
					projectiles.erase(projectiles.begin()+i);
					free(tempe);

					i--;
				}
			}
		}
	}
};
class Heap {
	public: 
	Node heap[SIZE*SIZE];
	int heapSize;
	
	
	/*Initialize Heap*/
	Heap() {
	    heapSize = 0;
	    
	}
	
	int size(){
		return heapSize;
	}
	bool contains(Node node){
		return heap[node.heapIndex].y==node.y&&heap[node.heapIndex].x==node.x;
		
	}
	 
	/*In
	sert an element into the heap */
	void insert(Node element) {
	    
		heapSize++;
	    heap[heapSize] = element; 
		element.heapIndex=heapSize;
		/*Insert in the last place*/
	    /*Adjust its position*/
	    int now = heapSize;
	    
	    
	    while (compare(element ,heap[now / 2] )) {
	        heap[now] = heap[now / 2];
	        now /= 2;
	    }
	    heap[now] = element;
	}
	 
	Node deleteMin() {
	    Node minElement, lastElement;

	    int now, child;
	    minElement = heap[1];
	    lastElement = heap[heapSize--];
	    /* now refers to the index at which we are now */
	    for (now = 1; now * 2 <= heapSize; now = child) {
	        /* child is the index of the element which is minimum among both the children */
	        /* Indexes of children are i*2 and i*2 + 1*/
	        child = now * 2;
	        /*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one
	         child */
	         
	        if (child != heapSize && compare( heap[child + 1] , heap[child])) {
	            child++;
	        }
	        /* To check if the last element fits ot not it suffices to check if the last element
	         is less than the minimum element among both the children*/
	        
			//>

			if (compare(heap[child],lastElement)) {
	            heap[now] = heap[child];
	        } else /* It fits there */
	        {
	            break;
	        }
	    }
	    heap[now] = lastElement;
	    return minElement;
	}
	bool compare(Node a, Node b){
		if(a.fCost()==b.fCost()){
			return a.hCost<b.hCost;
		}
		return a.fCost()<b.fCost();
	}
};

class Missile: public Mortal{

	private:

		//A star logic
						
		Node grid[SIZE][SIZE];
		
		Player *target;
		
		
		
		void initNodes(){
			for(int i=0;i<SIZE;i++){
				for(int t=0;t<SIZE;t++){
					grid[i][t]=*new Node(i,t,true);

				}
			}
		}
		
		void findPath(){
			Node startNode=grid[position->y][position->x];
			Node targetNode=grid[target->position->y][target->position->x];
			
			Heap openSet;
			vector<Node> closedSet;			
			openSet.insert(startNode);
					
			
			
			while(openSet.size()>0){
				Node node=openSet.deleteMin();
				int idx=0;
				
				closedSet.push_back(node);
//				printf("Node y: %d, Node x: %d\n",node.y,node.x);
//				printf("Target y: %d, Target x: %d\n",target->position->y,target->position->x);
//				keypress();
				if(node.y==target->position->y&&node.x==target->position->x){		
//					printf("\n%d %d\n", grid[target->position->y][target->position->x].y,grid[target->position->y][target->position->x].x);
//					printf("%d %d\n", grid[target->position->y][target->position->x].parent->y,grid[target->position->y][target->position->x].parent->x);
//					keypress();
					retracePath();
					return;
				}
				
				for(int i=-1;i<=1;i++){
					for(int t=-1;t<=1;t++){
						if(i==0&&t==0)continue;			
						int checkY=i+node.y;
						int checkX=t+node.x;
						if(checkX>=0&&checkX<SIZE&&checkY>=0&&checkY<SIZE){
							Node *neighbour=&grid[checkY][checkX];
							if(!neighbour->passable||contains(closedSet,*neighbour)){
								continue;
							}
							int newCost=node.gCost+getDistance(node,*neighbour);
							if(newCost<neighbour->gCost||!openSet.contains(*neighbour)){
								neighbour->gCost=newCost;
								neighbour->hCost=getDistance(node,*neighbour);
								neighbour->parent=&node;
								grid[checkY][checkX].parent=&grid[node.y][node.x];
								 
								if(!openSet.contains(*neighbour)){	
									openSet.insert(*neighbour);
								}
							}
						}
					}
				}
			}

		}
		
		bool contains(vector<Node> nodeVector, Node node){	
			for(int i=0;i<nodeVector.size();i++){
				Node temp=nodeVector[i];
				if(temp.y==node.y&&temp.x==node.x){
					return true;
				}
			}
			
			
			return false;
		}
		
		void retracePath(){
			Node currNode=grid[target->position->y][target->position->x];


//			for(int i=0;i<SIZE;i++){
//				for(int t=0;t<SIZE;t++){
//					if(grid[i][t].parent)printf("%d,%d ",grid[i][t].parent->y,grid[i][t].parent->x);
//					else printf("");
//				}
//				printf("\n");
//			}
			
			while(currNode.parent->x!=position->x||currNode.parent->y!=position->y){
				currNode=*currNode.parent;
//				printf("\n%d %d\n",currNode.y,currNode.x);	
//				printf("\n%d %d\n",grid[target->position->y][target->position->x].parent->y,grid[target->position->y][target->position->x].parent->x);	
//				keypress();
				
				
			}
//			printf("Sans");

//			if(currNode.y<0-position->y)dirY=-1;
//			if(currNode.y>0-position->y)dirY=1;
//			
//			if(currNode.x<0-position->x)dirX=-1;
//			if(currNode.x>0-position->x)dirX=1;


			if(direction)free(direction);
			direction=newPosition(currNode.y,currNode.x);
//			printf("%d %d\n",currNode.y,currNode.x);
//			
//			printf("\n%d %d\n",direction->x,direction->y);			
			keypress();
		
		}
		
		
		int getDistance(Node a, Node b){
			int dstY=a.y-b.y;
			if(dstY<0)dstY=dstY*-1;
			int dstX=a.x-b.x;
			if(dstX<0)dstX=dstX*-1;
			if(dstX>dstY)return 14*dstY+10*(dstX-dstY); 
			return 14*dstX+10*(dstY-dstX);
		}
		
	public:
	Position *direction;

	char animationSymbol;

	int damage;
	bool homing;
	
	
	Missile(Position *position, Player *target,char symbol, int hp){
			initNodes();
		
		this->hp=1;
		this->position=position;
		this->target=target;
		this->symbol=symbol;
		this->damage=1;
		this->direction=newPosition(1,0);		
	}
	
	bool move(){
		findPath();	
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;
		position->x=direction->x;
		position->y=direction->y;
		int dir=(rand()%2==0?-1:1);
		int dir2=(rand()%2==0?-1:1);
		
		while(map[this->position->y][this->position->x].mortal!=NULL&&map[this->position->y][this->position->x].mortal->symbol=='o'){
			this->position->y=this->position->y+(rand()%2*this->direction->x*dir);			
			this->position->x=this->position->x+(rand()%2*this->direction->y*dir2);
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
	
	bool validate(){ 
		return position->x>=1&&position->y>=1&&position->x<SIZE-1&&position->y<SIZE-1&&map[position->y][position->x].symbol!='#';
	}
	
};
class Enemy: public Mortal{
	public:
	std::vector<Missile*> missiles;	
	Position *direction;
	Player *target;
	
	Enemy(int hp, struct Position *position, char symbol, Player *target,struct Position *direction){
		this->position=position;
		this->hp=hp;
		this->symbol=symbol;
		this->entityObstacle=true;
		map[this->position->y][this->position->x].symbol=symbol;
		map[this->position->y][this->position->x].mortal=this;
		this->target=target;
		this->direction=direction;
		
	}	
	
	bool isAlive(){
		return hp>0;
	} 
	void shoot(){
		Missile *shootedMissile=new Missile(
		newPosition(position->y+direction->y,position->x+direction->x),target,'o',1);
		

		missiles.push_back(shootedMissile);
		
	}	
	void validateMissiles(){
		
		

		for(int i=0;i<missiles.size();i++){
			Missile *tempe=missiles[i];
			if(!tempe->move()){   
				missiles.erase(missiles.begin()+i);
				free(tempe);
				i--;
			}
		
		}
		
		
	}

	void terminated(){
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;
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
	
//	key=realTimePress();
	key=keypress();	
	switch(key){
		case 'w':
			currPlayer->move(-1,0);
			break;
		case 'W':
			currPlayer->move(-1,0);
			break;
		case 'w'+'a':
			currPlayer->move(-1,-1);
			break;
		case 'W'+'A':
			currPlayer->move(-1,-1);
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

vector <Enemy*> *initEnemy(Player *player){
	
	vector<Enemy*> *enemyList=new vector<Enemy*>();
	
	enemyList->push_back(new Enemy(10,newPosition(4,4), 'E',player,newPosition(1,0)));
	
	enemyList->push_back(new Enemy(10,newPosition(4,SIZE-5), 'E',player,newPosition(1,0)));
	
	enemyList->push_back(new Enemy(10,newPosition(SIZE-5,4), 'E',player,newPosition(1,0)));
	
	enemyList->push_back(new Enemy(10,newPosition(SIZE-5,SIZE-5), 'E',player,newPosition(1,0)));
	
	
	return enemyList;
}

int tempFlag=0;
void enemyController(vector <Enemy*> *enemyList, bool blink){
	
	for(int i=0;i<enemyList->size();i++){
		Enemy *tempEnemy=enemyList->at(i);
		if(blink)tempEnemy->validateMissiles();
//		if(rand()%4==0&&blink)tempEnemy->shoot();
		if(tempFlag==0){
			tempEnemy->shoot();
			tempFlag=1;
		}
		if(!tempEnemy->isAlive()){   
			tempEnemy->terminated();
			enemyList->erase(enemyList->begin()+i);
			free(tempEnemy);
			i--;
		}else{
			map[tempEnemy->position->y][tempEnemy->position->x].symbol=tempEnemy->symbol;
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
	vector<Enemy*> *enemyList= initEnemy(currPlayer);
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


