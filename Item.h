#ifndef ITEM_H
#define ITEM_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Snake.h"
#include "Board.h"

// Item 게임 내 아이템을 정의
class Item {
public:
    // 아이템의 위치, 아이콘, 지속 시간, 생성 시간 초기화
    Item(int y, int x, chtype icon, int duration);
    int getY() const;
    int getX() const;
    chtype getIcon() const;
    time_t getSpawnTime() const;
    int getDuration() const;
    void resetSpawnTime();
    virtual void respawn(int maxY, int maxX) = 0;
    void setY(int newY);
    void setX(int newX);

private:
    int y, x;  // 아이템 위치
    chtype icon;  
    int duration;  // 5초
    time_t spawnTime;  // 생성 시간
};

class Apple : public Item {
public:
    // 랜덤한 위치에 사과 아이템 생성
    Apple(int maxY, int maxX);
    // 사과 아이템 리스폰
    void respawn(int maxY, int maxX) override;
};

class Poison : public Item {
public:
    // 생성자: 랜덤한 위치에 독 아이템 생성
    Poison(int maxY, int maxX);
    // 독 아이템 리스폰
    void respawn(int maxY, int maxX) override;
};

// ItemManager  아이템의 생성, 삭제, 갱신 관리
class ItemManager {
public:
    ItemManager(int maxY, int maxX);
    ~ItemManager();
    // 초기 아이템 생성
    void spawnInitialItems();
    // 아이템을 주기적으로 갱신
    void updateItems(Board& board);
    // 특정 위치의 아이템 제거
    void removeItemAt(int y, int x);
    // 보드에 아이템을 그리기
    void drawItems(Board& board) const;

private:
    std::vector<Item*> items;  // 아이템을 저장
    int maxY, maxX;  // 아이템 생성 가능한 최대 Y, X 좌표

    // 새로운 아이템을 추가
    void addItem();
};

#endif // ITEM_H
