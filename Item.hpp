// Item.hpp

#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Snake.hpp"
#include "Board.hpp"

// Item 게임 내 아이템을 정의
class Item {
public:
    // 아이템의 위치, 아이콘, 지속 시간, 생성 시간 초기화
    Item(int y, int x, chtype icon, int duration)
        : y(y), x(x), icon(icon), duration(duration), spawnTime(time(NULL)) {}

    int getY() const { return y; }
    int getX() const { return x; }
    chtype getIcon() const { return icon; }
    time_t getSpawnTime() const { return spawnTime; }
    int getDuration() const { return duration; }
    void resetSpawnTime() { spawnTime = time(NULL); }
    virtual void respawn(int maxY, int maxX) = 0;
    void setY(int newY) { y = newY; }
    void setX(int newX) { x = newX; }

private:
    int y, x;  // 아이템 위치
    chtype icon;  
    int duration;  // 5초
    time_t spawnTime;  // 생성 시간
};

class Apple : public Item {
public:
    // 랜덤한 위치에 사과 아이템 생성
    Apple(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'A', 5) {}

    // 사과 아이템 리스폰
    void respawn(int maxY, int maxX) override {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

class Poison : public Item {
public:
    // 생성자: 랜덤한 위치에 독 아이템 생성
    Poison(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'P', 5) {}

    // 독 아이템 리스폰
    void respawn(int maxY, int maxX) override {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

// ItemManager  아이템의 생성, 삭제, 갱신 관리
class ItemManager {
public:
    ItemManager(int maxY, int maxX) : maxY(maxY), maxX(maxX) {
        srand(time(NULL)); 
        spawnInitialItems();
    }

    ~ItemManager() {
        for (auto item : items) {
            delete item;
        }
    }

    // 초기 아이템 생성
    void spawnInitialItems() {
        addItem();
    }

    // 아이템을 주기적으로 갱신
    void updateItems(Board& board) {
        time_t now = time(NULL);
        // 만료된 아이템 제거
        auto it = std::remove_if(items.begin(), items.end(),
                                 [&](Item* item) {
                                     if (difftime(now, item->getSpawnTime()) > item->getDuration()) {
                                         board.addEmpty(SnakePiece(item->getY(), item->getX(), ' '));
                                         delete item;
                                         return true;
                                     }
                                     return false;
                                 });
        items.erase(it, items.end());

        // 필요 시 새로운 아이템 생성
        if (items.size() < 3 && rand() % 100 < 10) { // 10% 확률로 아이템 생성
            addItem();
        }
    }

    // 특정 위치의 아이템 제거
    void removeItemAt(int y, int x) {
        auto it = std::remove_if(items.begin(), items.end(),
                                 [&](Item* item) {
                                     if (item->getY() == y && item->getX() == x) {
                                         delete item;
                                         return true;
                                     }
                                     return false;
                                 });
        items.erase(it, items.end());
    }

    // 보드에 아이템을 그리기
    void drawItems(Board& board) const {
        for (const auto& item : items) {
            board.addAt(item->getY(), item->getX(), item->getIcon());
        }
    }

private:
    std::vector<Item*> items;  // 아이템을 저장
    int maxY, maxX;  // 아이템 생성 가능한 최대 Y, X 좌표

    // 새로운 아이템을 추가
    void addItem() {
        if (rand() % 2 == 0) {
            items.push_back(new Apple(maxY, maxX));
        } else {
            items.push_back(new Poison(maxY, maxX));
        }
    }
};