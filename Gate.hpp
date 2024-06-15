// Gate.hpp

#pragma once
#include <ncurses.h>
#include <vector>
#include <ctime>
#include <algorithm> // std::any_of
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
        findEmptyPositions(map);
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
    std::vector<std::pair<int, int>> emptyPositions;
    std::pair<int, int> gate1, gate2;
    int maxY, maxX;
    bool isActive;
    bool snakeInGate = false;
    time_t startTime;
    int spawnInterval;
    const int gateLifetime = 10; // 게이트 지속 시간 10초. 10초 지나면 지워짐

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

    void findEmptyPositions(Map& map) {
        emptyPositions.clear();
        for (int y = 0; y < map.mapY; ++y) {
            for (int x = 0; x < map.mapX; ++x) {
                if (map.getValue(y, x) == 0) { // 빈 공간 위치
                    emptyPositions.push_back({ y, x });
                }
            }
        }
    }

    void placeGates(Board& board) {
        if (wallPositions.empty() || emptyPositions.empty()) return; // 벽과 빈 공간 위치가 없으면 리턴

        int gateType = rand() % 2; // 0이면 벽-벽, 1이면 벽-빈 공간

        if (gateType == 0) {
            // 벽-벽 게이트 생성
            int idx1 = rand() % wallPositions.size();
            int idx2;
            do {
                idx2 = rand() % wallPositions.size();
            } while (idx2 == idx1 || areGatesOnSameWall(idx1, idx2));

            gate1 = wallPositions[idx1];
            gate2 = wallPositions[idx2];
        } else {
            // 벽-빈 공간 게이트 생성
            int idx1 = rand() % wallPositions.size();
            int idx2 = rand() % emptyPositions.size();

            gate1 = wallPositions[idx1];
            gate2 = emptyPositions[idx2];
        }

        board.addAt(gate1.first, gate1.second, 'G');
        board.addAt(gate2.first, gate2.second, 'G');
    }

    void removeGates(Board& board) {
        board.addAt(gate1.first, gate1.second, 'o');
        board.addAt(gate2.first, gate2.second, isWall(gate2) ? 'o' : ' ');
    }

    bool areGatesOnSameWall(int idx1, int idx2) {
        // 같은 벽에 있는지 확인
        return (wallPositions[idx1].first == wallPositions[idx2].first) ||
               (wallPositions[idx1].second == wallPositions[idx2].second);
    }

    std::pair<int, int> getExitPosition(const std::pair<int, int>& gate, const SnakePiece& head, DIRECTION& newDir) {
        int y = gate.first;
        int x = gate.second;

        // 중간에 있을 때 나가는 방향 결정
        if (y > 0 && y < maxY - 1 && x > 0 && x < maxX - 1) {
            std::vector<std::pair<int, int>> directions = {
                {y + 1, x}, // down
                {y, x + 1}, // right
                {y - 1, x}, // up
                {y, x - 1}  // left
            };

            // 진입 방향과 일치하는 방향, 시계방향, 역시계방향, 반대 방향 순으로 검사
            switch (head.getDirection()) {
                case downD:
                    directions = {{y + 1, x}, {y, x + 1}, {y, x - 1}, {y - 1, x}};
                    break;
                case upD:
                    directions = {{y - 1, x}, {y, x - 1}, {y, x + 1}, {y + 1, x}};
                    break;
                case rightD:
                    directions = {{y, x + 1}, {y - 1, x}, {y + 1, x}, {y, x - 1}};
                    break;
                case leftD:
                    directions = {{y, x - 1}, {y + 1, x}, {y - 1, x}, {y, x + 1}};
                    break;
            }

            for (const auto& dir : directions) {
                if (dir.first >= 0 && dir.first < maxY && dir.second >= 0 && dir.second < maxX &&
                    !isWall(dir)) {
                    newDir = getNewDirection(head.getDirection(), dir, {y, x});
                    return dir;
                }
            }
        }

        // 가장자리에 있을 때 기존 로직 유지
        if (y == 0) {
            newDir = downD;
            return { y + 1, x };
        }
        if (y == maxY - 1) {
            newDir = upD;
            return { y - 1, x };
        }
        if (x == 0) {
            newDir = rightD;
            return { y, x + 1 };
        }
        if (x == maxX - 1) {
            newDir = leftD;
            return { y, x - 1 };
        }

        newDir = head.getDirection();
        switch (newDir) {
            case downD: return { y + 1, x };
            case upD: return { y - 1, x };
            case rightD: return { y, x + 1 };
            case leftD: return { y, x - 1 };
        }

        return { -1, -1 };
    }

    bool isWall(const std::pair<int, int>& position) {
        return std::any_of(wallPositions.begin(), wallPositions.end(),
                           [&position](const std::pair<int, int>& wallPos) {
                               return wallPos == position;
                           });
    }

    DIRECTION getNewDirection(DIRECTION currentDir, std::pair<int, int> newPos, std::pair<int, int> oldPos) {
        int yDiff = newPos.first - oldPos.first;
        int xDiff = newPos.second - oldPos.second;

        if (yDiff == 1) return downD;
        if (yDiff == -1) return upD;
        if (xDiff == 1) return rightD;
        if (xDiff == -1) return leftD;

        return currentDir;
    }
};
