// Gate.hpp
#pragma once
#include <ncurses.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Snake.hpp"
#include "Board.hpp"
#include "Map.hpp"

class Gate {
public:
    Gate(int maxY, int maxX, int spawnTime)
        : maxY(maxY), maxX(maxX), spawnTime(spawnTime), isActive(false) {
        startTime = time(NULL);
    }

    void initialize(Map& map, Board& board) {
        findWallPositions(map);
    }

    void update(Board& board) {
        if (!isActive && difftime(time(NULL), startTime) > spawnTime) {
            placeGates(board);
            isActive = true;
        }
    }

    std::pair<int, int> getNextPosition(const SnakePiece& head, DIRECTION& newDir) {
        if (!isActive) return { -1, -1 };

        if (head.getY() == gate1.first && head.getX() == gate1.second) {
            return getExitPosition(gate2, head, newDir);
        } else if (head.getY() == gate2.first && head.getX() == gate2.second) {
            return getExitPosition(gate1, head, newDir);
        }
        return { -1, -1 };
    }

private:
    std::vector<std::pair<int, int>> wallPositions;
    std::pair<int, int> gate1, gate2;
    int maxY, maxX;
    bool isActive;
    time_t startTime;
    int spawnTime;

    void findWallPositions(Map& map) {
        wallPositions.clear();
        for (int y = 0; y < map.mapY; ++y) {
            for (int x = 0; x < map.mapX; ++x) {
                if (map.getValue(y, x) == 1) { // 일반 벽 위치
                    wallPositions.push_back({ y, x });
                }
            }
        }
    }

    void placeGates(Board& board) {
        if (wallPositions.size() < 2) return; // 최소 두 개의 게이트 필요

        do {
            int idx1 = rand() % wallPositions.size();
            int idx2;
            do {
                idx2 = rand() % wallPositions.size();
            } while (idx2 == idx1 || areGatesOnSameWall(idx1, idx2));

            gate1 = wallPositions[idx1];
            gate2 = wallPositions[idx2];
        } while (areGatesOnSameWall());

        board.addAt(gate1.first, gate1.second, 'G');
        board.addAt(gate2.first, gate2.second, 'G');
    }

    bool areGatesOnSameWall(int idx1, int idx2) {
        // 같은 벽에 있는지 확인
        return (wallPositions[idx1].first == wallPositions[idx2].first) ||
               (wallPositions[idx1].second == wallPositions[idx2].second);
    }

    bool areGatesOnSameWall() {
        // 같은 벽에 있는지 확인
        return (gate1.first == gate2.first) || (gate1.second == gate2.second);
    }

    std::pair<int, int> getExitPosition(const std::pair<int, int>& gate, const SnakePiece& head, DIRECTION& newDir) {
        int y = gate.first;
        int x = gate.second;

        // 상단 벽
        if (y == 0) {
            newDir = downD;
            return { y + 1, x };
        }
        // 하단 벽
        if (y == maxY - 1) {
            newDir = upD;
            return { y - 1, x };
        }
        // 좌측 벽
        if (x == 0) {
            newDir = rightD;
            return { y, x + 1 };
        }
        // 우측 벽
        if (x == maxX - 1) {
            newDir = leftD;
            return { y, x - 1 };
        }

        // 맵 가장자리 벽이 아닌 경우, 들어간 방향으로 나옴
        newDir = head.getDirection();
        switch (newDir) {
            case downD: return { y + 1, x };
            case upD: return { y - 1, x };
            case rightD: return { y, x + 1 };
            case leftD: return { y, x - 1 };
        }

        return { -1, -1 };
    }
};
