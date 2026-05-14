#include "Label.h"
#include <SFML/Graphics.hpp>
Label::Label(float x, float y, const std::string& content, const sf::Font& font, unsigned int size)
    : UI{x,y},
    text{font,content,size}
{
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.size.x / 2.0f,
        bounds.size.y / 2.0f});

    text.setPosition({x,y});
}

void Label::setText(const std::string& content) {
    text.setString(content);
}

void Label::draw(sf::RenderWindow& window) {
    if (isVisible) {
        window.draw(text);
    }
}