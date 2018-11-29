#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include<cstdlib>
#include<ctime>
#include <unistd.h>

using std::cout;
using std::cin;
using std::endl;

bool impasse(int, int, int**, int, int); //визначає тупики
void makeMaze(int**, int, int); //алгоритм

const int wall = 0, pass = 1, hero = 2, exitPortal = 3;//позначення в масиві
const int red = 12, white = 7, darkGreen = 2, black = 0, magenta = 13, green = 10;//кольори
const int up = 1, down = 2, left = 3, right = 4; //направлення
const int maxSize = 99; // максимальний розмір лабіринту


enum mode {freeMode = 0, easy, medium, hard};//режим гри

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //доступ до консолі для зміни кольору і положення курсора
COORD position; // позиція курсора

class Maze{ //лабіринт
public:
    int** maze = new int*[maxSize]; //масив лабіринту не більше 99х99
    int height, width; //висота і ширина лабіринту
    Maze(int h, int w){ //конструктор
        height = h, width = w;
        for(int i = 0; i < height; i++)//виділення пам'яті
            maze[i] = new int[width];

        for(int i = 0; i < height; i++) // Масив заповнюється стінами
        	for(int j = 0; j < width; j++)
        		maze[i][j] = wall;

        makeMaze(maze, height, width); // створюється лабіринт
        maze[1][width - 2] = exitPortal; // встановлення точки виходу
    }
    void draw(int  i, int j){
        switch(maze[i][j]){
            case wall:SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | darkGreen)); cout<<"# "; break;
            case pass:SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | white)); cout<<"  "; break;
            case hero:SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red)); cout<<"@ "; break;
            case exitPortal:SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta)); cout<<"O "; break;
        }
    }
    void visual(){ //відрисувати весь кадр
    	for(int i = 0; i < height; i++){
    		for(int j = 0; j < width; j++)
                draw(i, j);
    		cout<<endl;
    		}
    	cout<<endl;
    }

    void visual(int height){ // відрисувати частину кадра за рядком
        for(int j = 0; j < width; j++){
            draw(height, j);
        }
        cout<<endl;
    }
};

class Hero{ //герой
public:
    int x, y; //положення героя
    Hero(int h, int w){ // конструктор
        y = h - 2;
        x = 1;
    }
    void up(){ //крок вверх
        y--;
    }
    void down(){ //крок вниз
        y++;
    }
    void right(){ //крок вправо
        x++;
    }
    void left(){ //крок вліво
        x--;
    }
};

void stop(){
    Sleep(100);
    sleep(1/100);
}

void win(){
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"      ___                          \n";
    cout<<"  _.-|   |          |\\__/,|   (`\\  \n";
    cout<<" {   |   |          |o o  |__ _) ) \n";
    cout<<"  \"-.|___|        _.( T   )  `  /  \n";
    cout<<"  .--'-`-.     _((_ `^--' /_<  \\   \n";
    cout<<".+|______|__.-||__)`-'(((/  (((/   \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | green));
    cout<<" ,--.   ,--.    ,--.    ,--.  ,--. \n";
    cout<<" |  |   |  |    |  |    |  ,'.|  | \n";
    cout<<" |  |.'.|  |    |  |    |  |' '  | \n";
    cout<<" |   ,'.   |    |  |    |  | `   | \n";
    cout<<" '--'   '--'    `--'    `--'  `--' \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | white));
}

void prew(){
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | green));
    cout<<"      ,--.   ,--.      ,---.      ,-------.    ,------. \n";
    cout<<"      |   `.'   |     /  O  \\     `--.   /     |  .---' \n";
    cout<<"      |  |'.'|  |    |  .-.  |      /   /      |  `--,  \n";
    cout<<"      |  |   |  |    |  | |  |     /   `--.    |  `---. \n";
    cout<<"      `--'   `--'    `--' `--'    `-------'    `------' \n";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"________________________________________________________________  \n";
    cout<<"|____|____|____|____|____|____|____|____|____|____|____|____|____ \n";
    cout<<"____|____|____|____|____|____|____|____|____|____|____|____|____| \n";
    cout<<"__|____|____|____|____|___|_         ____|____|____|____|____|__  \n";
    cout<<"|____|____|____|____|___|";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
    cout<<"     (\\.-./)";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<" _|____|____|____|___|___|_  \n";
    cout<<"____|____|____|____|____|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
    cout<<"  = (^ Y ^) =  ";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"_|____|____|____|____|__ \n";
    cout<<"|____|____|____|____|____|___";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
    cout<<" /`---`\\ ";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"__|____|____|____|____|____|\n";
    cout<<"__|____|____|____|____|____|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
    cout<<"U";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"___|_";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
    cout<<"U";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | magenta));
    cout<<"|____|____|____|____|____|___ \n";
    cout<<"|____|____|____|____|____|____|____|____|____|____|____|____|____ \n";
    cout<<"____|____|____|____|____|____|____|____|____|____|____|____|____|_\n";
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | white));
}

