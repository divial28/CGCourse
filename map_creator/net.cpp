#include "net.h"
#include "generator.h"

Net::Net()
{

}

Net::Net(int x, int y, int w, int h, int size, int spacer)
    : x(x), y(y), w(w), h(h), ts(size), s(spacer), focus(-1)
    , wl(w/3), hl(h/3), ls(size*3), currentLayer("N")
    , focusShape(sf::Vector2f(3*size + 2*s, 3*size + 2*s))
{
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            blocks.emplace_back(sf::Vector2f(size, size));
            blocks.back().setFillColor(sf::Color(0x0, 0x0, 0x0, 0x0));
            blocks.back().setOutlineColor(sf::Color::Black);
            blocks.back().setOutlineThickness(s);
            blocks.back().setPosition(sf::Vector2f(x + j*(size+s), y + i*(size+s) ));
        }
    }

    layers.resize(wl*hl);

    focusShape.setFillColor(sf::Color(0x0, 0x0, 0x0, 0x0));
    focusShape.setOutlineColor(sf::Color::Red);
    focusShape.setOutlineThickness(s);

    /*test
    std::vector<std::string> layers = {"N", "G", "F", "CF", "SF", "#", "D", "C#", "W"
                                      , "RL", "RM", "RH", "CRL", "CRM", "RSC", "RC"};

    for(int n = 0 ; n < 30; n+=3)
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {

                char id = Generator::layers[layers[n/3]]->blocks[i][j];
                sf::Color color = Generator::colors[id];
                blocks[(i+n)*w + j].setFillColor(color);
            }
        }
    }
    for(int n = 0 ; n < 18; n+=3)
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {

                char id = Generator::layers[layers[10+n/3]]->blocks[i][j];
                sf::Color color = Generator::colors[id];
                blocks[(i+n)*w + j + 5].setFillColor(color);
            }
        }
    }
    /*test*/
}

void Net::draw(sf::RenderWindow * window, Net * parent)
{
    /*if(parent)
    {
        sf::Vector2f shift = getPos() - parent->getPos();
        parent->moveTiles(shift);
        parent->draw(window, 0);
        shift = -shift;
        parent->moveTiles(shift);
    }*/

    for(int i = 0; i < blocks.size(); i++)
    {
        window->draw(blocks[i]); 
    }

    if(focus >= 0)
        window->draw(focusShape);
}

void Net::moveTiles(sf::Vector2f& shift)
{
    for(int i = 0; i < blocks.size(); i++)
        blocks[i].move(shift);
}

void Net::handleMouseEvent(sf::Event * ev)
{
    int mx = (ev->type == sf::Event::MouseMoved) ? ev->mouseMove.x : ev->mouseButton.x;
    int my = (ev->type == sf::Event::MouseMoved) ? ev->mouseMove.y : ev->mouseButton.y;

    if (isBound(mx, my))
    {
        int j = (mx-x) / (ls + 3*s);
        int i = (my-y) / (ls + 3*s);
        
        if(ev->type == sf::Event::MouseButtonPressed)
        {
            setLayer(i, j);
        }

        if(ev->type == sf::Event::MouseMoved)
        {
            focus = i*w + j;
            focusShape.setPosition(blocks[focus*3].getPosition());
            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                setLayer(i, j);
        }
    }
    else if(ev->type == sf::Event::MouseMoved && focus >= 0)
    {
        focus = -1;
    }
}

void Net::setLayer(int i, int j)
{
    //std::cout << currentLayer << std::endl;
    layers[i*wl + j] = Generator::layers[currentLayer];

    int id = (i*w + j)*3;
    for(int k = 0; k < 3; k++)
    {
        for(int l = 0; l < 3; l++)
        {
            char key = Generator::layers[currentLayer]->blocks[k][l];
            sf::Color color = Generator::colors[key];
            //std::cout << id << " ";
            blocks[id + k*w + l].setFillColor(color);        
        }
        //std::cout << std::endl;
    }
    //std::cout << std::endl;

}

bool Net::isBound(int mx, int my)
{
    return mx > x && mx < x + w*(ts+s) && my > y && my < y + h*(ts+s);
}

void Net::setCurrentLayer(std::string layerID)
{
    currentLayer = layerID;
}

sf::Vector2f Net::getPos()
{
    return sf::Vector2f(x, y);
}