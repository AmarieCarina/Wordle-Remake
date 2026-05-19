#include <SFML/Graphics.hpp>
#include<iostream>
#include "Button.h"
#include "GE.h"
#include "WordleExceptions.h"

int main() {
    try {
        ////////FEREASTRA PRINCIPALA
        sf::RenderWindow window(sf::VideoMode({800,800}), "WORDLE");

        ///////INCARCARE ICONITA
        if (sf::Image icon; icon.loadFromFile("assets/icon.png")) {
            const sf::Vector2u size = icon.getSize();

            window.setIcon(size, icon.getPixelsPtr());
        } else {
            std::cerr << "[AVERTISMENT] Nu s-a putut încărca iconița ferestrei!" << std::endl;
        }
        ///////FONT
        sf::Font font;
        if (!font.openFromFile("assets/Roboto.ttf")) {
            throw ResourceException("assets/Roboto.ttf");
        }
        sf::Font fontTitle;
        if (!fontTitle.openFromFile("assets/Diplomata-Regular.ttf")) {
            throw ResourceException("assets/Diplomata-Regular.ttf");
        }

        ///////GAME ENGINE
        GE engine(font, fontTitle);


        //////////////////////////////////////////////////////////////////////////////////
        ////////////////// B U C L A   W H I L E /////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////

        if (std::getenv("GITHUB_ACTIONS")) {
            window.close();
        }

        while (window.isOpen()) {

            while (const std::optional event=window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                //INPUT MOUSE
                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseBtn->button == sf::Mouse::Button::Left) {
                        //CLICK STANGA

                        //transformam pozitia mouse-ului in pixeli
                        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        engine.handleMouseClick(mousePos);
                    }
                }

                //INPUT TASTATURA FIZICA (litere)
                if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
                    engine.addLetter(static_cast<char>(textEvent->unicode));

                }

                //INPUT TASTATURA FIZICA (enter+backspace)
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        engine.checkGuess();
                    }
                    if (keyEvent->code == sf::Keyboard::Key::Backspace) {
                        engine.deleteLastLetter();
                    }
                }
            }
            window.clear(sf::Color::Black);
            engine.draw(window);
            window.display();
        }
    } catch (const WordleException& e){
        std::cerr <<"[APLICATIA S A OPRIT] "<<e.what()<<std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr <<"[EROARE STANDARD] "<<e.what()<<std::endl;
        return 2;
    }
    return 0;
}