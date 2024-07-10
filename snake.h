/*
Snake Header File
Author: Adam Rubinstein
Date: 07/09/2024
*/
#ifndef SNAKE_H
#define SNAKE_H

#include <utility>
#include "direction.h"

struct SnakepImpl;

class Snake {
    public:
        Snake();
        Snake(int i, int j);
        Snake(const Snake &other);
        Snake(Snake &&other);
        Snake &operator=(const Snake &other);
        Snake &operator=(Snake &&other);
        ~Snake();

        /******************** SETTERS ********************/
        // Move the snake in the direction it is facing
        void move();

        void set_direction(Direction d);
        // Returns the coordinates of the removed tail
        std::pair<int, int> remove_tail();

        /******************** GETTERS ********************/
        std::pair<int, int> head();

    private:
        SnakepImpl *snake;
};

#endif