#ifndef FOOD_H_INCLUDED
#define FOOD_H_INCLUDED

#include <SFML/Graphics.hpp>

class Food
{
    sf::RectangleShape comida;

public:
    Food();
    void setPosition(float x, float y);
    void draw(sf::RenderWindow &w);
    double getEnergia();
    bool intersects(const sf::Shape &s);
    sf::Shape &getShape();
};

#endif // FOOD_H_INCLUDED
