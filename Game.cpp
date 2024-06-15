#include "Game.h"

Game::Game(int height, int width, int speed, WINDOW* game, Map& map, int gateSpawnTime)
    : board(height, width, speed, game), snake(), itemManager(height, width), gate(height, width, gateSpawnTime), map(map) {
    initialize();
}

void Game::initialize() {
    board.initialize();
    gameOver = false;
    srand(time(NULL));
    initializeBoardWithMap();
    gate.initialize(map, board);
    randomMission();
}

void Game::initializeSnake(int startY, int startX) {
    snake.setDirection(downD);
}

bool Game::over() const {
    return gameOver;
}

bool Game::missionAchieved() const {
    return snakeSF == 'O' && appleSF == 'O' && poisonSF == 'O' && gateSF == 'O';
}

void Game::randomMission() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(1, 2);

    snakeM = dist(gen);
    appleM = dist(gen);
    poisonM = dist(gen);
    gateM = dist(gen);
}

void Game::missionComplete() {
    if (appleC >= appleM) appleSF = 'O';
    if (poisonC >= poisonM) poisonSF = 'O';
    if (gateC >= gateM) gateSF = 'O';
}

void Game::input() {
    chtype input = board.getInput();
    switch (input) {
        case KEY_UP:
            if (!snake.canChangeDirection(upD)) {
                gameOver = true;
            }
            if (snake.getDirection() != downD) snake.setDirection(upD);
            break;
        case KEY_DOWN:
            if (!snake.canChangeDirection(downD)) {
                gameOver = true;
            }
            if (snake.getDirection() != upD) snake.setDirection(downD);
            break;
        case KEY_LEFT:
            if (!snake.canChangeDirection(leftD)) {
                gameOver = true;
            }
            if (snake.getDirection() != rightD) snake.setDirection(leftD);
            break;
        case KEY_RIGHT:
            if (!snake.canChangeDirection(rightD)) {
                gameOver = true;
            }
            if (snake.getDirection() != leftD) snake.setDirection(rightD);
            break;
        default:
            break;
    }
}

void Game::update() {
    SnakePiece next = snake.nextHead();
    chtype ch = board.getChar(next.getY(), next.getX());

    gate.update(board);

    if (ch == '#') {
        gameOver = true;
    } else {
        if (ch == ' ' || ch == 'A') {
            next = SnakePiece(next.getY(), next.getX(), '@');
            addSnakePiece(next);
            if (ch == 'A') {
                snake.addPiece(next);
                itemManager.removeItemAt(next.getY(), next.getX());
                ++snakeC;
                ++appleC;
            }
        } else if (ch == 'P') {
            if (snake.getLength() > 1) {
                SnakePiece tail = snake.tail();
                board.addEmpty(tail);
                snake.removePiece();
                itemManager.removeItemAt(next.getY(), next.getX());
                --snakeC;
                ++poisonC;
                board.addEmpty(next);
                if (snake.getLength() < 3) {
                    gameOver = true;
                }
            } else {
                gameOver = true;
            }
        } else if (ch == 'o' || ch == '@') {
            gameOver = true;
        } else if (ch == 'G') {
            DIRECTION newDirection;
            auto exitPos = gate.getNextPosition(next, newDirection);
            if (exitPos.first != -1 && exitPos.second != -1) {
                snake.setDirection(newDirection);
                next = SnakePiece(exitPos.first, exitPos.second, '@');
                addSnakePiece(next);
                ++gateC;
            }
        }

        board.scoreUpdate(snakeC, maxSnakeC, appleC, poisonC, gateC);
        missionComplete();
        board.missionUpdate(snakeSF, appleSF, poisonSF, gateSF, snakeM, appleM, poisonM, gateM);
        itemManager.updateItems(board);
        itemManager.drawItems(board);

        checkAndUpdateSpeed();  // 뱀의 길이에 따라 게임 속도를 업데이트

        if (gameOver) {
            displayGameOver();
        }
    }
}

void Game::reDraw() {
    board.refresh();
}

void Game::displayGameOver() {
    WINDOW* win = board.getBoardWin();
    mvwprintw(win, map.mapY / 2, (map.mapX / 2) - 5, "GAME OVER!");
    mvwprintw(win, (map.mapY / 2) + 1, (map.mapX / 2) - 10, "Press any key to exit...");
    wrefresh(win);
    nodelay(win, FALSE);
    wgetch(win);
}

void Game::initializeBoardWithMap() {
    for (int y = 0; y < map.mapY; ++y) {
        for (int x = 0; x < map.mapX; ++x) {
            int value = map.getValue(y, x);
            if (value == 1) {
                board.addAt(y, x, 'o');
            } else if (value == 2) {
                board.addAt(y, x, 'O');
            } else if (value == 3) {
                SnakePiece body(y, x, '#');
                snake.addPiece(body);
                board.add(body);
            } else if (value == 4) {
                SnakePiece head(y, x, '@');
                snake.addPiece(head);
                board.add(head);
            }
        }
    }
}

void Game::addSnakePiece(SnakePiece next) {
    if (snake.getLength() > 0) {
        SnakePiece head = snake.head();
        board.add(SnakePiece(head.getY(), head.getX(), '#'));
    }
    board.add(next);
    snake.addPiece(next);
    if (snake.getLength() > 3) {
        SnakePiece tail = snake.tail();
        board.addEmpty(tail);
        snake.removePiece();
    }
}

void Game::checkAndUpdateSpeed() {
    if (snake.getLength() > 10) {
        board.setSpeed(100);  // 뱀의 길이가 10을 초과하면 속도를 빠르게 설정
    } else {
        board.setSpeed(200);  // 기본 속도 설정
    }
}