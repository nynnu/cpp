//main.cpp

#include "Map.h"
#include "View.h"
#include "Game.h"

int main() {
    View view;
    int gateSpawnTime = 20; // 게이트 생성 시간을 20초로 설정
    view.draw(gateSpawnTime);

    return 0;
}