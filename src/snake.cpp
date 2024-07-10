/*
Snake Implementation File
Author: Adam Rubinstein
Date: 07/09/2024
*/

#include "snake.h"
#include "direction.h"
#include <queue>
#include <utility>

#include <iostream>

typedef std::pair<int, int> PairInt;

/* Snake faces towards the back of q - the tail is at q.front() and the head is pushed back. */
struct SnakepImpl {
    std::queue<PairInt> q;
    Direction d;
    PairInt head;
};

/******************** CONSTRUCTORS AND SUCH ********************/
Snake::Snake() {
    snake = (SnakepImpl *)malloc(sizeof(SnakepImpl));

    snake->d = Direction::NONE;
}

Snake::Snake(int i, int j) {
    snake = (SnakepImpl *)malloc(sizeof(SnakepImpl));

    snake->head = PairInt(i, j);
    snake->q.push(snake->head);
    snake->d = Direction::NONE;
}

Snake::Snake(const Snake &s) {
    snake = (SnakepImpl *)malloc(sizeof(SnakepImpl));

    snake->head = s.snake->head;
    snake->q = s.snake->q;
    snake->d = s.snake->d;
}
Snake::Snake(Snake &&s) {
    snake = s.snake;
    s.snake = nullptr;
}

Snake &Snake::operator=(const Snake &s) {
    if (this != &s) {
        snake->head = s.snake->head;
        snake->q = s.snake->q;
        snake->d = s.snake->d;
    }

    return *this;
}
Snake &Snake::operator=(Snake &&s) {
    if (this != &s) {
        snake = s.snake;
        s.snake = nullptr;
    }

    return *this;
}

Snake::~Snake() {
    if (snake != nullptr) {
        free(snake);
    }
}


/******************** GETTERS ********************/
PairInt Snake::head() {
    if (snake->q.empty()) {
        throw std::runtime_error("Snake has no head");
    }
    return snake->head;
}

/******************** SETTERS ********************/
void Snake::move() {
    if (snake->q.empty()) {
        throw std::runtime_error("Snake has no head");
    }
    int i = snake->head.first;
    int j = snake->head.second;

    assert(snake->d != NONE);

    switch (snake->d) {
        case UP:
            i -= 1;
            break;
        case DOWN:
            i += 1;
            break;
        case LEFT:
            j -= 1;
            break;
        case RIGHT:
            j += 1;
            break;
        default:
            break;
    }

    snake->head = std::make_pair(i, j);
    snake->q.push(snake->head);
}

PairInt Snake::remove_tail() {
    if (snake->q.empty()) {
        throw std::runtime_error("Snake has no head");
    }
    PairInt tail = snake->q.front();
    snake->q.pop();

    return tail;
}

void Snake::set_direction(Direction d) {
    Direction opp = NONE;

    switch (d) {
        case UP:
            opp = DOWN;
            break;
        case DOWN:
            opp = UP;
            break;
        case RIGHT:
            opp = LEFT;
            break;
        case LEFT:
            opp = RIGHT;
            break;
        default:
            break;
    }

    if (snake->d != opp) {
        snake->d = d;
    }
}