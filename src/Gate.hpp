// Gate.hpp

// Gate.hpp
#pragma once
#include <ncurses.h>
#include <vector>
#include <ctime>
#include "Snake.hpp"
#include "Board.hpp"
#include "Map.hpp"

class Gate {
public:
    Gate(int maxY, int maxX, int spawnInterval)
        : maxY(maxY), maxX(maxX), spawnInterval(spawnInterval), isActive(false) {
        startTime = time(NULL);
    }

    void initialize(Map& map, Board& board) {
        findWallPositions(map);
    }

    void update(Board& board) {
        time_t now = time(NULL);
        if (isActive) {
            if (!snakeInGate && difftime(now, startTime) > gateLifetime) {
                removeGates(board);
                isActive = false;
                startTime = now + rand() % spawnInterval; // 랜덤 시간 후 게이트 재생성
            }
        } else {
            if (difftime(now, startTime) > spawnInterval) {
                placeGates(board);
                isActive = true;
                startTime = now; // 게이트 생성 시간 초기화
            }
        }
    }

    std::pair<int, int> getNextPosition(const SnakePiece& head, DIRECTION& newDir) {
        if (!isActive) return { -1, -1 };

        snakeInGate = true; // 뱀이 게이트에 들어가는 것을 감지

        if (head.getY() == gate1.first && head.getX() == gate1.second) {
            return getExitPosition(gate2, head, newDir);
        } else if (head.getY() == gate2.first && head.getX() == gate2.second) {
            return getExitPosition(gate1, head, newDir);
        }

        snakeInGate = false; // 뱀이 게이트에서 나왔음을 감지
        return { -1, -1 };
    }

private:
    std::vector<std::pair<int, int>> wallPositions;
    std::pair<int, int> gate1, gate2;
    int maxY, maxX;
    bool isActive;
    bool snakeInGate = false;
    time_t startTime;
    int spawnInterval;
    const int gateLifetime = 10; // 게이트 지속 시간  10초. 10초 지나면 지워짐

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

    void removeGates(Board& board) {  // 게이트 지우기. 
        board.addAt(gate1.first, gate1.second, 'o');
        board.addAt(gate2.first, gate2.second, 'o');
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

