#include "../include/PopUp.h"

void PopUp::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    //desenam fundalul
    window.draw(background);
    //desenam elementele din fundal
    for (const auto& el : elements) {
        el->draw(window);
    }
}

void PopUp::addElement(std::unique_ptr<UI> element) {
    elements.push_back(std::move(element));
    //std::move pt ca unique_ptr nu poate fi copiat
}

PopUp::PopUp(float x, float y, float width, float height)
    :UI(x,y) {

    //width, height - parametri
    background.setSize({width,height});

    //UI::position
    background.setPosition(position);

    //culoare de fundal - gri spre negru
    background.setFillColor(sf::Color(30,30,30,240));
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color::White);
}

void PopUp::clear() {
    elements.clear();
}

