#include "food.h"


    Food::Food()
    {
        this->comida.setFillColor(sf::Color::Magenta);
        this->comida.setSize(sf::Vector2f(10.f,10.f));
        this->comida.setPosition(sf::Vector2f(-100.f,-100.f));
    }
    void Food::setPosition(float x, float y)
    {
        this->comida.setPosition(sf::Vector2f(x,y));
    }

    void Food::draw(sf::RenderWindow &w)
    {
        w.draw(this->comida);
    }

    double Food::getEnergia()
    {
        return 100.0;
    }

    bool Food::intersects(const sf::Shape &s)
    {
       return this->comida.getGlobalBounds().intersects(s.getGlobalBounds());
    }

    sf::Shape &Food::getShape()
    {
        return this->comida;
    }