void crushWall(Maze maze, Hero player, int key){
    switch(key){
        case up:
            maze.maze[player.y - 1][player.x] = pass;
            position.Y = player.y - 1;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(player.y - 1); break;
        case down:
            maze.maze[player.y + 1][player.x] = pass;
            position.Y = player.y + 1;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(player.y + 1); break;
            break;
        case left:
            maze.maze[player.y][player.x - 1] = pass;
            position.Y = player.y;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(player.y); break;
            break;
        case right:
            maze.maze[player.y][player.x + 1] = pass;
            position.Y = player.y;
            SetConsoleCursorPosition(hConsole, position);
            maze.visual(player.y); break;
            break;
    }
}

void crushPoint(int crush, Maze maze){
    position.Y = maze.height;
    SetConsoleCursorPosition(hConsole, position);
    if(crush != 0){
        SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | red));
        cout << "Crush : ";
        for(int i = 0; i < crush; i++){
            cout<< " * ";
        }
        cout << "   ";
        SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | darkGreen));
    }else{
        position.Y = maze.height;
        SetConsoleCursorPosition(hConsole, position);
        cout << "           ";
    }
}

void game(Maze maze, Hero player, int crush){
    unsigned int start_time, end_time, search_time;
    int flag = 0; // індикатор проходження лабіринту
    position.X = 0;
    position.Y = 0;
    int moveCounter = 0;

    system("cls"); // очищаємо поле
    SetConsoleCursorPosition(hConsole, position);//курсор в початок
    maze.maze[player.y][player.x] = hero; // початкове полложення героя
    maze.visual(); // відрисувати кадр
    crushPoint(crush, maze);

    start_time = clock();
    while(flag == 0){
        if(player.y == 1 && player.x == maze.width - 2){
            flag++;
            system("cls");
            win();
        }else{
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_UP) && maze.maze[player.y - 1][player.x] == wall){
                if(player.y - 1 != maze.height-1 && player.y - 1 != 0 && crush != 0){
                    crushWall(maze, player, up);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_DOWN) && maze.maze[player.y + 1][player.x] == wall){
                if(player.y + 1 != maze.height-1 && player.y + 1 != 0 && crush != 0){
                    crushWall(maze, player, down);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_LEFT) && maze.maze[player.y][player.x - 1] == wall){
                if(player.x - 1 != maze.width-1 && player.x - 1 != 0 && crush != 0){
                    crushWall(maze, player, left);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_RIGHT) && maze.maze[player.y][player.x + 1] == wall){
                if(player.x + 1 != maze.width-1 && player.x + 1 != 0 && crush != 0){
                    crushWall(maze, player, right);
                    crush--;
                    crushPoint(crush, maze);
                }
            }
            if(GetAsyncKeyState(VK_UP) && maze.maze[player.y - 1][player.x] != 0){
                player.up();
                moveCounter++;
                maze.maze[player.y][player.x] = hero;
                maze.maze[player.y + 1][player.x] = pass;

                position.Y = player.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y);

                position.Y = player.y + 1;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y + 1);
                stop();
            }
            if(GetAsyncKeyState(VK_DOWN) && maze.maze[player.y + 1][player.x] != 0){
                player.down();
                moveCounter++;
                maze.maze[player.y][player.x] = hero;
                maze.maze[player.y-1][player.x] = pass;

                position.Y = player.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y);

                position.Y = player.y - 1;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y - 1);
                stop();
            }
            if(GetAsyncKeyState(VK_LEFT) && maze.maze[player.y][player.x - 1] != 0){
                player.left();
                moveCounter++;
                maze.maze[player.y][player.x] = hero;
                maze.maze[player.y][player.x + 1] = pass;

                position.Y = player.y;
                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y);
                stop();
            }
            if(GetAsyncKeyState(VK_RIGHT) && maze.maze[player.y][player.x + 1] != 0){
                player.right();
                moveCounter++;
                maze.maze[player.y][player.x] = hero;
                maze.maze[player.y][player.x - 1] = pass;
                position.Y = player.y;

                SetConsoleCursorPosition(hConsole, position);
                maze.visual(player.y);
                stop();
            }
        }
    }
    end_time = clock();
    search_time = end_time - start_time;
    SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | white));
    cout << " Time : " << search_time/1000.0 << "        Move : " << moveCounter << endl;
    cout <<"__________________________________\n";
    system("pause");
}

