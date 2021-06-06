#include<iostream>
#include<time.h>
#include<stdlib.h> 
#include<Windows.h>
#include<conio.h>

using namespace std;

#define ROW 80
#define COL 80


void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}


void initialize(char** &world, int rows, int cols, char life) {
    world = new char* [rows];
    for (int i = 0; i < rows; i++) {
        world[i] = new char[cols];
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            world[i][j] = ' ';
        }
    }
	int r, c;
	int prevr, prevc;
	do{
		getRowColbyLeftClick(r, c);
		if(r == prevr && c == prevc)
			continue;
		gotoRowCol(r, c);
		if(world[r][c] == life){
			cout << ' ';
			world[r][c] = ' ';
		}
		else{
			cout << life;
			world[r][c] = life;
		}
		prevr = r;
		prevc = c;
	}while(r!=0 || c != 0);
}

void repopulate(char** &world, int M, int N, char life) {
    
    char** futureworld = new char* [M];
    for (int i = 0; i < M; i++) {
        futureworld[i] = new char[N];
    }

    for (int l = 0; l < M ; l++)
    {
        for (int m = 0; m < N ; m++)
        {
            int aliveNeighbours = 0;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0)
                        continue;
                    else if (l + i < 0 || l + i >= M)
                        continue;
                    else if (m + j < 0 || m + j >= N)
                        continue;
                    else if (world[l + i][m + j] == life)
                        aliveNeighbours++;

                }
            //alone
            if ((world[l][m] == life) && (aliveNeighbours < 2))
                futureworld[l][m] = ' ';

            
            else if ((world[l][m] == life) && (aliveNeighbours >= 2 && aliveNeighbours <= 3))
                futureworld[l][m] = world[l][m];

            else if ((world[l][m] == life) && (aliveNeighbours > 3))
                futureworld[l][m] = ' ';
            //overpopulation 
            else if ((world[l][m] == ' ') && (aliveNeighbours == 3))
                futureworld[l][m] = life;

            else
                futureworld[l][m] = world[l][m];
        }
    }
    for (int i = 0; i < M; i++) {
        delete world[i];
    }
    delete world;
    world = futureworld;
}

void display(char** world, int M, int N) {
    system("cls");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << world[i][j];
        }
    }

}

int main() {
	char** world;
	char life, death, dtl, ltd; 
    life = 2;
    initialize(world, ROW, COL, life);
    while(true)
    {
        display(world, ROW, COL);
        repopulate(world, ROW, COL, life);
    }
   
	return 0;
}