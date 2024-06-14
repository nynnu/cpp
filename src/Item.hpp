//Item.hpp

#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "Snake.hpp"
#include "Board.hpp"

class Item {
public:
    Item(int y, int x, chtype icon, int duration)
        : y(y), x(x), icon(icon), duration(duration), spawnTime(time(NULL)) {}

    int getY() const { return y; }
    int getX() const { return x; }
    void setY(int newY) { y = newY; }
    void setX(int newX) { x = newX; }
    chtype getIcon() const { return icon; }
    time_t getSpawnTime() const { return spawnTime; }
    int getDuration() const { return duration; }

    void resetSpawnTime() { spawnTime = time(NULL); }
    virtual void respawn(int maxY, int maxX) = 0; // 순수 가상 함수로 정의

private:
    int y, x;
    chtype icon;
    int duration;  // 지속 시간 (초 단위)
    time_t spawnTime;
};

class Apple : public Item {
public:
    Apple(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'A', 5) {}

    void respawn(int maxY, int maxX) override {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

class Poison : public Item {
public:
    Poison(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'P', 5) {}

    void respawn(int maxY, int maxX) override {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

class ItemManager {
public:
    ItemManager(int maxY, int maxX) : maxY(maxY), maxX(maxX) {
        srand(time(NULL)); // Seed for randomization
        spawnInitialItems();
    }

    ~ItemManager() {
        for (auto item : items) {
            delete item;
        }
    }

    void spawnInitialItems() {
        addItem();
    }

    void updateItems(Board& board) {
        time_t now = time(NULL);
        for (auto& item : items) {
            if (difftime(now, item->getSpawnTime()) > item->getDuration()) {
                board.addEmpty(SnakePiece(item->getY(), item->getX(), ' ')); // 화면에서 아이템 제거
                item->respawn(maxY, maxX); // 아이템 재생성
            }
        }
        if (rand() % 100 < 10 && items.size() < 3) { // 10% 확률로 아이템 생성
            addItem();
        }
    }

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

    void drawItems(Board& board) const {
        for (const auto& item : items) {
            board.addAt(item->getY(), item->getX(), item->getIcon());
        }
    }

private:
    std::vector<Item*> items;
    int maxY, maxX;

    void removeExpiredItems(Board& board) {
        time_t now = time(NULL);
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
    }

    void addItem() {
        if (rand() % 2 == 0) {
            items.push_back(new Apple(maxY, maxX));
        } else {
            items.push_back(new Poison(maxY, maxX));
        }
    }
};
