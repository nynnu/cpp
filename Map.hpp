#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
#include <ncurses.h>


class Map {
public:
    int mapY; //map 새로 길이

    int mapX; // map 가로 길이

    int **map;
    std::string mapName; // 불러올 map 파일 이름

    Map() {};
    
    Map(int stage) {
        if (stage == 1)
            mapName = "map1.txt";
        if (stage == 2)
            mapName = "map2.txt";
        if (stage == 3)
            mapName = "map3.txt";
        if (stage == 4)
            mapName = "map4.txt";
        load();
    }


    void load(){
        ifstream in(mapName);
        in >> mapY >> mapX;
        map = new int *[mapY];
        for (int i = 0; i < mapY; i++) {
            map[i] = new int [mapX];
            for (int j = 0; j < mapX; j++) {
                in >> map[i][j];
            }
        }
    } // 맵 불러오기

    
    int getValue(int y, int x){
        return map[y][x];
    }  // 좌표로 값 불러오기


    void setValue(int y, int x, int value) {
        map[y][x] = value;
    }

   
    vector<int> getPos(int value) {   // 아이템, 머리, 몸통 위치 파악
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


    void setSnakeZero() {
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

};