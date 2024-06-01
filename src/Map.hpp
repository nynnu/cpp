//Map.hpp

#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
#include <ncurses.h>

class Map {
public:
    int mapY; // map 세로 길이
    int mapX; // map 가로 길이
    int **map;
    std::string mapName; // 불러올 map 파일 이름

    Map() {}

    Map(int stage) {
        switch (stage) {
            case 1: mapName = "map1.txt"; break;
            case 2: mapName = "map2.txt"; break;
            case 3: mapName = "map3.txt"; break;
            case 4: mapName = "map4.txt"; break;
            default: throw invalid_argument("Invalid stage");
        }
        load();
    }

    ~Map() {
        for (int i = 0; i < mapY; ++i) {
            delete[] map[i];
        }
        delete[] map;
    }

    void load() {
        ifstream in(mapName);
        if (!in) {
            throw runtime_error("Unable to open map file: " + mapName);
        }
        in >> mapY >> mapX;
        map = new int *[mapY];
        for (int i = 0; i < mapY; ++i) {
            map[i] = new int[mapX];
            for (int j = 0; j < mapX; ++j) {
                in >> map[i][j];
            }
        }
    }

    int getValue(int y, int x) const {
        return map[y][x];
    }

    void setValue(int y, int x, int value) {
        map[y][x] = value;
    }

    vector<int> getPos(int value) const {
        vector<int> v;
        for (int i = 0; i < mapY; ++i) {
            for (int j = 0; j < mapX; ++j) {
                if (map[i][j] == value) {
                    v.push_back(i);
                    v.push_back(j);
                }
            }
        }
        return v;
    }

    void setSnakeZero() {
        for (int i = 0; i < mapY; ++i) {
            for (int j = 0; j < mapX; ++j) {
                if (map[i][j] == 3 || map[i][j] == 4) {
                    map[i][j] = 0;
                }
            }
        }
    }
};


