#ifndef PAREDE_H_INCLUDED
#define PAREDE_H_INCLUDED

#include <SFML/Graphics.hpp>
class Parede
{
    sf::RectangleShape corpo;

public:
    Parede(float posX, float posY);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow &w);
    sf::Shape &getShape();
};


#endif // PAREDE_H_INCLUDED
