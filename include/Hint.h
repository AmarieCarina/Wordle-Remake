#ifndef WORDLE_HINT_H
#define WORDLE_HINT_H
#include <iostream>
#include<string>
#include <vector>
#include "ColorStrategy.h"

class Hint {
    int hintsUsed = 0;
    std::unique_ptr<ColorStrategy> colorScheme;
public:
    Hint() : colorScheme(std::make_unique<ClassicWordleStrategy>()) {}

    bool revealLetter(const std::string& target, std::vector<GridCell>& cells, const int currentRow, const int maxCols) {
        const int rowStartIndex = currentRow * maxCols;

        // Căutăm prima căsuță GOALĂ de pe rândul curent pentru a pune indiciul
        for (int i = 0; i < maxCols; ++i) {
            if (const int cellIndex = rowStartIndex + i; cells[cellIndex].getLetter() == ' ' && cells[cellIndex].getState() != CellState::Hint) {
                cells[cellIndex].setLetter(target[i]);
                cells[cellIndex].updateState(CellState::Hint); // O facem roz și o blocăm

                hintsUsed++;
                std::cout << "Hint folosit! Total indicii utilizate: " << hintsUsed << "/5" << std::endl;
                break;
            }
        }
        bool rowIsComplete = true;
        for (int j = 0; j < maxCols; ++j) {
            if (cells[rowStartIndex + j].getLetter() == ' ') {
                rowIsComplete = false; // Încă mai sunt spații goale
                break;
            }
        }

        return rowIsComplete;
    }

    int getHintsUsed() const {
        return hintsUsed;
    }

    // Verifică dacă s-a atins limita absolută de 5 indicii
    bool isMaxHintsReached() const {
        return hintsUsed >= 5;
    }

    ///CERINTA: PATTERN STRATEGY
    sf::Color getCellColor(const CellState state) const{
        return colorScheme->getColor(state);
    }
};
#endif //WORDLE_HINT_H