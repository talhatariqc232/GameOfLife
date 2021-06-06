#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

#define FACE 6
#define ROW 120
#define COL 120
#define SIZE 5

#define LIFE 1
#define DEAD 0
#define PRE_LIFE 2
#define PRE_DEATH -1

int world[FACE][ROW][COL];
sf::RectangleShape cells[FACE][ROW][COL];
bool pregame = true;
bool give_life = true;

int current_face = 0;

int left[6][4] = {
    {4,-1,2,1},
    {5,-1,2,1},
    {4,0,-1,1},
    {4,2,-1,-1},
    {1,-1,2,1},
    {0,-1,2,1}
};

int right[6][4] = {
    {5,-1,0,1},
    {4,-1,0,1},
    {5,0,-1,-1},
    {5,2,-1,1},
    {0,-1,0,1},
    {1,-1,0,1}
};

int up[6][4] = {
    {2,2,-1,1},
    {2,0,-1,-1},
    {1,0,-1,-1},
    {0,2,-1,1},
    {2,-1,0,1},
    {2,-1,2,-1}
};  

int down[6][4] = {
    {3,0,-1,1},
    {3,2,-1,-1},
    {0,0,-1,1},
    {1,2,-1,-1},
    {3,-1,0,-1},
    {3,-1,2,1}
};

void init(){
    srand(time(NULL));
    for(int i = 0; i < FACE; i++){
        for(int j = 0; j < ROW; j++){
            for(int k = 0; k < COL; k++){
                world[i][j][k] = 0;
            }
        }
    }
	
	
	for(int i = 0; i < FACE; i++){
        for(int j = 0; j < ROW; j++){
            for(int k = 0; k < COL; k++){
				
                cells[i][j][k].setSize(sf::Vector2f(SIZE, SIZE));
                cells[i][j][k].setPosition(sf::Vector2f(SIZE*k, SIZE*j));
                if(world[i][j][k] == LIFE)
                    cells[i][j][k].setFillColor(sf::Color(255, 255, 255));
                else
                    cells[i][j][k].setFillColor(sf::Color(0, 0, 0));
            }
        }
    }
}

int countNeighbors(int f, int r, int c){
    int count = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 and j == 0)
                continue;
            if(r+i < 0 or r+i >= ROW)
                continue;
            if(c+j < 0 or c+j >= COL)
                continue;
            if(world[f][r+i][c+j] == LIFE or world[f][r+i][c+j] == PRE_DEATH)
                count++;
        }
    }
    int cell;   
    if(r == 0){
        for(int i = -1; i <= 1; i++){
            if(up[f][1] == -1){
                int row = r+i;
                if(up[f][3] == -1)
                    row = ROW-row;
                cell = world[up[f][0]][row][up[f][2]];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
            else if(up[f][2] == -1){
                int col = r+i;
                if(up[f][3] == -1)
                    col = COL-col;
                cell = world[up[f][0]][up[f][1]][col];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
        } 
    }
    else if(r == ROW-1){
        for(int i = -1; i <= 1; i++){
            if(down[f][1] == -1){
                int row = r+i;
                if(down[f][3] == -1)
                    row = ROW-row;
                cell = world[down[f][0]][row][down[f][2]];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
            else if(down[f][2] == -1){
                int col = r+i;
                if(down[f][3] == -1)
                    col = COL-col;
                cell = world[down[f][0]][down[f][1]][col];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
        } 
    }
    if(c == 0){
        for(int i = -1; i <= 1; i++){
            if(left[f][1] == -1){
                int row = r+i;
                if(left[f][3] == -1)
                    row = ROW-row;
                cell = world[left[f][0]][row][left[f][2]];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
            else if(left[f][2] == -1){
                int col = r+i;
                if(left[f][3] == -1)
                    col = COL-col;
                cell = world[left[f][0]][left[f][1]][col];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
        } 
    }
    else if(c == COL-1){
        for(int i = -1; i <= 1; i++){
            if(right[f][1] == -1){
                int row = r+i;
                if(right[f][3] == -1)
                    row = ROW-row;
                cell = world[right[f][0]][row][right[f][2]];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
            else if(right[f][2] == -1){
                int col = r+i;
                if(right[f][3] == -1)
                    col = COL-col;
                cell = world[right[f][0]][right[f][1]][col];
                if(cell == LIFE or cell == PRE_DEATH)
                    count++;
            }
        } 
    }
    return count;
}

void repopulate() {
   
    for(int i = 0; i < FACE; i++){
        for(int j = 0; j < ROW; j++){
            for(int k = 0; k < COL; k++){
                int neighbors = countNeighbors(i, j, k);
                if(world[i][j][k] == LIFE){
                    if(neighbors != 2 and neighbors != 3)
                        world[i][j][k] = PRE_DEATH;
                }
                if(world[i][j][k] == DEAD){
                    if(neighbors == 3)
                        world[i][j][k] = PRE_LIFE;
                }
            }
        }
    }
    for(int i = 0; i < FACE; i++){
        for(int j = 0; j < ROW; j++){
            for(int k = 0; k < COL; k++){
                if(world[i][j][k] == PRE_LIFE){
                    world[i][j][k] = LIFE;
                    cells[i][j][k].setFillColor(sf::Color(255, 255, 255));
                }
                else if (world[i][j][k] == PRE_DEATH){
                    world[i][j][k] = DEAD;
                    cells[i][j][k].setFillColor(sf::Color(0, 0, 0));
                }
            }
        }
    }
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(SIZE*COL, SIZE*ROW), "Game of Life");
    init();
    int prevx, prevy;

    int updateTime = 500;
    int timer = updateTime;

    sf::Clock clock;
    sf::Time prev_time = clock.getElapsedTime();
    sf::Time curr_time;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Left){
                    current_face--;
                    if(current_face < 0)
                        current_face = 5;
                }
                else if (event.key.code == sf::Keyboard::Right)
                    current_face = (current_face+1)%6;
                else if (event.key.code == sf::Keyboard::Up)
                    pregame = false;
				else if (event.key.code == sf::Keyboard::Z)
                    give_life = true;
				else if (event.key.code == sf::Keyboard::X)
                    give_life = false;
            }

            if(pregame){
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    sf::Vector2i position = sf::Mouse::getPosition(window);
                    int x = position.x/SIZE;
                    int y = position.y/SIZE;
                    if((x >= 0 and x < COL) and (y >= 0 and y < ROW) and(x != prevx or y != prevy)){
                        if(give_life){
							world[current_face][y][x] = LIFE;
                            cells[current_face][y][x] .setFillColor(sf::Color(255, 255, 255));
						}
                        else{
							world[current_face][y][x] = DEAD;
                            cells[current_face][y][x] .setFillColor(sf::Color(0, 0, 0));
						}
                        prevx = x;
                        prevy = y;
                    }
                }
            }
        }
        if(!pregame){
            curr_time = clock.getElapsedTime();
            timer -= curr_time.asMilliseconds() - prev_time.asMilliseconds();
            if(timer < 0){
                timer = updateTime;
                repopulate();
            }
            prev_time = clock.getElapsedTime();
        }
        window.clear(); 
        for(int i = 0; i < ROW; i++)
            for(int j = 0; j < COL; j++)
                window.draw(cells[current_face][i][j]);
        window.display();
    }

    return 0;
}