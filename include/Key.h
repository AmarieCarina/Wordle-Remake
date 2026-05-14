#ifndef POO2_KEY_H
#define POO2_KEY_H
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GridCell.h"

class Key : public Button {
protected:
    char letter;
    CellState state;
public:
    Key(float x, float y, float width, float height, char l, const sf::Font& font, const CellState st=CellState::Key)
        :   Button{x, y, width, height, std::string(1,l), font},
            letter{l},
            state{st}{}

    char getLetter() const {return letter;}
    CellState getState() const {return state;}

    void updateState(const CellState newState) {
        //folosim functia statica din GridCell
        const sf::Color color = GridCell::getColorForState(newState);
        shape.setFillColor(color);
    }

};

#endif //POO2_KEY_H