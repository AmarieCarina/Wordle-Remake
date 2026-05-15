#ifndef WORDLE_HINT_H
#define WORDLE_HINT_H
#include <iostream>
#include<string>
#include <vector>
#include "ColorStrategy.h"

class Hint : public ButtonObserver{
    static int totalScore;
    int hintsUsed = 0;
    std::unique_ptr<ColorStrategy> colorScheme;
public:
    Hint() : colorScheme(std::make_unique<ClassicWordleStrategy>()) {}

    void revealLetter(const std::string& target, std::vector<GridCell>& cells) {
        for (size_t i = 0; i < target.length() && i < cells.size(); ++i) {
            if (cells[i].getLetter() != target[i]) {
                cells[i].setLetter(target[i]);
                cells[i].updateState(CellState::Correct); // O marcăm ca verde/corectă

                // Aplicăm culoarea folosind STRATEGY pattern
                cells[i].setColor(colorScheme->getColor(CellState::Correct));

                giveHint(); // Scădem scorul
                break; // Dezvăluim doar o literă la un click
            }
        }
    }

    template <typename T>
    T calculateFinalScore(T baseScore) {
        // baseScore poate fi int sau float
        return static_cast<T>(baseScore - (hintsUsed * 50));
    }

    void onButtonClick(const std::string& action) override {
        if (action == "GIVE_HINT") {
            giveHint();
        }
    }

    void giveHint() {
        hintsUsed++;
        totalScore -= 50; // Scade scorul pentru fiecare indiciu
        std::cout << "Hint folosit! Scos actual: " << totalScore << std::endl;
        // Aici va veni logica de reveal din GameEngine
    }

    static int getFinalScore() { return totalScore; }

    // Folosirea Strategy
    sf::Color getCellColor(const CellState state) const{
        return colorScheme->getColor(state);
    }
};
inline int Hint::totalScore = 1000;
#endif //WORDLE_HINT_H