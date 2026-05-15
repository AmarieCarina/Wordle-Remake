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
#include "Hint.h"

template<class T>
void alignElements(std::vector<std::unique_ptr<T>>& elements, float startX, float startY, float spacingX, float spacingY, int columns) {
    for (size_t i = 0; i < elements.size(); ++i) {
        float x = startX + (i % columns) * spacingX;
        float y = startY + (i / columns) * spacingY;
        elements[i]->setPosition({x, y});
    }
}

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
        Hint hintManager;

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


        std::vector<std::unique_ptr<GridCell>> exampleCells;

        for (int i = 0; i < 15; i++) {
            char exemple[] = {'W','O','R','D','Y','L','I','G','H','T','R','O','G','U','E'};
            auto cell = std::make_unique<GridCell>(0.f, 0.f, 30.f, font);
            cell->setLetter(exemple[i]);

            // Folosim Strategy din hintManager pentru culori
            if (exemple[i] == 'W') cell->setColor(hintManager.getCellColor(CellState::Correct));
            if (exemple[i] == 'I') cell->setColor(hintManager.getCellColor(CellState::Present));

            exampleCells.push_back(std::move(cell));
        }
        alignElements(exampleCells, 200.f, 430.f, 36.f, 60.f, 5);

        // Mutăm celulele în PopUp
        for (auto& cell : exampleCells) {
            Instr->addElement(std::move(cell));
        }
        Instr->setVisible(false);


        Button closeButton(600.0f, 160.0f, 30.0f, 30.0f, "x", font);

        playButton.setAction("PLAY");
        closeButton.setAction("CLOSE");

        // Înregistrăm observatorii (trebuie ca engine-ul sau o clasă nouă să fie Observer)
        // Pentru simplitate acum, dacă ai păstrat setOnClick, lasă-l, dar cerința cere Observer.
        // Dacă ai creat ButtonObserver, codul arată așa:
        playButton.addObserver(&engine);
        closeButton.addObserver(&engine);

        Button hintButton(650.f, 50.f, 100.f, 40.f, "Hint", font);
        hintButton.setAction("GIVE_HINT");
        hintButton.addObserver(&hintManager);

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