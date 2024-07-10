/*
Snake Game Header File
Author: Adam Rubinstein
Date: 07/09/2024
*/
#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

struct SnakeGamepImpl;

class SnakeGame {
    public:
        SnakeGame();
        SnakeGame(int size); //Defines the size x size playable grid
        ~SnakeGame();

        void run();
    
    private:
        void init_game(int size);

        void draw();
        void move();
        void game_over();
        
        bool process_input();
        void new_fruit();

        SnakeGamepImpl* game;
};

#endif