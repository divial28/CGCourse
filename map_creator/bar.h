#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Generator;

class Button
{
public:

    sf::RectangleShape blocks[3][3];
    int x;
    int y;
    int size;

    Button(int x, int y, int size);
    bool isBound(int mx, int my);
    void draw(sf::RenderWindow* window);
};

class Bar
{
public:
    Bar(int x, int y, int bSize, bool horizontal = false);

    std::string handle(sf::Event* ev);

    void draw(sf::RenderWindow* window);

private:
    int focus;
    int size;
    int x;
    int y;
    int bSize;
    int spacer;
    bool horizontal;
    std::vector<Button> buttons;
    sf::RectangleShape focusShape;
};