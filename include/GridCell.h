#ifndef POO2_GRIDCELL_H
#define POO2_GRIDCELL_H
#include "UI.h"
enum class CellState {
    Key,
    Default,
    Absent,
    Present,
    Correct
};

class GridCell : public UI {
private:
    sf::RectangleShape box; //container-ul in sine al celulei
    sf::Text letter;
    CellState state;
public:
    //constructori
    GridCell(float x, float y, float size, const sf::Font& font);
    ~GridCell() override = default;

    void setLetter(char c);
    char getLetter() const;

    void updateState(CellState newState); // modif culoare

    //redefinim functia virtuala de desenare
    void draw(sf::RenderWindow& window) override;

    //functie pura, nu opereaza pe datele membre
    static sf::Color getColorForState(CellState state);

};
#endif //POO2_GRIDCELL_H