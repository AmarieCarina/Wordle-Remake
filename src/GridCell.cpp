#include "../include/GridCell.h"
#include <SFML/Graphics.hpp>
void GridCell::setLetter(char c) {
    //copiere caracter in continutul elementului
    letter.setString(c);

    //calibrare in centru a literei

    //dreptunghiul local al textului
    const sf::FloatRect textRect = letter.getLocalBounds();

    const float midX = textRect.size.x / 2.0f;
    const float midY = textRect.size.y / 2.0f;

    //originea elementului este in centrul sau (nu in coltul din stg sus)
    letter.setOrigin({
        textRect.position.x + midX,
        textRect.position.y + midY,
    });
    //setare pozitie in centru
    letter.setPosition({
                box.getPosition().x + box.getSize().x / 2.0f,
                box.getPosition().y + box.getSize().y / 2.0f
    });
}

char GridCell::getLetter() const {
    sf::String s = letter.getString();
    return s.isEmpty() ? ' ' : static_cast<char>(s[0]);
}

void GridCell::updateState(CellState newState) {
    state = newState;
    const sf::Color color= getColorForState(newState);
    box.setFillColor(color);
}

GridCell::GridCell(float x, float y, float size, const sf::Font& font)
    :   UI{x,y},
        box{{size,size}},
        letter{font, " ", static_cast<unsigned int>(size * 0.6f)},
        state{CellState::Default}
{
    //configuram dreptunghiul
    box.setPosition({x,y});
    box.setFillColor(sf::Color(50,50,50)); //gri inchis
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color(100,100,100));//contur celula

    //configuram litera
    letter.setFillColor(sf::Color::White);
    setLetter(' ');//pt logica, apelam metoda
}

void GridCell::draw(sf::RenderWindow& window) {
    window.draw(box);
    window.draw(letter);
}

sf::Color GridCell::getColorForState(CellState state) {
    switch (state) {
        case CellState::Absent:return sf::Color(50,50,50); //gri inchis
        case CellState::Present:return sf::Color(200,180,50); //galben
        case CellState::Correct:return sf::Color(50,150,50);
        // case CellState::Key:return sf::Color(127,127,127);
        default: //CellState::Default:
            return sf::Color(120,120,120);  //gri neutru
    }
}