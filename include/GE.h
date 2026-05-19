#ifndef POO2_GE_H
#define POO2_GE_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "GridCell.h"
#include "Hint.h"
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

    std::unique_ptr<PopUp> introPopUp;
    std::unique_ptr<PopUp> instrPopUp;
    std::unique_ptr<PopUp> finalPopUp;

    Button playButton;
    Button closeButton;
    Button hintButton;

    void initIntroPopUp(sf::Font& font, sf::Font& fontTitle);
    void initInstrPopUp(sf::Font& font, sf::Font& fontTitle);
    void initFinalPopUp(sf::Font& font, sf::Font& fontTitle);

    WM wordmanager;
    Hint hintManager;
    Label hintsUsed;

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

    void handleMouseClick(sf::Vector2f mousePos);

    void onButtonClick(const std::string& action) override {
        if (action == "PLAY") {
            std::cout << "Game Engine: Start joc -> Deschidem instrucțiunile." << std::endl;
            introPopUp->setVisible(false);
            instrPopUp->setVisible(true);
        }
        else if (action == "CLOSE") {
            std::cout << "Game Engine: Închidem instrucțiunile -> Începe gameplay-ul." << std::endl;
            instrPopUp->setVisible(false);

            // TODO: Aici vom activa și butonul de Hint să devină vizibil!
        }
    }

};

#endif //POO2_GE_H