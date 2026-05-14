#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include<iostream>

#include "GridCell.h"
#include "PopUp.h"
#include "UI.h"
#include "Button.h"
#include "Key.h"
#include "Label.h"
#include "GE.h"
#include "WordleExceptions.h"


int main() {
    try {
        ////////FEREASTRA PRINCIPALA
        sf::RenderWindow window(sf::VideoMode({800,800}), "WORDLE");

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

        ////////POP UP INTRO + BUTON PLAY
        auto Intro = std::make_unique<PopUp>(150.f, 200.f, 500.f, 200.f);
        Button playButton(370.0f,330.0f,70.0f,40.0f,"Play",font);
        Intro->addElement(std::make_unique<Label>(400.f,250.f,"WORDLE",fontTitle,40));
        Intro->addElement(std::make_unique<Label>(400.f,300.f,"Get 6 chances to guess a 5-letter word.", font,20));


        /////////POP UP INSTRUCTIUNI DE JOC + BUTON CLOSE
        auto Instr = std::make_unique<PopUp>(150.f, 150.f, 500.f, 500.f);
        Instr->addElement(std::make_unique<Label>(400.f,200.f, "HOW TO PLAY", fontTitle, 30));
        Instr->addElement(std::make_unique<Label>(300.f,270.f,"Guess the Wordle in 6 tries.",font,20));
        Instr->addElement(std::make_unique<Label>(390.f,310.f,"- each guess must be a valid 5-letter word.",font,20));
        Instr->addElement(std::make_unique<Label>(390.f,340.f,"- the color of the tiles will change to show",font,20));
        Instr->addElement(std::make_unique<Label>(390.f,360.f,"how close your guess was to the word.",font,20));
        Instr->addElement(std::make_unique<Label>(220.f,400.f,"Examples",font,20));
        int index=0;
        int dist=0;
        char exemple[]={'W','O','R','D','Y','L','I','G','H','T','R','O','G','U','E'};
        size_t nrLit=0;
        for (int i=0;i<3;i++) {
            for (int j=0;j<5;j++) {
                auto x = static_cast<float>(200+j*10+index);
                auto y = static_cast<float>(430+i*10+dist);
                auto cell = std::make_unique<GridCell>(x,y,30.f,font);
                if (exemple[nrLit]=='W') {
                    cell->updateState(CellState::Correct);
                }
                if (exemple[nrLit]=='I') {
                    cell->updateState(CellState::Present);
                }
                if (exemple[nrLit]=='U') {
                    cell->updateState(CellState::Default);
                }
                cell->setLetter(exemple[nrLit]);
                nrLit++;
                Instr->addElement(std::move(cell));
                index+=26;
                if (j==4) {index=0; dist+=60;}
            }
        }
        Instr->addElement(std::make_unique<Label>(380.f,475.f,"W is in the word and in the correct spot.",font,20));
        Instr->addElement(std::make_unique<Label>(365.f,545.f,"I is in the word but in the wrong spot.",font,20));
        Instr->addElement(std::make_unique<Label>(340.f,615.f,"U is not in the word in any spot.",font,20));

        Instr->setVisible(false);
        Button closeButton(600.0f, 160.0f, 30.0f, 30.0f, "x", font);
        closeButton.setOnClick([&Instr]() {
            if (Instr) {
                Instr->setVisible(false);
            }
        });

        playButton.setOnClick([&Intro, &Instr]() {
            if (Intro) {
                Intro->setVisible(false);
            }
            if (Instr) {
                Instr->setVisible(true);
            }
        });


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
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                        if (Instr && Instr->getVisible() && closeButton.isMouseOver(mousePos)) {
                            closeButton.click();
                        }

                        if (Intro && Intro->getVisible() && playButton.isMouseOver(mousePos)) {
                            playButton.click();
                        }

                        auto& keyboard = engine.getKeyboard();
                        for (auto& k:keyboard) {
                            if (k.isMouseOver(mousePos)) {
                                k.click();
                                engine.addLetter(k.getLetter());
                            }
                        }
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
            //// se repeta la infinit
            engine.draw(window);

            if (Instr && Instr->getVisible()) {
                Instr->draw(window);
                closeButton.draw(window);
            }

            if (Intro && Intro->getVisible()) {
                Intro->draw(window);
                playButton.draw(window);
            }

            ///////////
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