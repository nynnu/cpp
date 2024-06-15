#include "Map.h"

Map::Map() : mapY(0), mapX(0), map(nullptr) {}

Map::Map(int stage) {
    switch (stage) {
        case 1: mapName = "map1.txt"; break;
        case 2: mapName = "map2.txt"; break;
        case 3: mapName = "map3.txt"; break;
        case 4: mapName = "map4.txt"; break;
        default: throw std::invalid_argument("Invalid stage");
    }
    load();
}

Map::~Map() {
    for (int i = 0; i < mapY; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

void Map::load() {
    std::ifstream in(mapName);
    if (!in) {
        throw std::runtime_error("Unable to open map file: " + mapName);
    }
    in >> mapY >> mapX;
    if (mapY <= 0 || mapX <= 0) {
        throw std::runtime_error("Invalid map size in file: " + mapName);
    }
    map = new int*[mapY];
    for (int i = 0; i < mapY; ++i) {
        map[i] = new int[mapX];
        for (int j = 0; j < mapX; ++j) {
            if (!(in >> map[i][j])) {
                throw std::runtime_error("Error reading map data from file: " + mapName);
            }
        }
    }
}

int Map::getValue(int y, int x) const {
    if (y < 0 || y >= mapY || x < 0 || x >= mapX) {
        throw std::out_of_range("Map position out of range");
    }
    return map[y][x];
}

void Map::setValue(int y, int x, int value) {
    if (y < 0 || y >= mapY || x < 0 || x >= mapX) {
        throw std::out_of_range("Map position out of range");
    }
    map[y][x] = value;
}

std::vector<int> Map::getPos(int value) const {
    std::vector<int> v;
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

void Map::setSnakeZero() {
    for (int i = 0; i < mapY; ++i) {
        for (int j = 0; j < mapX; ++j) {
            if (map[i][j] == 3 || map[i][j] == 4) {
                map[i][j] = 0;
            }
        }
    }
}
