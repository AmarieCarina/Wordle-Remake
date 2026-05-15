#ifndef POO2_POPUP_H
#define POO2_POPUP_H
#include "UI.h"
#include<vector>
#include "Button.h"
class BaseHolder {
public:
    virtual ~BaseHolder() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
};

///CERINTA
template <typename T>
class ElementHolder : public BaseHolder {
public:
    T data; ///CERINTA

    explicit ElementHolder(T val) : data(std::move(val)) {}

    ///CERINTA
    void updateData(T newVal) { data = std::move(newVal); }

    void draw(sf::RenderWindow& window) override {
        // Dacă T este un unique_ptr la ceva ce moștenește din UI
        if (data) {
            data->draw(window);
        }
    }
};

class PopUp : public UI {
    sf::RectangleShape background;

    //vector cu tot felul de componente:
        //Labels, GridCells, Button
    std::vector<std::unique_ptr<BaseHolder>> elements;
        //unique_ptr: cand PopUp se sterge, se sterg si elementele, fara memory leaks

public:
    //constructor
    PopUp(const float x, const float y, float width, float height)
        : UI(x, y) {

            background.setSize({width, height});
            background.setPosition(position);
            background.setFillColor(sf::Color(30, 30, 30, 240));
            background.setOutlineThickness(2);
            background.setOutlineColor(sf::Color::White);
    }

    template <class T>
    void addElement(std::unique_ptr<T> element) {
        elements.push_back(std::make_unique<ElementHolder<std::unique_ptr<T>>>(std::move(element)));
    }


    void draw(sf::RenderWindow& window) override {
        if (!isVisible) return;
        //desenam fundalul
        window.draw(background);
        //desenam elementele din fundal
        for (const auto& el : elements) {
            el->draw(window);
        }
    }

    void clear() {
        elements.clear();
    }
};
#endif //POO2_POPUP_H