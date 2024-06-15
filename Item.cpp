#include "Item.h"

// Item 클래스 멤버 함수 구현
Item::Item(int y, int x, chtype icon, int duration)
    : y(y), x(x), icon(icon), duration(duration), spawnTime(time(NULL)) {}

int Item::getY() const { return y; }

int Item::getX() const { return x; }

chtype Item::getIcon() const { return icon; }

time_t Item::getSpawnTime() const { return spawnTime; }

int Item::getDuration() const { return duration; }

void Item::resetSpawnTime() { spawnTime = time(NULL); }

void Item::setY(int newY) { y = newY; }

void Item::setX(int newX) { x = newX; }

// Apple 클래스 멤버 함수 구현
Apple::Apple(int maxY, int maxX)
    : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'A', 5) {}

void Apple::respawn(int maxY, int maxX) {
    setY(rand() % (maxY - 2) + 1);
    setX(rand() % (maxX - 2) + 1);
    resetSpawnTime();
}

// Poison 클래스 멤버 함수 구현
Poison::Poison(int maxY, int maxX)
    : Item(rand() % (maxY - 2) + 1, rand() % (maxX - 2) + 1, 'P', 5) {}

void Poison::respawn(int maxY, int maxX) {
    setY(rand() % (maxY - 2) + 1);
    setX(rand() % (maxX - 2) + 1);
    resetSpawnTime();
}

// ItemManager 클래스 멤버 함수 구현
ItemManager::ItemManager(int maxY, int maxX)
    : maxY(maxY), maxX(maxX) {
    srand(time(NULL));
    spawnInitialItems();
}

ItemManager::~ItemManager() {
    for (auto item : items) {
        delete item;
    }
}

void ItemManager::spawnInitialItems() {
    addItem();
}

void ItemManager::updateItems(Board& board) {
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

void ItemManager::removeItemAt(int y, int x) {
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

void ItemManager::drawItems(Board& board) const {
    for (const auto& item : items) {
        board.addAt(item->getY(), item->getX(), item->getIcon());
    }
}

void ItemManager::addItem() {
    if (rand() % 2 == 0) {
        items.push_back(new Apple(maxY, maxX));
    } else {
        items.push_back(new Poison(maxY, maxX));
    }
}
