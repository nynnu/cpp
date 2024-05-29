#include <fstream>
#include "Map.h"
#include <string>
using namespace std;
#include <ncurses.h>



Map::Map() {}

Map::Map(int stage) {
    mapName = "map1.txt";
    load();
}


void Map::load() {
    ifstream in(mapName);
    in >> mapY >> mapX;
    map = new int *[mapY];
    for (int i = 0; i < mapY; i++) {
        map[i] = new int [mapX];
        for (int j = 0; j < mapX; j++) {
            in >> map[i][j];
        }
    }
}

int Map::getValue(int y, int x) {  // 좌표값 얻기
    return map[y][x];
}

void Map::setValue(int y, int x, int value) { // 좌표값 바꾸기
    map[y][x] = value;
}

vector<int> Map::getPos(int value) {   // 아이템, 머리, 몸통 위치 파악
    vector<int> v;  
    for (int i = 0; i < mapY; i++) {
        for (int j = 0; j < mapX; j++) {
            if (map[i][j] == value) {
                v.push_back(i);
                v.push_back(j);
            }
        }
    }
    return v;
}

void Map::setSnakeZero() {   //  뱀 초기화 (게임 종료할때)
    for (int i = 0; i < mapY; i++) {
        for (int j = 0;j < mapX; j++) {
            if (map[i][j] == 3) {
                map[i][j] = 0;
            }
            if (map[i][j] == 4) {
                map[i][j] = 0;
            }
        }
    }
}


void Map::displayMap() {  // 화면에 출력
    initscr(); 
    noecho(); 
    cbreak(); 
    curs_set(0); 

    for (int i = 0; i < mapY; ++i) {
        for (int j = 0; j < mapX; ++j) {
            move(i, j);
            switch (map[i][j]) {
                case 0:
                    addch(' '); // Empty space
                    break;
                case 1:
                    addch('#'); // Wall
                    break;
                case 2:
                    addch('#'); // immune wall
                    break;
                case 3:
                    addch('B'); // Snake body
                    break;
                case 4:
                    addch('H'); // Snake head
                    break;
                case 9:
                    addch('X'); // poison item
                case 7 :
                    addch('+'); // growth item
                default:
                    break;
            }
        }
    }

    refresh(); 

    getch(); 

    endwin(); 
}


