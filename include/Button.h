#ifndef POO2_BUTTON_H
#define POO2_BUTTON_H
#include <SFML/Graphics.hpp>
#include "UI.h"
#include <functional>

class Button : public UI {
protected:
    sf::RectangleShape shape;
    sf::Text label;
    //functie pentru interactiune cu mai multe tipuri de butoane
    std::function<void()> onClick;
public:
    Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font);

    bool isMouseOver(sf::Vector2f mousePos) const;

    //pentru setare std::function
    void setOnClick(std::function<void()> callback);

    void click();

    void draw(sf::RenderWindow& window) override;
};

#endif //POO2_BUTTON_H