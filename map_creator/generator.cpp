#include "generator.h"

std::map<std::string, Layer*>   Generator::layers;
std::map<char, sf::Color>       Generator::colors;

Generator::Generator()
{
    parseColors();
    parseLayers();
    parseTiles();

}
void Generator::parseColors()
{
    std::ifstream in("../colors");
    if(!in)
    {
        std::cout << "cant open colors file" << std::endl;
        return;
    }
    std::string s;
    while(std::getline(in, s))
    {
        sf::Color color;
        char id = s.at(0);
        //std::cout << id << ": ";

        std::getline(in, s);
        std::stringstream ss(s);
        ss >> s; color.r = std::stoi(s);
        //std::cout  << std::stoi(s) << ",";
        ss >> s; color.g = std::stoi(s);
        //std::cout  << s << ",";
        ss >> s; color.b = std::stoi(s);
        //std::cout  << s << ",";
        ss >> s; color.a = std::stoi(s);
        //std::cout  << s << std::endl;

        
        colors[id] = color;
    }
}

void Generator::parseLayers()
{
    std::ifstream in("../layers");
    if(!in)
    {
        std::cout << "cant open layers file" << std::endl;
        return;
    }

    std::string s;
    while(std::getline(in, s))
    {
        if(s.front() == '/')
            continue;

        std::string id = s;
        std::string blocks;
        for(int i = 0; i < 3; i++)
        {
            std::getline(in, s);
            blocks += s;
        }
        //std::cout << id << ": " << blocks << std::endl;
        layers[id] = new Layer(blocks);
    }
}

void Generator::parseTiles()
{

}