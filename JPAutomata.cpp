#include<stdio.h>
#include<string.h>
#include<time.h>
#include<limits.h>
#include<windows.h>
#include<conio.h>
#include<pthread.h>
#include<iostream>
#include<vector>
#include<math.h>
#include<algorithm>
#define HEIGHT 40
#define WIDTH 80
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE    "\x1b[97m"
#define ANSI_COLOR_GRAY    "\x1b[37m"
#define ANSI_COLOR_LIGHT_RED    "\x1b[91m"
#define ANSI_COLOR_LIGHT_YELLOW    "\x1b[93m"
#define MISSILE 'O'
#define DEADMISSILE '0'
#define ANSI_COLOR_RESET   "\x1b[0m"
#define WALL 219
#define VWALL -37
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

void setConsoleColor(int bgCode, int fgCode){

	WORD wColor = ((bgCode & 0x0F) << 4) + (fgCode & 0x0F);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	
	
//	setbkcolor(1);
//	system("COLOR  F2");
}
using namespace std;
void clear(){
	system("cls");
}
void resetCursor(){
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {0,0};
	SetConsoleCursorPosition(hConsole,pos); 
}
struct Position{
	int y;
	int x;
};
void delay(){
	int milsec = 1;
		clock_t startTime = clock();
		while(clock() < (startTime + milsec)){
			
		}
		
}
void printD(char text[]){
	int len=strlen(text);
	for(int i=0;i<len;i++){
		printf("%c",text[i]);
		delay();
	}
}

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
struct Tile map[HEIGHT][WIDTH]={0};
bool validTile(int y, int x){
	return y<HEIGHT-1&&y>0&&x<WIDTH-1&&x>0&&map[y][x].symbol!=VWALL;
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
void initMap(){
	for(int i=0;i<HEIGHT;i++){
		for(int t=0;t<WIDTH;t++){
			map[i][t].symbol=' ';
			map[i][t].mortal=NULL;
		}
	}
	for(int i=0;i<HEIGHT;i++){
		map[i][0].symbol=WALL;
		map[i][WIDTH-1].symbol=WALL;
	}
	for(int i=0;i<WIDTH;i++){
		map[0][i].symbol=WALL;
		map[HEIGHT-1][i].symbol=WALL;
	}
		
}
void initObstacles(){
	
	for(int i=0;i<30;i++){
		
		int yCor=(rand()%(HEIGHT-3))+2;
		int xCor=(rand()%(WIDTH-3))+2;
		while(map[yCor][xCor].symbol!=' '){
			yCor=(rand()%(HEIGHT-3))+2;
			xCor=(rand()%(WIDTH-3))+2;
		}
		map[yCor][xCor].symbol=WALL;
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
		else if(validTile(position->y,position->x)&&this->hp>0){
			map[position->y][position->x].symbol=this->symbol;
			map[position->y][position->x].mortal=this;
					
			return true;	
		}

		return false;
	}
	bool validate(){ 
		
		
		return !validTile(position->y,position->x);
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
		this->heapIndex=0;
		gCost=0;
		hCost=0;
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
		
		if(!validTile(position->y+y,position->x+x)||(map[this->position->y+y][this->position->x+x].mortal&&map[this->position->y+y][this->position->x+x].mortal->entityObstacle)){
			map[this->position->y][this->position->x].symbol=this->symbol;
			map[this->position->y][this->position->x].mortal=this;
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
	Node *heap[HEIGHT*WIDTH];
	int heapSize=1;
	
	Heap() {
	    heapSize = 1;
	}
	
	int size(){
		return heapSize;
	}
	bool contains(Node node){
//		printf("%d\n",node.heapIndex);
//		printf("\n%d %d",heap[node.heapIndex]->y,heap[node.heapIndex]->x);
//		printf("\n%d %d\n",node.y,node.x);
		return heap[node.heapIndex]&&heap[node.heapIndex]->y==node.y&&heap[node.heapIndex]->x==node.x;
		
	}
	 
	/*In
	sert an element into the heap */
	void insert(Node *element) {
//	    printf("%d %d %d\n",element->gCost,element->hCost,element->fCost());
//	    keypress();	    		

		heap[heapSize] = element;
		element->heapIndex=heapSize;
		upHeap(heapSize);
		heapSize++;
		
	}

	 void upHeap(int index){
		if(index <= 1){
			return;
		}
		if (compare(heap[index] , heap[index / 2] ))
		{
			//tuker data
			Node *temp = heap[index];
			heap[index] = heap[index / 2];
			heap[index / 2] = temp;
			
			int tempIdx=heap[index]->heapIndex;
			heap[index]->heapIndex = heap[index / 2]->heapIndex;
			heap[index / 2]->heapIndex = tempIdx;
			
			
			upHeap(index / 2);
		}
	}
	Node deleteMin() {
		Node *root = heap[1];
		heap[1] = heap[heapSize - 1];
		heap[1]->heapIndex=1;
		heapSize--;
		downHeap(1);
		
		root->heapIndex=0;
//		printf("\nMin: %d %d",root->y,root->x);
//		keypress();
		return *root;
	    

	}
	
	void updateItem(Node *node){
		upHeap(node->heapIndex);
	}
	void downHeap(int index)
	{
		// Punya 2 anak
		if (index * 2 + 1 < heapSize)
		{
			// Has a child that is larger than parent
			if (compare(heap[index * 2], heap[index] ) || compare( heap[index * 2 + 1],heap[index] ))
			{
				// Select the smallest
				// Left child is smaller
				if (compare(heap[index * 2],heap[index * 2 + 1] ))
				{
					Node *temp = heap[index * 2];
					heap[index * 2] = heap[index];
					heap[index] = temp;
					
					int tempIdx=heap[index * 2]->heapIndex;
					heap[index * 2]->heapIndex=heap[index]->heapIndex;
					heap[index]->heapIndex=tempIdx;
					
					downHeap(index * 2);
				}
				else // Right child is smaller
				{
					Node * temp = heap[index * 2 + 1];
					heap[index * 2 + 1] = heap[index];
					heap[index] = temp;
					
					int tempIdx=heap[index * 2 + 1]->heapIndex;
					heap[index * 2 + 1]->heapIndex=heap[index]->heapIndex;
					heap[index]->heapIndex=tempIdx;
					
					downHeap(index * 2 + 1);
				}
			}
		}
		// Hanya punya 1 anak
		else if (index * 2< heapSize) 
		{
			if (compare(heap[index * 2],heap[index]  ))
			{
				Node *temp = heap[index];
				heap[index] = heap[index * 2];
				heap[index * 2] = temp;
				
				
				int tempIdx=heap[index * 2 + 1]->heapIndex;
				heap[index * 2 + 1]->heapIndex=heap[index]->heapIndex;
				heap[index]->heapIndex=tempIdx;
			}
		}
	}
	bool compare(Node a, Node b){
		if(a.fCost()==b.fCost()){
			return a.hCost<b.hCost;
		}
//		return a.fCost()<b.fCost();
		return a.hCost<b. hCost;
	}
	bool compare(Node *a, Node *b){
		
		if(a->fCost()==b->fCost()){
			return a->hCost<b->hCost;
		}
//		return a->fCost()<b->fCost();
		return a->fCost()<b->fCost();
		
	}
	
};

class Missile: public Mortal{

	private:
		bool homing;

		//A star logic
						
		Node grid[HEIGHT][WIDTH];
		
		Player *target;
		
		
		
		void initNodes(){
			for(int i=1;i<HEIGHT-1;i++){
				for(int t=1;t<WIDTH-1;t++){
					
					grid[i][t]=*new Node(i,t,map[i][t].symbol!=VWALL&&map[i][t].symbol!='E');
					
				}
			}
//			for(int i=0;i<SIZE;i++){
//				grid[i][SIZE-1]=*new Node(i,SIZE-1,false);
//				grid[i][0]=*new Node(i,0,false);
//				grid[0][i]=*new Node(0,i,false);
//				grid[SIZE-1][i]=*new Node(SIZE-1,i,false);
//					
//			}
		}
		
		void clearNodes(){
			for(int i=1;i<HEIGHT-1;i++){
				for(int t=1;t<WIDTH-1;t++){

					grid[i][t].passable=map[i][t].symbol!=VWALL&&map[i][t].symbol!='E';
					grid[i][t].gCost=0;
					grid[i][t].hCost=0;
					
				}
			}
		}
		void findPath(){
			initNodes();
			Node targetNode=grid[target->position->y][target->position->x];
			
			Heap openSet=*new Heap();
			vector<Node> closedSet;			
			openSet.insert(&grid[position->y][position->x]);
					
			while(openSet.size()>1){
				Node node=openSet.deleteMin();
				int idx=0;
				closedSet.push_back(node);
			
			
//				printf("Target y: %d, Target x: %d\n",target->position->y,target->position->x);
//				printf("Node y: %d, Node x: %d\n",node.y,node.x);
//				keypress();
//				printf("Start");	
				if(node.y==target->position->y&&node.x==target->position->x){	
//									printf("balik\n");

					retracePath();
					return;   
				}
//				printf("End\n");
				for(int i=-1;i<=1;i++){

					for(int t=-1;t<=1;t++){

						
						if(i==0&&t==0)continue;			
						int checkY=i+node.y;
						int checkX=t+node.x;
			

						if(checkX>=0&&checkX<WIDTH&&checkY>=0&&checkY<HEIGHT){
							Node *neighbour=&grid[checkY][checkX];
							
							if(!neighbour->passable||contains(closedSet,*neighbour)){
								continue;
							}
										
							int newCost=node.gCost+getDistance(node,*neighbour);

							if(newCost<neighbour->gCost||!openSet.contains(*neighbour)){
								neighbour->gCost=newCost;
								neighbour->hCost=getTargetDistance(*neighbour);
								neighbour->parent=&node;
								grid[checkY][checkX].parent=&grid[node.y][node.x];
								
//								printf("%d",neighbour->heapIndex);
								if(!openSet.contains(*neighbour)){
//									printf("Insert Start");	
			
									openSet.insert(neighbour);
//									printf("Insert End\n");
									
								}else{
									openSet.updateItem(neighbour);
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

			int distance=0;
			
			while(currNode.parent&&(currNode.parent->x!=position->x||currNode.parent->y!=position->y)){
				
				currNode=*currNode.parent;
				
				distance++;
				
//				printf("\n%d %d\n",currNode.y,currNode.x);	
//				printf("\n%d %d\n",grid[target->position->y][target->position->x].parent->y,grid[target->position->y][target->position->x].parent->x);	
			}
//			printf("Sans");

//			if(currNode.y<0-position->y)dirY=-1;
//			if(currNode.y>0-position->y)dirY=1;
//			
//			if(currNode.x<0-position->x)dirX=-1;
//			if(currNode.x>0-position->x)dirX=1;

				
			if(direction)free(direction);
			direction=newPosition(currNode.y-position->y,currNode.x-position->x);

//			direction=newPosition(currNode.y,currNode.x);
//			if(&currNode){
// 
//			}
				
			
			
			if(distance<5){
				homing=false;
				symbol=DEADMISSILE;
			}
//			printf("%d %d\n",currNode.y,currNode.x);
//			
//			printf("\n%d %d\n",direction->x,direction->y);			
			
		
		}
		
		
		int getDistance(Node a, Node b){
			int dstY=a.y-b.y;
			if(dstY<0)dstY=dstY*-1;
			
			int dstX=a.x-b.x;
			if(dstX<0)dstX=dstX*-1;

			if(dstX>dstY)return 14*dstY+10*(dstX-dstY);
			return 14*dstX+10*(dstY-dstX);                                                             

		}
		int getTargetDistance(Node a){
			int dstY=a.y-target->position->y;
			if(dstY<0)dstY=dstY*-1;

			int dstX=a.x-target->position->x;
			if(dstX<0)dstX=dstX*-1;
			
			if(dstX>dstY)return 14*dstY+10*(dstX-dstY);
			return 14*dstX+10*(dstY-dstX);                                                             
		}
	public:
	Position *direction;

	char animationSymbol;

	int damage;
	
	
	Missile(Position *position, Player *target,char symbol, int hp){
		this->homing=true;
		this->hp=1;
		this->position=position;
		this->target=target;
		this->symbol=symbol;
		this->damage=1;
		this->direction=newPosition(1,0);		
	}
	
	bool move(){

		if(homing){
//			std::thread thread_object(findPath);

			findPath();
		}
		if(hp<0)return false;
		map[position->y][position->x].symbol=' ';
		map[position->y][position->x].mortal=NULL;
//		position->x=direction->x;
//		position->y=direction->y;
		position->x=position->x+direction->x;
		position->y=position->y+direction->y;
//		int dir=(rand()%2==0?-1:1);
//		int dir2=(rand()%2==0?-1:1);
//		while(map[this->position->y][this->position->x].mortal!=NULL&&map[this->position->y][this->position->x].mortal->symbol=='o'){
//			this->position->y=this->position->y+(rand()%2*this->direction->x*dir);			
//			this->position->x=this->position->x+(rand()%2*this->direction->y*dir2);			
//		}
		
		if(map[position->y][position->x].mortal){
			map[position->y][position->x].mortal->hp=map[position->y][position->x].mortal->hp-damage;
			if(map[position->y][position->x].mortal->hp<=0){
				map[position->y][position->x].mortal=NULL;
				map[position->y][position->x].symbol=' ';
			}
			return false;
		}
		
		else if(validTile(position->y,position->x)&&this->hp>0){
			map[position->y][position->x].symbol=this->symbol;
			map[position->y][position->x].mortal=this;
			return true;
		}
		return false;
		
	}	
	bool validate(){ 
		return position->x>=1&&position->y>=1&&position->x<WIDTH-1&&position->y<HEIGHT-1&&map[position->y][position->x].symbol!=WALL;
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
	
	
	void shoot(){
		Missile *shootedMissile=new Missile(
		newPosition(position->y+direction->y,position->x+direction->x),target,MISSILE,1);
		

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
		for(int i=0;i<missiles.size();i++){
			Missile *tempe=missiles[i];

			tempe->symbol=DEADMISSILE;
		
		
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
	int milsec = 10;
		int ch=-1;
			int hit = kbhit();
			if (hit){	
				ch = getch();
				return ch;	




			}
		
}









void printMap(Player *currPlayer){
	resetCursor();
	for(int i=0;i<HEIGHT;i++){
		for(int t=0;t<WIDTH;t++){
			switch(map[i][t].symbol){
				case MISSILE:
					printf(ANSI_COLOR_LIGHT_RED "%c" ,map[i][t].symbol);
					break;
				case DEADMISSILE:
					printf(ANSI_COLOR_BLUE "%c" ,map[i][t].symbol);
					break;
				case '*':
					printf(ANSI_COLOR_LIGHT_YELLOW "%c" ,map[i][t].symbol);
					break;
				case 'E':
					printf(ANSI_COLOR_GRAY "%c" ,map[i][t].symbol);
					break;
				case -37:
					printf(ANSI_COLOR_GRAY "%c",map[i][t].symbol);
					break;
				default: 
					printf(ANSI_COLOR_WHITE"%c",map[i][t].symbol);
					break;		
			}
		}
		printf("\n");
	}
	setConsoleColor(0,15);
	printf("hp: %d",currPlayer->hp);
}


int playerController(Player *currPlayer,int gameStatus){
	if(gameStatus==0&&currPlayer->hp<=0)return -1;
	currPlayer->shoot();

	currPlayer->validateProjectiles();
	
	int key=-1;
	
	key=realTimePress();
//	key=keypress();	
	
	switch(key){
		case 'w':
			
			currPlayer->move(-1,0);
			break;
		case 'W':
			currPlayer->move(-1,0);
			break;
//		case 'w'+'a':
//			currPlayer->move(-1,-1);
//			break;
//		case 'W'+'A':
//			currPlayer->move(-1,-1);
//			break;
//		case 'W'+'a':
//			currPlayer->move(-1,-1);
//			break;
//			
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
//		case 'S'+'a':
//			currPlayer->move(1,-1);
//			break;
//		case 'S'+'A':
//			currPlayer->move(1,-1);
//			break;
//		case 's'+'a':
//			currPlayer->move(1,-1);
//			break;
//			
		case 'd':
			currPlayer->move(0,1);
			break;
		case 'D':
			currPlayer->move(0,1);
			break;
//		case 'D'+'s':
//			currPlayer->move(1,1);
//			break;
//		case 'D'+'S':
//			currPlayer->move(1,1);
//			break;
//		case 'd'+'s':
//			currPlayer->move(1,1);
//			break;
//		case 'w'+'d':
//			currPlayer->move(-1,1);
//		break;
//		case 'W'+'d':
//			currPlayer->move(-1,1);
//		break;
//		case 'W'+'D':
//			currPlayer->move(-1,1);
//		break;

		case 'i':
			currPlayer->face(-1,0,30);
			break;
		case 'I':
			currPlayer->face(-1,0,30);
			break;
//		case 'j'+'i':
//			currPlayer->face(-1,-1,'<');
//			break;
//		case 'J'+'i':
//			currPlayer->face(-1,-1,'<');
//			break;
//		case 'J'+'I':
//			currPlayer->face(-1,-1,'<');
//			break;
		case 'j':
			currPlayer->face(0,-1,17);
			break;
		case 'J':
			currPlayer->face(0,-1,17);
			break;
		case 'k':
			currPlayer->face(1,0,31);
			break;
		case 'K':
			currPlayer->face(1,0,31);
			break;
//		case 'K'+'j':
//			currPlayer->face(1,-1,'v');
//			break;
//		case 'K'+'J':
//			currPlayer->face(1,-1,'v');
//			break;
//		case 'k'+'j':
//			currPlayer->face(1,-1,'v');
//			break;
		case 'l':
			currPlayer->face(0,1,16);
			break;
		case 'L':
			currPlayer->face(0,1,16);
			break;
		
//		case 'l'+'k':
//			currPlayer->face(1,1,'v');
//			break;
//		case 'L'+'k':
//			currPlayer->face(1,1,'v');
//			break;
//		case 'l'+'K':
//			currPlayer->face(1,1,'v');
//			break;	
		default:
			currPlayer->move(0,0);
	}	
	return gameStatus;
}

vector <Enemy*> *initEnemy(Player *player){
	
	vector<Enemy*> *enemyList=new vector<Enemy*>();
	int enemyCount=10+(rand()%5);
	for(int i=0;i<enemyCount;i++){
		
		int yCor=4+(rand()%4);
		int xCor=(rand()%(WIDTH-2))+1;
		while(map[yCor][xCor].mortal!=NULL){
			yCor=4+(rand()%4);
			xCor=(rand()%(WIDTH-2))+1;
		}
		enemyList->push_back(new Enemy(10,newPosition(yCor,xCor), 'E',player,newPosition(1,0)));		
	}
	return enemyList;
}

int tempFlag=0;
int enemyController(vector <Enemy*> *enemyList, bool blink,int gameStatus){
	if(enemyList->size()==0)return 1;
	for(int i=0;i<enemyList->size();i++){
		
		Enemy *tempEnemy=enemyList->at(i);
		if(tempEnemy->hp<=0){   
			tempEnemy->terminated();
			enemyList->erase(enemyList->begin()+i);
			free(tempEnemy);
			i--;
			continue;
		}
		if(blink)tempEnemy->validateMissiles();
		if(rand()%10==0&&blink)tempEnemy->shoot();
//		if(tempFlag==0){
//			tempEnemy->shoot();
//			tempFlag=1;
//		}
		
		map[tempEnemy->position->y][tempEnemy->position->x].symbol=tempEnemy->symbol;
			
	}
	return gameStatus;
	
}
void printFirstLost(){
	clear();
    printD("                   .. ......................................''..............                     \n" );  
	printD("              ..  ... ......................................:c'.................                 \n");  
	printD(" ....         ..   ..  ........ ....  ......................................   .    .            \n");  
	printD(".....   ............. ...........................................................  .... .        \n");  
	printD("..      ......... ... ...........................................................  ........   .  \n");  
	printD("     . ........................................................................... ........   ...\n");  
	printD(".. ...........''.................................................................................\n");  
	printD("....................''','........................................................................\n");  
	printD("....................,;:::;;;;,''.................................................................\n");  
	printD("........................',;;;,,,,,',,,'..........................................................\n");  
	printD("...............................'''',,,,''''......................................................\n");  
	printD(".......,'...............................'','....................''...............................\n");  
	printD("......,c;......              _                                                     ,.............\n");  
	printD(".......'.......           | |_)   o    /\\     _|_  _  ._ _   _. _|_  _.            ;.............\n");  
	printD(".........'....'         \\_| |     o   /--\\ |_| |_ (_) | | | (_|  |_ (_|            :.............\n");  
	printD("'...'''.','''''                                                                    ;.............\n");  
	printD(",,,,,,,,,,,,'',,;,'....''....',,''',;'',clldxdl:;:::::c:;;c:;,;,,;,'';;,,;',,'','','''...........\n");  
	printD(";;;:::;;;;,,,,,,,,''......                                          ::::;;;;;;,,,,,'''''''''''''.\n");  
	printD("llooddooxdc:;;::::,'''''''                                          OOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printD("dooodxxxxocc::cllc;,,,,,,,    57 S failed to hack the machine,      kOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printD("olc::::::cc:;;;;;,,,,,;;;;    resulting failure in his attempt on   ollcoxxxdxollodllllcclddoc:::\n");  
	printD("lcc:cllllooooolc:;:;;;::;:    defeating a machine, resulting world  :::cldllllcclcccc:cccclolcccd\n");  
	printD("llllclollcclooddloolc:::c:    destruction and android extinction.   ;;;;::::;;;:::::;:cc:::colclx\n");  
	printD("oddolcc::::lodddddddddllcc                                          ;;;;;;::;;;;:::::clc:::clc::l\n");  
	printD("lolllcccccllodolcclodoc:cl         In Honesty and Hardwork,         ;;::::colc::lllccccc::clollll\n");  
	printD("clllllloodxdoool:;:cooc:ld         We Strive for Excellence         c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printD("looooooodddolc:clloolclccl                                          ;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printD("oolccloodolccccccllolllc::          Press Any Button                '''..','''''........';::;,,;;\n");  
	printD(";;:c:cccllooollc::cclolc::                                          '''....'.............'...';:,\n");  
	printD(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printD(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printD("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printD(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
	//Preas Any Button
//	if(blink)printf("_");
//	else printf(" ");
}

void printLost(bool blink){
	clear();
    printf("                   .. ......................................''..............                     \n" );  
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
	printf(";;;:::;;;;,,,,,,,,''......                                          ::::;;;;;;,,,,,'''''''''''''.\n");  
	printf("llooddooxdc:;;::::,'''''''                                          OOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printf("dooodxxxxocc::cllc;,,,,,,,    57 S failed to hack the machine,      kOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printf("olc::::::cc:;;;;;,,,,,;;;;    resulting failure in his attempt on   ollcoxxxdxollodllllcclddoc:::\n");  
	printf("lcc:cllllooooolc:;:;;;::;:    defeating a machine, resulting world  :::cldllllcclcccc:cccclolcccd\n");  
	printf("llllclollcclooddloolc:::c:    destruction and android extinction.   ;;;;::::;;;:::::;:cc:::colclx\n");  
	printf("oddolcc::::lodddddddddllcc                                          ;;;;;;::;;;;:::::clc:::clc::l\n");  
	printf("lolllcccccllodolcclodoc:cl         In Honesty and Hardwork,         ;;::::colc::lllccccc::clollll\n");  
	printf("clllllloodxdoool:;:cooc:ld         We Strive for Excellence         c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printf("looooooodddolc:clloolclccl                                          ;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printf("oolccloodolccccccllolllc::          Press Any Button");
	if(blink)printf("_");
	else printf(" ");
	printf("               '''..','''''........';::;,,;;\n");  
	printf(";;:c:cccllooollc::cclolc::                                          '''....'.............'...';:,\n");  
	printf(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printf(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printf("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printf(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
	//Press Any Button
//	if(blink)printf("_");
//	else printf(" ");
}
void lost(){
	printFirstLost();
	bool blink=true;
	int input=-1;
	while(input==-1){
		blink=!blink;
		printLost(blink);
		input=waitKeyPress(8);
	}
}


void printFirstWin(){
		clear();
    printD("                   .. ......................................''..............                     \n" );  
	printD("              ..  ... ......................................:c'.................                 \n");  
	printD(" ....         ..   ..  ........ ....  ......................................   .    .            \n");  
	printD(".....   ............. ...........................................................  .... .        \n");  
	printD("..      ......... ... ...........................................................  ........   .  \n");  
	printD("     . ........................................................................... ........   ...\n");  
	printD(".. ...........''.................................................................................\n");  
	printD("....................''','........................................................................\n");  
	printD("....................,;:::;;;;,''.................................................................\n");  
	printD("........................',;;;,,,,,',,,'..........................................................\n");  
	printD("...............................'''',,,,''''......................................................\n");  
	printD(".......,'...............................'','....................''...............................\n");  
	printD("......,c;......              _                                                     ,.............\n");  
	printD(".......'.......           | |_)   o    /\\     _|_  _  ._ _   _. _|_  _.            ;.............\n");  
	printD(".........'....'         \\_| |     o   /--\\ |_| |_ (_) | | | (_|  |_ (_|            :.............\n");  
	printD("'...'''.','''''                                                                    ;.............\n");  
	printD(",,,,,,,,,,,,'',,;,'....''....',,''',;'',clldxdl:;:::::c:;;c:;,;,,;,'';;,,;',,'','','''...........\n");  
	printD(";;;:::;;;;,,,,,,,,''......                                          ::::;;;;;;,,,,,'''''''''''''.\n");  
	printD("llooddooxdc:;;::::,'''''''                                          OOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printD("dooodxxxxocc::cllc;,,,,,,,    57 S succesfully hack the machine,    kOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printD("olc::::::cc:;;;;;,,,,,;;;;    resulting peace for all the android   ollcoxxxdxollodllllcclddoc:::\n");  
	printD("lcc:cllllooooolc:;:;;;::;:    and safety to the world.              :::cldllllcclcccc:cccclolcccd\n");  
	printD("llllclollcclooddloolc:::c:                                          ;;;;::::;;;:::::;:cc:::colclx\n");  
	printD("oddolcc::::lodddddddddllcc                                          ;;;;;;::;;;;:::::clc:::clc::l\n");  
	printD("lolllcccccllodolcclodoc:cl         In Honesty and Hardwork,         ;;::::colc::lllccccc::clollll\n");  
	printD("clllllloodxdoool:;:cooc:ld         We Strive for Excellence         c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printD("looooooodddolc:clloolclccl                                          ;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printD("oolccloodolccccccllolllc::          Press Any Button                '''..','''''........';::;,,;;\n");  
	printD(";;:c:cccllooollc::cclolc::                                          '''....'.............'...';:,\n");  
	printD(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printD(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printD("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printD(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
}
void printWin(bool blink){
	clear();
    printf("                   .. ......................................''..............                     \n" );  
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
	printf(";;;:::;;;;,,,,,,,,''......                                          ::::;;;;;;,,,,,'''''''''''''.\n");  
	printf("llooddooxdc:;;::::,'''''''                                          OOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printf("dooodxxxxocc::cllc;,,,,,,,    57 S succesfully hack the machine,    kOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printf("olc::::::cc:;;;;;,,,,,;;;;    resulting peace for all the android   ollcoxxxdxollodllllcclddoc:::\n");  
	printf("lcc:cllllooooolc:;:;;;::;:    and safety to the world.              :::cldllllcclcccc:cccclolcccd\n");  
	printf("llllclollcclooddloolc:::c:                                          ;;;;::::;;;:::::;:cc:::colclx\n");  
	printf("oddolcc::::lodddddddddllcc                                          ;;;;;;::;;;;:::::clc:::clc::l\n");  
	printf("lolllcccccllodolcclodoc:cl         In Honesty and Hardwork,         ;;::::colc::lllccccc::clollll\n");  
	printf("clllllloodxdoool:;:cooc:ld         We Strive for Excellence         c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printf("looooooodddolc:clloolclccl                                          ;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printf("oolccloodolccccccllolllc::          Press Any Button");
	if(blink)printf("_");
	else printf(" ");
	printf("               '''..','''''........';::;,,;;\n");  
	printf(";;:c:cccllooollc::cclolc::                                          '''....'.............'...';:,\n");  
	printf(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printf(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printf("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printf(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
	//Press Any Button
//	if(blink)printf("_");
//	else printf(" ");
}
void win(){
	printFirstWin();
	bool blink=true;
	int input=-1;
	while(input==-1){
		blink=!blink;
		printWin(blink);
		input=waitKeyPress(8);
	}
}
void playGame(){
	adjustWindowSize(1000,1000);
			//14 krem
	//8 abu abu 
	//7 abu abu muda
	setConsoleColor(0,15);

	clear();
	initMap();
	int gameStatus=0;
	bool blink=true;
	Player *currPlayer=new Player(3, newPosition(HEIGHT-4,WIDTH/2),newPosition(-1,0),30);
	vector<Enemy*> *enemyList= initEnemy(currPlayer);
	setConsoleColor(7,14);
	initObstacles();
	while(gameStatus==0){
		
		printMap(currPlayer);
		gameStatus=enemyController(enemyList,blink,gameStatus);
		gameStatus=playerController(currPlayer,gameStatus);		


		blink=!blink;
		
	}	
	if(gameStatus==-1){
		lost();
	}
	if(gameStatus==1){
		win();
	}
//	while(realTimePress()!=-1);
}

void printIntro(bool blink){
	resetCursor();
    printf("                   .. ......................................''..............                     \n" );  
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
	resetCursor();
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
void printAbout(bool blink){
	clear();
    printf("                   .. ......................................''..............                     \n" );  
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
	printf(";;;:::;;;;,,,,,,,,''......                                          ::::;;;;;;,,,,,'''''''''''''.\n");  
	printf("llooddooxdc:;;::::,'''''''                                          OOkkkxdxxollc::;;;;:::;;,,,''\n");  
	printf("dooodxxxxocc::cllc;,,,,,,,    JP Automata is a shooting game. In    kOOxxOOOxxkkxkkollodxxolcc:;,\n");  
	printf("olc::::::cc:;;;;;,,,,,;;;;    this game, you will be playing as     ollcoxxxdxollodllllcclddoc:::\n");  
	printf("lcc:cllllooooolc:;:;;;::;:    57 S, who will be hacking a machine.  :::cldllllcclcccc:cccclolcccd\n");  
	printf("llllclollcclooddloolc:::c:    Help 57 S hack through the machine.   ;;;;::::;;;:::::;:cc:::colclx\n");  
	printf("oddolcc::::lodddddddddllcc                                          ;;;;;;::;;;;:::::clc:::clc::l\n");  
	printf("lolllcccccllodolcclodoc:cl         In Honesty and Hardwork,         ;;::::colc::lllccccc::clollll\n");  
	printf("clllllloodxdoool:;:cooc:ld         We Strive for Excellence         c:;:;;;:::ccc:;;,,,;::clllloo\n");  
	printf("looooooodddolc:clloolclccl                                          ;,,;,,;;,;::;,''''''';;:llcc:\n");  
	printf("oolccloodolccccccllolllc::          Press Any Button");
	if(blink)printf("_");
	else printf(" ");
	printf("               '''..','''''........';::;,,;;\n");  
	printf(";;:c:cccllooollc::cclolc::                                          '''....'.............'...';:,\n");  
	printf(";;;;::;;:lddollllccllllllllc:;;;,,,,,,;:::;;,,,;,,,,,;;;,;:::cc::cc:;,'.''',,,,'......''..',;;;,.\n");  
	printf(",,;;::;,,;;;:cccllccclllcc::;,;;,;lc;;;;c:cooldl:;;ll;cc,;clc:cloodo:''';:,,:;,''''',;::,..''....\n");  
	printf("..'''''''....'',::;;;;,,'''',;;;,:cc;,,,:cccc::;,,,;;,,;'',,''',;;;,'''',;,,;;'';;;,'............\n");  
	printf(".......'''.......';c;'.....'';:,,;:;,'',:cccc:,',,,,,,'''.........''',,,''''...,:;...............\n");  
	//Press Any Button
//	if(blink)printf("_");
//	else printf(" ");
}
void about(){
	bool blink=true;
	int input=-1;
	while(input==-1){
		blink=!blink;
		printAbout(blink);
		input=waitKeyPress(8);
	}
}
void menu(){
		//14 krem
	//8 abu abu 
	//7 abu abu muda
	setConsoleColor(0,15);
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
				clear();
				break;
				
			case 2:
				about();
				clear();
				break;
		}
		
		
	}
}






int main(){
	srand(time(NULL));
	adjustWindowSize(1000,600);
	//0 hitam 
		//14 krem
	//8 abu abu 
	//7 abu abu muda
	setConsoleColor(0,15);
	clear();
	intro();	
	menu();
}


