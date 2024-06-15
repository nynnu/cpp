#ifndef GATE_H
#define GATE_H

#include <ncurses.h>
#include <vector>
#include <ctime>
#include <algorithm> // std::any_of
#include "Snake.h"
#include "Board.h"
#include "Map.h"

class Gate {
public:
    Gate(int maxY, int maxX, int spawnInterval);
    void initialize(Map& map, Board& board);
    void update(Board& board);
    std::pair<int, int> getNextPosition(const SnakePiece& head, DIRECTION& newDir);

private:
    std::vector<std::pair<int, int>> wallPositions;
    std::vector<std::pair<int, int>> emptyPositions;
    std::pair<int, int> gate1, gate2;
    int maxY, maxX;
    bool isActive;
    bool snakeInGate;
    time_t startTime;
    int spawnInterval;
    const int gateLifetime = 10; // 게이트 지속 시간 10초. 10초 지나면 지워짐

    void findWallPositions(Map& map);
    void findEmptyPositions(Map& map);
    void placeGates(Board& board);
    void removeGates(Board& board);
    bool areGatesOnSameWall(int idx1, int idx2);
    std::pair<int, int> getExitPosition(const std::pair<int, int>& gate, const SnakePiece& head, DIRECTION& newDir);
    bool isWall(const std::pair<int, int>& position);
    DIRECTION getNewDirection(DIRECTION currentDir, std::pair<int, int> newPos, std::pair<int, int> oldPos);
};

#endif // GATE_H
