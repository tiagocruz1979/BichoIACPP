#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED
#include <SFML/Graphics.hpp>

// font.h
#ifndef FONT_H
#define FONT_H

#include <SFML/Graphics.hpp>

class FontManager
{
public:
    static FontManager& getInstance();
    sf::Font& getFont();

private:
    FontManager();
    FontManager(FontManager const&) = delete;
    void operator=(FontManager const&) = delete;

    sf::Font font;
};

extern FontManager& fontManager;

#endif

// font.cpp





#endif // FONT_H_INCLUDED
