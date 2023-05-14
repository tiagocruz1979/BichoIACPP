#include "obstaculo.h"

    Obstaculo::Obstaculo(float posX, float posY)
    {
        this->corpo.setFillColor(sf::Color::Blue);
        this->corpo.setSize(sf::Vector2f(10.f,10.f));
        setPosition(posX,posY);
    }

    void Obstaculo::setPosition(float x, float y)
    {
        corpo.setPosition(sf::Vector2f(x,y));
    }

    void Obstaculo::draw(sf::RenderWindow &w)
    {
        w.draw(this->corpo);
    }

    sf::Shape& Obstaculo::getShape()
    {
        return this->corpo;
    }
