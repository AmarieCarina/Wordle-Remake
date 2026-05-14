#include "GE.h"
#include "GridCell.h"
#include "Key.h"
#include "WM.h"
#include "Label.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

GE::GE(sf::Font& font, sf::Font& fontTitle)
    :currentRow{0},
    currentCol{0},
    maxRows{6},
    maxCols{5},
    wordmanager{"assets/words.txt"},
    warningLabel{350.f,80.f,"",font,20},
    showWarning{false}
    {
        const WM engine("assets/words.txt");
        targetWord=engine.getRandomWord();
    ////////////////////targetWord = wordmanager.getRandomWord();/////////////////////////////
        initGrid(font);
        initKeyboard(font);

        final = std::make_unique<PopUp>(150.f, 200.f, 500.f, 200.f);
        final->addElement(std::make_unique<Label>(400.f, 250.f,"Thanks for playing!", fontTitle,20));
        int index=0;
        for (int i=0;i<5;i++) {
            auto x = static_cast<float>(300+i*10+index);
            auto cell=std::make_unique<GridCell>(x,300.f,30,font);
            cell->setLetter(targetWord[i]);
            final->addElement(std::move(cell));
            index+=26;
        }
         final->setVisible(false);

    }

void GE::initGrid(sf::Font& font) {
    const float cell_size = 50.0f;
    const float margin = 10.0f;
    const float window_size = 800.0f;

    const float totalWidth=(static_cast<float>(maxCols)*cell_size)+(static_cast<float>(maxCols-1)*margin);
    const float totalHeight=(static_cast<float>(maxRows)*cell_size)+(static_cast<float>(maxRows-1)*margin);

    const float startX = (window_size - totalWidth)/2.0f;
    const float startY = (window_size - totalHeight)/2.0f - static_cast<float>(100);

    for (int r = 0; r< maxRows; r++) {
        for (int c = 0; c < maxCols; c++) {
            float x = startX + static_cast<float>(c) * (cell_size + margin);
            float y = startY + static_cast<float>(r) * (cell_size + margin);
            grid.emplace_back(x,y,cell_size, font);
        }
    }
}

void GE::initKeyboard(sf::Font& font) {
    std::vector<std::string> layout {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
        };

    float xInitial = 150.0f; //stanga
    const float yInitial = 550.0f; //jos
    const float offx = 50.0f; //distanta dintre taste sau randuri
    const float offy = 70.0f;
    const float kWidth = 40.0f;
    const float kHeight = 55.0f;
    float rowIndent=1;

    for (size_t r=0; r < layout.size(); r++) {
        for (size_t c = 0; c < layout[r].size(); c++) {
            float x = xInitial + static_cast<float>(c) * offx;
            float y = yInitial + static_cast<float>(r) * offy;
            keyboard.emplace_back(x,y,kWidth, kHeight,layout[r][c],font);
        }
        xInitial += 20.0f * rowIndent;
        rowIndent *= 2.5;
    }
}

void GE::draw(sf::RenderWindow& window) {
    for (auto& cell : grid) {
        cell.draw(window);
    }
    for (auto& key : keyboard) {
        key.draw(window);
    }
    if (final && final->getVisible()) {
        final->draw(window);
    }
    if (showWarning) {
        if (warningClock.getElapsedTime().asSeconds()<2.0f) {
            warningLabel.draw(window);
        }
        else {
            showWarning = false;
        }
    }
}

void GE::addLetter(char c) {
    const char upperC = (c >= 'a' && c <= 'z') ? static_cast<char>(c-32) : c;

    if (upperC < 'A' || upperC > 'Z') {return;}

    if (currentCol < maxCols) {
        const int index = (currentRow * maxCols) + currentCol;
        grid[index].setLetter(upperC);
        currentCol++;
    }
}

void GE::deleteLastLetter() {
    if (currentCol > 0) {
        currentCol--;
        const int index = (currentRow * maxCols) + currentCol;
        grid[index].setLetter(' ');
    }
}

void GE::checkGuess() {
    if (currentCol < maxCols) {
        warningLabel.setText("Too short!");
        showWarning = true;
        warningClock.restart();
        return;
    }


    //extragem cuvantul din literele introduse
    std::string currentGuess;
    const int rowStartIndex = currentRow * maxCols;
    for (int i=0;i<maxCols;i++) {
        currentGuess += grid[rowStartIndex+i].getLetter();
    }

/////validare//////////////////////////////////
    if (!wordmanager.isValidWord(currentGuess)) {
        warningLabel.setText("Not in word list!");
        showWarning = true;
        warningClock.restart();
        return;
    }
    showWarning=false;

    //algortimul de colorare
    //vector de stari + copie target
    std::vector results(maxCols, CellState::Default);
    std::string targetCopy = targetWord;

    //PAS 1: LITERELE CORECTE
    for (int i=0; i<maxCols; i++) {
        if (currentGuess[i] == targetCopy[i]) {
            results[i] = CellState::Correct;
            for (Key& key:keyboard) {
                if (key.getLetter()==currentGuess[i]) {
                    key.updateState(CellState::Correct);
                }
            }
            targetCopy[i] = '*'; //marcam litera ca fiind folosita
        }
    }

    //PAS 2: LITERELE PRESENTE
    for (int i=0; i<maxCols; i++) {
        //sarim peste literele corecte
        if (results[i] == CellState::Correct) continue;

        for (int j=0;j<maxCols;j++) {
            if (currentGuess[i] == targetCopy[j]) {
                results[i] = CellState::Present;

                for (Key& key:keyboard) {
                    if (key.getLetter()==currentGuess[i]) {
                        if (key.getState() != CellState::Correct)
                            key.updateState(CellState::Present);
                    }
                }
                targetCopy[j]='*';
                break;
            }
        }
    }

    //update in GridCell
    for (int i=0;i<maxCols;i++) {
        char litera = currentGuess[i];
        grid[rowStartIndex + i].updateState(results[i]);

        //update in keyboard
        for (Key& k: keyboard) {
            if (k.getLetter()==litera) {
                if (k.getState()!=CellState::Correct) {
                    k.updateState(results[i]);
                }
            }
        }
    }

    //progres joc
    if (currentGuess == targetWord) {
        final->setVisible(true);
    }else
        if (currentRow<maxRows-1) {
            currentRow++;
            currentCol = 0;
        } else {
            final->setVisible(true);
        }
}


