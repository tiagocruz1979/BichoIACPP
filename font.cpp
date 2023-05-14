
#include "font.h"

FontManager& FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

sf::Font& FontManager::getFont() {
    return font;
}

FontManager::FontManager() {
    font.loadFromFile("recursos/consola.ttf");
}

FontManager& fontManager = FontManager::getInstance();
