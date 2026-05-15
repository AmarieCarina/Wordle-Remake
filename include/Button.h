#ifndef POO2_BUTTON_H
#define POO2_BUTTON_H
#include <SFML/Graphics.hpp>
#include "UI.h"
#include <functional>
#include "ButtonObserver.h"

class Button : public UI {
protected:
    sf::RectangleShape shape;
    sf::Text label;
    //functie pentru interactiune cu mai multe tipuri de butoane
    std::vector<ButtonObserver*> observers;
    std::string actionName;
public:
    Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font);

    bool isMouseOver(sf::Vector2f mousePos) const;

    void draw(sf::RenderWindow& window) override;

    void addObserver(ButtonObserver* obs) { observers.push_back(obs); }
    void setAction(const std::string& action) {
        actionName = action;
    }

    void click() const {
        for (auto const obs : observers) obs->onButtonClick(actionName);
    }
};

#endif //POO2_BUTTON_H