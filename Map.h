#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <ncurses.h>

class Map {
public:
    int mapY; // map 세로 길이
    int mapX; // map 가로 길이
    int **map;
    std::string mapName; // 불러올 map 파일 이름

    Map(); // 기본 생성자
    Map(int stage); // stage를 기반으로 map을 로드하는 생성자
    ~Map(); // 소멸자

    void load(); // map 파일 로드
    int getValue(int y, int x) const; // 특정 위치의 map 값 반환
    void setValue(int y, int x, int value); // 특정 위치의 map 값 설정
    std::vector<int> getPos(int value) const; // 특정 값이 있는 모든 위치 반환
    void setSnakeZero(); // 뱀의 위치를 0으로 설정
};

#endif // MAP_H
