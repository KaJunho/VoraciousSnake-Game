#include<iostream>
#include<windows.h>
#include<deque>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<string>

using namespace std;

void PrintMap(){
	int i, j;
	
	for(i = 0; i < 30; i++){
		for(j = 0; j < 60; j++){
			if(i == 0 | i == 29) {
				cout << '-';
			}
			else if(j == 0 | j == 59){
				cout << '|';
			}
			else{
				cout << ' ';
			}	
		}
		cout << endl;
	} 
}

void gotoxy(int x, int y){
	COORD pos;
	pos = {x, y};
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut,pos); 
}

void HideCursor() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false; 
	SetConsoleCursorInfo(handle, &CursorInfo);
}

bool searchdeque(deque<int> a, int n){
	int i;
	bool indicator = 0;
	for( i = 0 ; i < a.size() && indicator != 1; i++ ){
		if(a[i] == n){
			indicator = 1;
		}
	}
	return indicator;
}

class Snake {
	private:
		char head = '@';
		char body = '*'; 
		char food = '#';
		string direction;
		int length = 2;
		int pos_food[1][2];
		int score = 0;
		int head_x = 20;
		int head_y = 20;
		
	public:	
		deque<int> snake_x;  //x coordinate of snake's head and body
		deque<int> snake_y;  //y coordinate
		bool gameover = 0;
	
	public:
		void InitPrint();
		void PrintSnake();
		void MoveUp();        // move 1 step each time;
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		void ClearSnake();
		void CreateFood();
		void UserInput();    //get the direction from keyboard
		void JudgeDir();     // judge the direction of the snake and then move 1 step
		void CheckFood();      //Check if the food was eaten 
		void CheckCrush();     //Check if the snake crush wall; 
		bool WrongLocation();
		void AddTail();
		void ShowScore();
		void Gameover();
};


void Snake::InitPrint(){
	HideCursor();
	snake_x.push_back(20);
	snake_x.push_back(20);
	snake_y.push_back(20);
	snake_y.push_back(21);     // set head(20, 20), body(20, 21)
    PrintSnake();
    CreateFood();
}

void Snake::PrintSnake(){
	HideCursor();
	int i;
	for(i = 0; i < length; i++){
		if(i == 0){
			gotoxy(snake_x[i], snake_y[i]);
			cout << head;
		}
		else {
			gotoxy(snake_x[i], snake_y[i]);
			cout << body;
		}
	}
}

void Snake::MoveUp(){
	int h_x, h_y;
	h_x = snake_x.front();
	h_y = snake_y.front() - 1;
	snake_x.push_front(h_x);
	snake_y.push_front(h_y);  //add a new head(x, y-1); (20, 19)
	ClearSnake();
	PrintSnake();
}

void Snake::MoveLeft(){
    int h_x, h_y;
    h_x = snake_x.front() - 1;
    h_y = snake_y.front();
    snake_x.push_front(h_x);
    snake_y.push_front(h_y);  //add a new head(x-1, y);(19, 20)
    ClearSnake();              //judge if the snake find the food, if so don't clear its tail
    PrintSnake();
}

void Snake::MoveDown(){
	int h_x, h_y;
	h_x = snake_x.front();
	h_y = snake_y.front() + 1;
	snake_x.push_front(h_x);
	snake_y.push_front(h_y);  //add a new head(x, y+1); (20, 21)
	ClearSnake();
	PrintSnake();
}

void Snake::MoveRight(){
	int h_x, h_y;
	h_x = snake_x.front() + 1;
	h_y = snake_y.front();
	snake_x.push_front(h_x);
	snake_y.push_front(h_y);  //add a new head(x, y-1); (21, 20)
	ClearSnake();
	PrintSnake();
}

void Snake::ClearSnake(){
	HideCursor();
	int i;
	for(i = 1; i <= length; i++){
		gotoxy(snake_x[i], snake_y[i]);
		cout << ' ';
	}
}

