//main.cpp
//main.cpp

#include "Map.hpp"
#include "View.hpp"
#include "Game.hpp"

int main() {
    View view;
    Map map(1); // 맵 스테이지를 1로 설정
    int gateSpawnTime = 20; // 게이트 생성 시간을 20초로 설정
    view.draw(map, gateSpawnTime);

    return 0;
}
