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
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    chtype getIcon() const { return icon; }
    time_t getSpawnTime() const { return spawnTime; }
    int getDuration() const { return duration; }

    void resetSpawnTime() { spawnTime = time(NULL); }

protected:
    int y, x;
    chtype icon;
    int duration;  // 지속 시간 (초 단위)
    time_t spawnTime;
};

class Apple : public Item {
public:
    Apple(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'A', 5) {}

    void spawn(int maxY, int maxX) {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

class Poison : public Item {
public:
    Poison(int maxY, int maxX)
        : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'P', 5) {}

    void spawn(int maxY, int maxX) {
        setY(rand() % (maxY - 2) + 1);
        setX(rand() % (maxX - 2) + 1);
        resetSpawnTime();
    }
};

class ItemManager {
public:
    ItemManager(int maxY, int maxX) : maxY(maxY), maxX(maxX) {
        srand(time(NULL)); // Seed for randomization
        items.push_back(new Apple(maxY, maxX));
        items.push_back(new Poison(maxY, maxX));
    }

    ~ItemManager() {
        for (auto item : items) {
            delete item;
        }
    }

    void spawnItems() {
        removeExpiredItems();
        if (items.size() < 3) {
            items.push_back(new Apple(maxY, maxX));
            items.push_back(new Poison(maxY, maxX));
        }
    }

    bool snakeAteApple(const Snake& snake) const {
        for (const auto& item : items) {
            if (item->getIcon() == 'A' &&
                snake.head().getY() == item->getY() &&
                snake.head().getX() == item->getX()) {
                return true;
            }
        }
        return false;
    }

    bool snakeAtePoison(const Snake& snake) const {
        for (const auto& item : items) {
            if (item->getIcon() == 'P' &&
                snake.head().getY() == item->getY() &&
                snake.head().getX() == item->getX()) {
                return true;
            }
        }
        return false;
    }

    void drawItems(Board& board) const {
        for (const auto& item : items) {
            board.addAt(item->getY(), item->getX(), item->getIcon());
        }
    }

    void removeItemAt(int y, int x) {
        items.erase(
            std::remove_if(items.begin(), items.end(),
                           [&](Item* item) {
                               if (item->getY() == y && item->getX() == x) {
                                   delete item;
                                   return true;
                               }
                               return false;
                           }),
            items.end());
    }

private:
    std::vector<Item*> items;
    int maxY, maxX;

    void removeExpiredItems() {
        time_t now = time(NULL);
        items.erase(
            std::remove_if(items.begin(), items.end(),
                           [now](Item* item) {
                               if (difftime(now, item->getSpawnTime()) > item->getDuration()) {
                                   delete item;
                                   return true;
                               }
                               return false;
                           }),
            items.end());
    }
};
