#ifndef POO2_GE_H
#define POO2_GE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "GridCell.h"
#include "Key.h"
#include "PopUp.h"
#include "WM.h"
#include "Label.h"

class GE {
    //DATE LOGICE
    int currentRow;
    int currentCol;
    const int maxRows;
    const int maxCols;
    std::string targetWord; //cuvantul de ghicit

    //COMPONENTE UI
    std::vector<GridCell> grid;
    std::vector<Key> keyboard;
    std::unique_ptr<PopUp> final;
    WM wordmanager;

    //CAZURI LIMITA
    Label warningLabel;
    sf::Clock warningClock;
    bool showWarning;

    //HELPERI PRIVATI (init)
    void initGrid(sf::Font& font);
    void initKeyboard(sf::Font& font);

public:
    GE(sf::Font& font, sf::Font& fontTitle);

    void addLetter(char c);

    void deleteLastLetter();

    void checkGuess();

    void draw(sf::RenderWindow& window);

    std::vector<Key>& getKeyboard() {return keyboard;}

};

#endif //POO2_GE_H