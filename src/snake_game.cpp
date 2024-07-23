/*
Snake Game Implementation File
Author: Adam Rubinstein
Date: 07/09/2024
*/

#include <iostream>
#include <unistd.h>
#include <thread>
#include <termios.h>

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
    char *grid;
    Snake snake;
    std::thread input_thread;

    int size;
    short int empty_squares;
    char game_started;
    char game_over;

    SnakeGamepImpl(int size, int snake_row, int snake_col) 
        : grid(new char[size * size]), 
        snake(Snake(snake_row, snake_col)),
        size(size), empty_squares(size * size), game_started(0), game_over(0) {
            for (int i = 0; i < size * size; i++) {
                grid[i] = ' ';
            }
    }
    ~SnakeGamepImpl() {
        delete[] grid;
        input_thread.join();
    }

    char *grid_at(int x, int y) { // No bounds checking
        return &(grid[x * size + y]);
    }

};
////


/******************** CONSTRUCTORS AND SUCH ********************/
SnakeGame::SnakeGame() {
    init_game(SIZE);
}

SnakeGame::SnakeGame(int size) {
    init_game(size);
}

// Constructor. mallocs: SnakeGameImpl
void SnakeGame::init_game(int size) {
    assert(size >= 10);

    // Snake position
    int snake_row = std::rand() % size;
    int snake_col = std::rand() % size;

    game = new SnakeGamepImpl(size, snake_row, snake_col);
    fill_square(snake_row, snake_col, HEAD);

    // First fruit position
    new_fruit();
}

SnakeGame::~SnakeGame() {
    delete game;
}


/******************** PUBLIC RUN ********************/
void SnakeGame::run() {
    // Wait for the first key press
    screen_clear();
    draw();
    // while (!process_input()) {}
    process_input();
    while (!(game->game_started)) {
        usleep(100000);
    }

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
void SnakeGame::process_input() {
    game->input_thread = std::thread(&SnakeGame::input_loop, this);
}

void SnakeGame::input_loop() {
    // Term stuff
    struct termios  info, orig;
    tcgetattr(0, &info);
    orig = info;
    info.c_lflag    &= ~ECHO;
    info.c_lflag    &= ~ICANON;
    info.c_cc[VMIN]  = 0;
    info.c_cc[VTIME] = 1; // deciseconds
    tcsetattr(0, TCSANOW, &info);
    //

    int c, r;
    while (!(game->game_over)) {
        read(0, &r, 4);
        c = r & 255;
        // std::cerr << r << std::endl;
        if (r == 0) {
            continue;
        } 
        if (c == 'q' || r == EOF) {
            game->game_over = 1;
            break;
        } 

        if (c == Direction::LEFT   || 
            c == Direction::RIGHT  || 
            c == Direction::UP     || 
            c == Direction::DOWN) {
            game->game_started = 1;
            game->snake.set_direction((Direction)c);
        }
    }

    // Restore term stuff
    tcsetattr(0, TCSANOW, &orig);
}


void SnakeGame::move() {
    // process_input();

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
        std::cerr << "Ran into the edge" << std::endl;
        game_over();
        return;
    }
    // Set old head to tail, then check if we ran into the tail
    change_square(old_head.first, old_head.second, TAIL);
    char next_head = *(game->grid_at(row, col));
    if (next_head == TAIL) {
        std::cerr << "Ran into the tail" << std::endl;
        game_over();
    } 
    
    // Move the snake on the board and remove the tail if we didn't eat fruit
    else if (next_head == ' ') {
        std::pair<int, int> tail = game->snake.remove_tail();
        empty_square(tail.first, tail.second);
        fill_square(row, col, HEAD);
    } else {
        new_fruit();
        change_square(row, col, HEAD);
    }
    
}

void SnakeGame::game_over() {
    std::cout << "GAME OVER :(((((" << std::endl;
    game->game_over = 1;
}

void SnakeGame::game_win() {
    std::cout << "YOU WIN! :))))))" << std::endl;
    game->game_over = 1;
}

// Spawn new fruit on an empty sq
// TODO: If every square is full of snake you win? 
void SnakeGame::new_fruit() {
    if (game->empty_squares == 0) {
        game_win();
        return;
    }

    int size = game->size;
    int size_sq = size * size;


    int i = std::rand() % size_sq;
    int row = i / size;
    int col = i % size;

    while (*(game->grid_at(row, col)) != ' ') {
        i = (i + 1) % size_sq;
        row = i / size;
        col = i % size;
    }

    fill_square(row, col, FRUIT);
}

void SnakeGame::empty_square(int row, int col) {
    *(game->grid_at(row, col)) = ' ';
    game->empty_squares++;
}

void SnakeGame::fill_square(int row, int col, char c) {
    *(game->grid_at(row, col)) = c;
    game->empty_squares--;
}

void SnakeGame::change_square(int row, int col, char c) {
    *(game->grid_at(row, col)) = c;
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

    print_dashed_line(size * 2 + 1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << '|';

            char c = *(game->grid_at(i, j));
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