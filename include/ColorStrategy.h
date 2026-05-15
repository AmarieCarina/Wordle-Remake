#ifndef WORDLE_COLORSTRATEGY_H
#define WORDLE_COLORSTRATEGY_H
#include "SFML/Graphics/Color.hpp"
#include "GridCell.h"
///CERINTA: STRATEGY PATTERN
class ColorStrategy {
public:
    virtual ~ColorStrategy() = default;
    virtual sf::Color getColor(CellState state) = 0;
};

class ClassicWordleStrategy : public ColorStrategy {
public:
    sf::Color getColor(const CellState state) override {

        switch (state) {
            case CellState::Correct: return {106, 170, 100}; // Verde
            case CellState::Present: return {201, 180, 88}; // Galben
            default: return {58, 58, 60}; // Gri
        }
    }
};

#endif //WORDLE_COLORSTRATEGY_H