void Snake::CreateFood(){
	do{       
	    srand(time(NULL));
	    pos_food[0][0] = rand() % 59;    // %59 0~58
	    pos_food[0][1] = rand() % 29;
	}
	while(WrongLocation());
	
	gotoxy(pos_food[0][0], pos_food[0][1]);
	cout << food;
}

void Snake::UserInput(){
	char ch;
	ch = getch();
	
	switch(ch)
	{
		case 'a': 
		    if(direction != "right"){
		    	direction = "left";
			} 
			break;
			
		case 's': 
		    if(direction != "up") {
		    	direction = "down";
			}
			break;
			
		case 'd': 
		    if(direction != "left") {
		    	direction = "right";
			} 
			break;
			
		case 'w': 
		    if(direction != "down") {
		    	direction = "up";
			}
			break;
			
		case 'o': 
		    Gameover();
		    break;
	}
}

void Snake::JudgeDir(){
	if(direction == "up"){
		MoveUp();
	}
	else if(direction == "down"){
		MoveDown();
	} 
	else if(direction == "right"){
		MoveRight();
	} 
	else if(direction == "left"){ 
	    MoveLeft();
	}
	head_x = snake_x[0];
	head_y = snake_y[0];
	CheckFood();
	CheckCrush();
}

void Snake::CheckFood(){
	if(head_x == pos_food[0][0] && head_y == pos_food[0][1]){
		score = score + 100;
		CreateFood();
		AddTail();
	}
} 

void Snake::CheckCrush(){
	bool a1, a2, a3, a4;
	a1 = searchdeque(snake_x, 0);
	a2 = searchdeque(snake_x, 59);
	a3 = searchdeque(snake_y, 0);
	a4 = searchdeque(snake_y, 29);
	if( a1 || a2 || a3 || a4){
		Gameover();
	}
}

void Snake::AddTail(){
	length = length + 1;
	if(direction == "up"){
		snake_x.push_back(snake_x.back());
		snake_y.push_back(snake_y.back() + 1);
	}
	else if(direction == "down"){
		snake_x.push_back(snake_x.back());
		snake_y.push_back(snake_y.back() - 1);
	}
	else if(direction == "left"){
		snake_x.push_back(snake_x.back() + 1);
		snake_y.push_back(snake_y.back());
	}
	else if(direction == "right"){
		snake_x.push_back(snake_x.back() - 1);
		snake_y.push_back(snake_y.back());
	}
}

bool Snake::WrongLocation(){
	bool wrong;
	if(searchdeque(snake_x, pos_food[0][0]) && searchdeque(snake_y, pos_food[0][1])){
		wrong = 1;
	}
	else if(pos_food[0][0] == 0 || pos_food[0][1] == 0){
		wrong = 1;
	} 
	else {
		wrong = 0;
	}
	return wrong;
}

void Snake::ShowScore(){
	cout << "Your score is " << score << endl;
}

void Snake::Gameover(){
    gameover = 1;
}


int main(){
	
	Snake snake;
	char ch;

	PrintMap();
	snake.InitPrint();
	gotoxy(0, 31); 
	cout << "Welcome to The Voracious Snake!" << endl;
	cout << "Introduction:" << endl;
	cout << "Press w/a/s/d to control the snake's direction" << endl;
	cout << "#: Snake's Food       @: Snake's Head      *: Snake's Body" << endl;
	cout << "And don't crash either the wall or its own body. Good luck!" << endl << endl;
	Sleep(2000);
	
	while(1){
		if(kbhit()){
			snake.UserInput();
		}
	    
	    if(snake.gameover == 1){
	    	break;                         //finish the game
		}
		
		snake.JudgeDir();
	    Sleep(1000);
	}
	
	gotoxy(0, 40);
	cout << "Game Over." << endl;
	snake.ShowScore();
	
	return 0;
    system("pause");
} 



    //SetConsoleTextAttribute(hOut, FOREGROUND_INTENSITY);
