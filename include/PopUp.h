#ifndef POO2_POPUP_H
#define POO2_POPUP_H
#include "UI.h"
#include<vector>
class PopUp : public UI {
private:
    sf::RectangleShape background;

    //vector cu tot felul de componente:
        //Labels, GridCells, Button
    std::vector<std::unique_ptr<UI>> elements;
        //unique_ptr: cand PopUp se sterge, se sterg si elementele, fara memory leaks

public:
    //constructor
    PopUp(float x, float y, float width, float height);

    void addElement(std::unique_ptr<UI> element);

    void draw(sf::RenderWindow& window) override;

    void clear();
};
#endif //POO2_POPUP_H