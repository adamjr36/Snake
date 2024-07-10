/*
Snake Game Implementation File
Author: Adam Rubinstein
Date: 07/09/2024
*/

#include <iostream>
#include <unistd.h>

#include "snake_game.h"
#include "snake.h"
#include "direction.h"
#include "termfuncs.h"

// CONSTANTS //
const char HEAD = 'A';
const char TAIL = 'Z';
const char FRUIT = 'F';

const std::string SNAKE_COLOR = "green";
const std::string FRUIT_COLOR = "red";

#define VERT_BORDER '-'
#define HORZ_BORDER '|'
#define SIZE 12

//Game State Struct
struct SnakeGamepImpl {
    char **grid;
    Snake snake;

    int size;
    char game_over;
};
////

/******************** CONSTRUCTORS AND SUCH ********************/
SnakeGame::SnakeGame() {
    init_game(SIZE);
}

SnakeGame::SnakeGame(int size) {
    init_game(size);
}

// Constructor. mallocs: SnakeGameImpl, size x size grid, Snake
void SnakeGame::init_game(int size) {
    assert(size >= 10);
    game = new SnakeGamepImpl;
    game->size = size;
    game->game_over = 0;

    // GRID
    char** grid = (char **)malloc(size * sizeof(char *));
    game->grid = grid;
    // Initialize grid to empty spaces
    for (int i = 0; i < size; i++) {
        grid[i] = (char *)malloc(size * sizeof(char *));        
        for (int j = 0; j < size; j++) {
            grid[i][j] = ' ';
        }
    }

    // NEW SNAKE
    int snake_row = std::rand() % size;
    int snake_col = std::rand() % size;
    game->grid[snake_row][snake_col] = HEAD;
    game->snake = Snake(snake_row, snake_col);

    // NEW FRUIT
    new_fruit();
}

SnakeGame::~SnakeGame() {
    for (int i = 0; i < game->size; i++) {
        free(game->grid[i]);
    }
    free(game->grid);
    free(game);
}


/******************** PUBLIC RUN ********************/
void SnakeGame::run() {
    // Wait for the first key press
    screen_clear();
    draw();
    while (!process_input()) {}

    // Main game loop
    while (!(game->game_over)) {
        screen_clear();
        draw();

        usleep(200000);
        move();
    }
}


/******************** GAME RUN HELPERS ********************/

// Process input and return if it was a valid move
// TODO: Process inputs in a separate thread
bool SnakeGame::process_input() {
    int d = (int)getacharnow(1); //Need to get last char from stdin buffer here

    char v = d == Direction::LEFT || d == Direction::RIGHT || d == Direction::UP || d == Direction::DOWN;
    if (v) {
        game->snake.set_direction((Direction)d);
    }
    return v;
}


void SnakeGame::move() {
    process_input();

    // Get old head (to turn it into part of the tail later) and move the snake
    std::pair<int, int> old_head = game->snake.head();
    game->snake.move();
    std::pair<int, int> head = game->snake.head();
    int row = head.first;
    int col = head.second;
    // row and col represent the new head position

    // Check we didn't die
    // Running into the edge
    if (row < 0 || col < 0 || row >= game->size || col >= game->size) {
        game_over();
        return;
    }
    // Set old head to tail, then check if we ran into the tail
    game->grid[old_head.first][old_head.second] = TAIL;
    char next_head = game->grid[row][col];
    if (next_head == TAIL) {
        game_over();
    } 
    
    // Move the snake on the board and remove the tail if we didn't eat fruit
    else if (next_head == ' ') {
        std::pair<int, int> tail = game->snake.remove_tail();
        game->grid[tail.first][tail.second] = ' ';
    } else {
        new_fruit();
    }
    game->grid[row][col] = HEAD;
}

void SnakeGame::game_over() {
    std::cout << "GAME OVER :(((((" << std::endl;
    game->game_over = 1;
}

// Spawn new fruit on an empty sq
// TODO: If every square is full of snake you win? 
// TODO: Also maybe should just increment after first randomization. This algorithm is silly
void SnakeGame::new_fruit() {
    int size = game->size;
    int size_sq = size * size;
    char** grid = game->grid;

    int i = std::rand() % size_sq;
    int row = i / size;
    int col = i % size;

    while (grid[row][col] != ' ') {
        i = std::rand() % size_sq;
        row = i / size;
        col = i % size;
    }

    grid[row][col] = FRUIT;
}


/******************** DRAWING ********************/
void print_dashed_line(int size) {
    for (int i = 0; i < size; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;
}

// Draw current game state
// TODO: OpenGL. This is not pretty.
void SnakeGame::draw() {
    int size = game->size;
    char ** grid = game-> grid;

    print_dashed_line(size * 2 + 1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << '|';

            char c = grid[i][j];
            if (c == HEAD || c == TAIL) {
                screen_fg(SNAKE_COLOR);
                std::cout << c;
                screen_fg("white");
            } else if (c == FRUIT) {
                screen_fg(FRUIT_COLOR);
                std::cout << c;
                screen_fg("white");
            } else {
                std::cout << c;
            }
        }
        std::cout << '|';
        std::cout << std::endl;
        print_dashed_line(size * 2 + 1);
    }
}