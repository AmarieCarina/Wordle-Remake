#include "Button.h"
bool Button::isMouseOver(sf::Vector2f mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}
void Button::click() {
    if (onClick){onClick();}//executa functia, daca exista
}
//un fel de setter pentru un bloc de cod intreg
void Button::setOnClick(std::function<void()> callback) {
    //callback - functia pe care o trimitem din exterior, catre clasa
    onClick = callback;
}
Button::Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font)
    :   UI{x,y}, //pozitia din clasa de baza
        shape{{width,height}},
        label{font,text,25}{

    //forma butonului
    shape.setPosition({x,y});
    shape.setFillColor(sf::Color(100,100,100));//gri neutru
    // shape.setOutlineThickness(2);
    // shape.setOutlineColor(sf::Color::White);

    //textul din buton (label)
    label.setFillColor(sf::Color::White);

    //centrare label
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });
    label.setPosition({
        x + width / 2.0f,
        y + height / 2.0f
    });

}

void Button::draw(sf::RenderWindow& window) {
    if (!isVisible){return;}
    window.draw(shape);
    window.draw(label);
}