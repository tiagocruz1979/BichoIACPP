#include "parede.h"

    Parede::Parede(float posX, float posY)
    {
        this->corpo.setFillColor(sf::Color::Cyan);
        this->corpo.setSize(sf::Vector2f(10.f,10.f));
        setPosition(posX,posY);
    }

    void Parede::setPosition(float x, float y)
    {
        corpo.setPosition(sf::Vector2f(x,y));
    }

    void Parede::draw(sf::RenderWindow &w)
    {
        w.draw(this->corpo);
    }

    sf::Shape& Parede::getShape()
    {
        return this->corpo;
    }
