#ifndef OBSTACULO_H_INCLUDED
#define OBSTACULO_H_INCLUDED

#include <SFML/Graphics.hpp>
class Obstaculo
{
    sf::RectangleShape corpo;

public:
    Obstaculo(float posX, float posY);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow &w);
    sf::Shape &getShape();
};


#endif // OBSTACULO_H_INCLUDED