int main(){
    srand((unsigned)time(NULL));
    int crush = 0;
    mode gameMode = freeMode;
    int modeKey = gameMode;
    int width = 21,  height = 21;

    while(true){
        system("cls");
        prew();
        SetConsoleTextAttribute(hConsole, (WORD) ((black << 4) | white));
        cout << "Select mode: \n0)for generate maze arbitrary size not more "<< maxSize <<"x"<< maxSize << "\n1)Easy mode\n2)Medium mode\n3)Hard mode\n";
        cin >> modeKey;

        switch(modeKey){
        case freeMode:{
            int flag = 0;
            while(flag == 0){
                cout << "Enter odd width, height, crush: ";
                cin >> width >> height >> crush;
                if((width % 2 == 0 || height % 2 == 0) || (width > maxSize || height > maxSize)){
                    cout << "Error: no odd width or height or value > " << maxSize << "\n";
                }else{
                    flag++;
                }
            }
            break;
        }
        case easy:
            width = 19;
            height = 19;
            crush = 0;
            break;
        case medium:
            width = 31;
            height = 31;
            crush = 1;
            break;
        case hard:
            width = 31;
            height = 31;
            crush = 2;
            break;
        default:
            cout << "incorect mode, build maze 21x21\n";
            system("pause");
            break;
        }
        Maze maze = Maze(height, width);
        Hero player = Hero(height, width);
        game(maze, player, crush);
    }
    return 0;
}

bool impasse(int x, int y, int** maze, int height, int width){ //перевірка на тупік
	int a = 0;

	if(x != 1){
		if(maze[y][x-2] == pass)
			a+=1;
		}
	else a+=1;

	if(y != 1){
		if(maze[y-2][x] == pass)
			a+=1;
		}
	else a+=1;

	if(x != width-2){
		if(maze[y][x+2] == pass)
			a+=1;
		}
	else a+=1;

	if(y != height-2){
		if(maze[y+2][x] == pass)
			a+=1;
		}
	else a+=1;

	if(a == 4)
		return 1;
	else
		return 0;
}

void makeMaze(int** maze, int height, int width){ //генеруємо лабіринт
	int x, y, c, a;
	bool b;

	x = 3; y = 3; a = 0; // точка початку прокладання лабіринту і лічильник
	while(a < 10000){
		maze[y][x] = pass; a++; //прокладаємо шлях
		while(1){
			c = rand()%4;//вибираємо напрямок
			switch(c){
				case up: if(y != 1)
					if(maze[y-2][x] == wall){ // Вверх
                    maze[y-1][x] = pass;
					maze[y-2][x] = pass;
					y-=2;
				}
				case down: if(y != height-2)
					if(maze[y+2][x] == wall){ // Вниз
					maze[y+1][x] = pass;
					maze[y+2][x] = pass;
					y+=2;
				}
				case left: if(x != 1)
					if(maze[y][x-2] == wall){ // Наліво
					maze[y][x-1] = pass;
					maze[y][x-2] = pass;
					x-=2;
				}
				case right: if(x != width-2)
					if(maze[y][x+2] == wall){ // Направо
					maze[y][x+1] = pass;
					maze[y][x+2] = pass;
					x+=2;
				}
			}
			if(impasse(x,y,maze,height,width)) // перевіряємо на тупік
			   break;
		}
		if(impasse(x,y,maze,height,width)) //перекидаємо прокладчик в іншу точку
			do{
				x = 2*(rand()%((width-1)/2))+1;
				y = 2*(rand()%((height-1)/2))+1;
			}
			while(maze[y][x] != pass);
	}
}
