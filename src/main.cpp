/*
Entry point for the snake game
Author: Adam Rubinstein
Date: 07/09/2024
*/
#include <iostream>
#include "snake_game.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define SIZE 12

int main(int argc, char *argv[])
{
    int size = SIZE;

    if (argc > 1) {
        // Usage
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            std::cout << "Usage: " << argv[0] << "[size | -h | --help]" << std::endl;
            std::cout << "size: the size of the game board" << std::endl;
            std::cout << "This is a simple snake game. Use the awsd keys to move the snake and eat the fruit." << std::endl;
            return 0;
        } else {
            // Parse size arg
            try {
                size = std::stoi(argv[1]);
                if (size < 10 || size > 25) {
                    std::cerr << "ERROR: Size must be in the range [10, 25]" << std::endl;
                    return 1;
                }
            } catch (std::invalid_argument &e) {
                std::cerr << "Invalid argument: " << argv[1] << std::endl;
                return 1;
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    // SnakeGame game(size);
    // game.run();

    return 0;
}