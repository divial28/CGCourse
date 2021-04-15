#include "layer.h"

Layer::Layer()
{

}

Layer::Layer(int x, int y, int w, int h, int size, int spacer)
    : x(x), y(y), w(w), h(h), ts(size), s(spacer), focus(-1), currentColor(0x22, 0xa8, 0x03)
    , focusShape(sf::Vector2f(size, size))
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            vTiles.emplace_back(sf::Vector2f(size, size));
            vTiles.back().setFillColor(sf::Color::White);
            vTiles.back().setOutlineColor(sf::Color::Black);
            vTiles.back().setOutlineThickness(s);
            vTiles.back().setPosition(sf::Vector2f(x + j*(size+s), y + i*(size+s) ));
        }
    }

    focusShape.setFillColor(sf::Color(0x0, 0x0, 0x0, 0x0));
    focusShape.setOutlineColor(sf::Color::Red);
    focusShape.setOutlineThickness(s);
}

void Layer::draw(sf::RenderWindow * window)
{
    for(int i = 0; i < vTiles.size(); i++)
    {
        window->draw(vTiles[i]); 
    }

    if(focus >= 0)
        window->draw(focusShape);
}

void Layer::handleMouseEvent(sf::Event * ev)
{
    int mx = (ev->type == sf::Event::MouseMoved) ? ev->mouseMove.x : ev->mouseButton.x;
    int my = (ev->type == sf::Event::MouseMoved) ? ev->mouseMove.y : ev->mouseButton.y;

    if (isBound(mx, my))
    {
        int j = (mx-x) / (ts + s);
        int i = (my-y) / (ts + s);
        
        if(ev->type == sf::Event::MouseButtonPressed)
        {
            vTiles[i*w + j].setFillColor(currentColor);
        }

        if(ev->type == sf::Event::MouseMoved)
        {
            focus = i*w + j;
            focusShape.setPosition(vTiles[focus].getPosition());
            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                vTiles[i*w + j].setFillColor(currentColor);
        }
    }
    else if(ev->type == sf::Event::MouseMoved && focus >= 0)
    {
        focus = -1;
    }
}

bool Layer::isBound(int mx, int my)
{
    return mx > x && mx < x + w*(ts+s) && my > y && my < y + h*(ts+s);
}

void Layer::setCurrentColor(sf::Color& color)
{
    currentColor = color;
}