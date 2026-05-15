#ifndef POO2_GE_H
#define POO2_GE_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "GridCell.h"
#include "Key.h"
#include "PopUp.h"
#include "WM.h"
#include "Label.h"

class GE : public UI, public ButtonObserver{
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

    void draw(sf::RenderWindow& window) override;

    std::vector<Key>& getKeyboard() {return keyboard;}

    void onButtonClick(const std::string& action) override {
        if (action == "PLAY") {
            std::cout << "Game Engine: Am primit semnalul de PLAY!" << std::endl;
            // Aici poți pune logica de start, de exemplu:
            // this->startGame();
        }
        else if (action == "CLOSE") {
            std::cout << "Game Engine: Închidem ferestrele..." << std::endl;
        }
    }

};

#endif //POO2_GE_H