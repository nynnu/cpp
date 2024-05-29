#ifndef MAP
#define MAP

#include <iostream>
#include <vector>
#include "Board.hpp"

class Map {
public:
    int mapY; //map 새로 길이

    int mapX; // map 가로 길이

    int **map;
    std::string mapName; // 불러올 map 파일 이름

    Map();
    Map(int stage);

    void load(); // 맵 불러오기

    
    int getValue(int y, int x);  // 좌표로 값 불러오기


    void setValue(int y, int x, int v);

   
    std::vector<int> getPos(int v);


    void setSnakeZero();

    void displayMap(); 
};
#endif
