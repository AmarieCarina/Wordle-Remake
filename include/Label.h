#ifndef POO2_LABEL_H
#define POO2_LABEL_H
#include "UI.h"

class Label : public UI {
    sf::Text text;
public:
    Label(float x, float y, const std::string& content, const sf::Font& font, unsigned int size = 24);

    void setText(const std::string& content);

    void draw(sf::RenderWindow& window) override;
};

#endif //POO2_LABEL_H