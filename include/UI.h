#ifndef POO2_UI_H
#define POO2_UI_H
#include<SFML/Graphics.hpp>
class UI {
protected:
    sf::Vector2f position; //coordonatele (x,y) pe ecran
    bool isVisible = true; //vizibilitatea pe ecran
public:
    //constructor, destructor
    UI(float x, float y);
    virtual ~UI()=default;

    //forteaza derivatele sa decida CUM se deseneaza
    virtual void draw(sf::RenderWindow& window)=0;

    //getters, setters pentru vizibilitate
    void setVisible(bool visible);
    bool getVisible()const;

    //setare pozitie
    void setPosition(sf::Vector2f pos);
};
#endif //POO2_UI_H