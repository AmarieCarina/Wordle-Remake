#include "../include/UI.h"

//constructori
UI::UI(float x, float y): position{x,y}, isVisible{true}{}

//getters, setters pentru vizibilitate
void UI::setVisible(bool visible) {isVisible=visible;}
bool UI::getVisible() const {return isVisible;}

//setter pentru vizibilitate
void UI::setPosition(const sf::Vector2f pos){position=pos;}