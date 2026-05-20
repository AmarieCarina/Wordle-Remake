#include "GE.h"
#include "GridCell.h"
#include "Key.h"
#include "WM.h"
#include "Label.h"
#include "Hint.h"
#include <vector>
#include <SFML/Graphics.hpp>

template<class T>
void alignElements(std::vector<std::unique_ptr<T>>& elements, const float startX, const float startY, const float spacingX, const float spacingY, const int columns) {
    for (size_t i = 0; i < elements.size(); ++i) {
        float x = startX + static_cast<float>(i % columns) * spacingX;
        float y = startY + (static_cast<float>(i) / static_cast<float>(columns)) * spacingY;
        elements[i]->setPosition({x, y});
    }
}


GE::GE(sf::Font& font, sf::Font& fontTitle)
    :UI{0.f, 0.f},
    currentRow{0},
    currentCol{0},
    maxRows{6},
    maxCols{5},
    playButton{370.0f, 330.0f, 70.0f, 40.0f, "Play", font},
    closeButton{600.0f, 160.0f, 30.0f, 30.0f, "x", font},
    hintButton{650.f, 50.f, 100.f, 40.f, "Hint", font},
    wordmanager{"assets/words.txt"},
    hintsUsed{650.f, 100.f, "Hints: 0/5", font, 16},
    warningLabel{350.f,80.f,"",font,20},
    showWarning{false}
    {
        const WM engine("assets/words.txt");
        targetWord=engine.getRandomWord();
        initGrid(font);
        initKeyboard(font);
        initIntroPopUp(font, fontTitle);
        initInstrPopUp(font, fontTitle);
        initFinalPopUp(font, fontTitle);
        hintButton.setAction("GIVE_HINT");

    }

void GE::initGrid(sf::Font& font) {
    constexpr float cell_size = 50.0f;
    constexpr float margin = 10.0f;
    constexpr float window_size = 800.0f;

    const float totalWidth=(static_cast<float>(maxCols)*cell_size)+(static_cast<float>(maxCols-1)*margin);
    const float totalHeight=(static_cast<float>(maxRows)*cell_size)+(static_cast<float>(maxRows-1)*margin);

    const float startX = (window_size - totalWidth)/2.0f;
    const float startY = (window_size - totalHeight)/2.0f - static_cast<float>(100);

    for (int r = 0; r< maxRows; r++) {
        for (int c = 0; c < maxCols; c++) {
            float x = startX + static_cast<float>(c) * (cell_size + margin);
            float y = startY + static_cast<float>(r) * (cell_size + margin);
            grid.emplace_back(x,y,cell_size, font);
        }
    }
}

void GE::initKeyboard(sf::Font& font) {
    std::vector<std::string> layout {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
        };

    float xInitial = 150.0f; //stanga
    float rowIndent=1;

    for (size_t r=0; r < layout.size(); r++) {
        for (size_t c = 0; c < layout[r].size(); c++) {
            constexpr float kHeight = 55.0f;
            constexpr float kWidth = 40.0f;
            constexpr float offy = 70.0f;
            constexpr float offx = 50.0f;
            constexpr float yInitial = 550.0f;
            float x = xInitial + static_cast<float>(c) * offx;
            float y = yInitial + static_cast<float>(r) * offy;
            keyboard.emplace_back(x,y,kWidth, kHeight,layout[r][c],font);
        }
        xInitial += 20.0f * rowIndent;
        rowIndent *= 2.5;
    }
}

void GE::initIntroPopUp(sf::Font& font, sf::Font& fontTitle) {
    introPopUp = std::make_unique<PopUp>(150.f, 200.f, 500.f, 200.f);
    introPopUp->addElement(std::make_unique<Label>(400.f, 250.f, "WORDLE", fontTitle, 40));
    introPopUp->addElement(std::make_unique<Label>(400.f, 300.f, "Get 6 chances to guess a 5-letter word.", font, 20));

    // Configurăm butonul de play (care e membru GE acum)
    playButton.setAction("PLAY");
    playButton.addObserver(this);
}

void GE::initInstrPopUp(sf::Font& font, sf::Font& fontTitle) {
    instrPopUp = std::make_unique<PopUp>(150.f, 150.f, 500.f, 500.f);
    instrPopUp->addElement(std::make_unique<Label>(400.f, 200.f, "HOW TO PLAY", fontTitle, 30));
    instrPopUp->addElement(std::make_unique<Label>(300.f, 270.f, "Guess the Wordle in 6 tries.", font, 20));

    std::vector<std::unique_ptr<GridCell>> exampleCells;
    int nr_rand = 0;
    int offset_col = 0;
    int index = 0;
    for (int i = 0; i < 15; i++) {
        constexpr char exemple[] = {'W','O','R','D','Y','L','I','G','H','T','R','O','G','U','E'};
        const std::vector<std::string> tips {"W is in the word and in the correct spot.", "I is in the word but in the wrong spot.", "U is not in the word, in any spot."};
        auto cell = std::make_unique<GridCell>(210.f+static_cast<float>(40*nr_rand), 300.f+static_cast<float>(offset_col), 35.f, font);
        cell->setLetter(exemple[i]);
        nr_rand++;
        if (nr_rand == 5) {
            offset_col +=100;
            nr_rand = 0;
            instrPopUp->addElement(std::make_unique<Label>(380.f,260.f+static_cast<float>(offset_col), tips[index], font, 20));
            index++;
        }

        if (exemple[i] == 'W') cell->setColor(hintManager.getCellColor(CellState::Correct));
        if (exemple[i] == 'I') cell->setColor(hintManager.getCellColor(CellState::Present));

        exampleCells.push_back(std::move(cell));
    }
    alignElements(exampleCells, 200.f, 430.f, 36.f, 60.f, 5);

    for (auto& cell : exampleCells) {
        instrPopUp->addElement(std::move(cell));
    }

    closeButton.setAction("CLOSE");
    closeButton.addObserver(this); // GE observa si acest buton

    instrPopUp->setVisible(false);
}

void GE::initFinalPopUp(sf::Font& font, sf::Font& fontTitle) {
    finalPopUp = std::make_unique<PopUp>(150.f, 200.f, 500.f, 200.f);
    finalPopUp->addElement(std::make_unique<Label>(400.f, 250.f, "Thanks for playing!", fontTitle, 20));

    int index=0;
    for (int i=0;i<5;i++) {
        auto x = static_cast<float>(300+i*10+index);
        auto cell=std::make_unique<GridCell>(x,300.f,30,font);
        cell->setLetter(targetWord[i]);
        finalPopUp->addElement(std::move(cell));
        index+=26;
    }
    finalPopUp->setVisible(false);
}

void GE::draw(sf::RenderWindow& window) {
    for (auto& cell : grid) cell.draw(window);
    for (auto& key : keyboard) key.draw(window);
    hintButton.draw(window);
    hintsUsed.draw(window);

    if (introPopUp && introPopUp->getVisible()) {
        introPopUp->draw(window);
        playButton.draw(window);
    }

    if (instrPopUp && instrPopUp->getVisible()) {
        instrPopUp->draw(window);
        closeButton.draw(window);
    }

    if (finalPopUp && finalPopUp->getVisible()) {
        finalPopUp->draw(window);
    }

    if (showWarning) {
        if (warningClock.getElapsedTime().asSeconds() < 2.0f) {
            warningLabel.draw(window);
        } else {
            showWarning = false;
        }
    }
}

void GE::addLetter(const char c) {
    const char upperC = (c >= 'a' && c <= 'z') ? static_cast<char>(c-32) : c;
    if (upperC < 'A' || upperC > 'Z') return;

    const int rowStartIndex = currentRow * maxCols;

    // Sărim peste celulele hint-uite din fața cursorului
    while (currentCol < maxCols && grid[rowStartIndex + currentCol].getState() == CellState::Hint) {
        currentCol++;
    }

    if (currentCol < maxCols) {
        const int index = rowStartIndex + currentCol;
        grid[index].setLetter(upperC);
        currentCol++;
    }

    // Sărim din nou dacă următoarea poziție după inserare este tot un hint locked
    while (currentCol < maxCols && grid[rowStartIndex + currentCol].getState() == CellState::Hint) {
        currentCol++;
    }
}

void GE::deleteLastLetter() {
    while (currentCol > 0) {
        currentCol--;
        if (const int index = (currentRow * maxCols) + currentCol; grid[index].getState() != CellState::Hint) {
            grid[index].setLetter(' ');
            break;
        }
    }
}

void GE::checkGuess() {
    if (currentCol < maxCols) {
        warningLabel.setText("Too short!");
        showWarning = true;
        warningClock.restart();
        return;
    }

    //extragem cuvantul din literele introduse
    std::string currentGuess;
    const int rowStartIndex = currentRow * maxCols;
    for (int i=0; i<maxCols; i++) {
        currentGuess += grid[rowStartIndex+i].getLetter();
    }

/////validare//////////////////////////////////
    if (!wordmanager.isValidWord(currentGuess)) {
        warningLabel.setText("Not in word list!");
        showWarning = true;
        warningClock.restart();
        return;
    }
    showWarning=false;

    //algortimul de colorare
    //vector de stari + copie target
    std::vector results(maxCols, CellState::Default);
    std::string targetCopy = targetWord;

    //PAS 1: LITERELE CORECTE
    for (int i=0; i<maxCols; i++) {
        if (currentGuess[i] == targetCopy[i]) {
            results[i] = CellState::Correct;
            for (Key& key:keyboard) {
                if (key.getLetter()==currentGuess[i]) {
                    key.updateState(CellState::Correct);
                }
            }
            targetCopy[i] = '*'; //marcam litera ca fiind folosita
        }
    }

    //PAS 2: LITERELE PRESENTE
    for (int i=0; i<maxCols; i++) {
        //sarim peste literele corecte
        if (results[i] == CellState::Correct) continue;

        for (int j=0;j<maxCols;j++) {
            if (currentGuess[i] == targetCopy[j]) {
                results[i] = CellState::Present;

                for (Key& key:keyboard) {
                    if (key.getLetter()==currentGuess[i]) {
                        if (key.getState() != CellState::Correct)
                            key.updateState(CellState::Present);
                    }
                }
                targetCopy[j]='*';
                break;
            }
        }
    }

    //update in GridCell
    for (int i=0;i<maxCols;i++) {
        const char litera = currentGuess[i];
        grid[rowStartIndex + i].updateState(results[i]);

        //update in keyboard
        for (Key& k: keyboard) {
            if (k.getLetter()==litera) {
                if (k.getState()!=CellState::Correct) {
                    k.updateState(results[i]);
                }
            }
        }
    }

    //progres joc
    if (currentGuess == targetWord || currentRow >= maxRows - 1) {
        finalPopUp->setVisible(true);
    }
    else {
        currentRow++;
        currentCol = 0;
    }

}

void GE::handleMouseClick(const sf::Vector2f mousePos) {
    if (introPopUp && introPopUp->getVisible()) {
        if (playButton.isMouseOver(mousePos)) {
            playButton.click(); // Trimite semnalul "PLAY" către Observer (adică spre GE)
        }
        return; // Blochează click-urile pe orice se află în spatele acestui PopUp
    }

    if (instrPopUp && instrPopUp->getVisible()) {
        if (closeButton.isMouseOver(mousePos)) {
            closeButton.click(); // Trimite semnalul "CLOSE" către Observer
        }
        return; // Blochează gameplay-ul din fundal
    }

    if (finalPopUp && finalPopUp->getVisible()) {
        return;
    }

    if (hintButton.isMouseOver(mousePos)) {
        if (hintManager.isMaxHintsReached()) {
            return;
        }
        hintButton.click();

        const std::string textNou = "Hints: " + std::to_string(hintManager.getHintsUsed()+1) + "/5";
        hintsUsed.setText(textNou);
        if (hintManager.revealLetter(targetWord, grid, currentRow, maxCols)) {
            finalPopUp->setVisible(true);
            checkGuess();
        } else {
            const int rowStartIndex = currentRow * maxCols;
            currentCol = 0;

            while (currentCol < maxCols && grid[rowStartIndex + currentCol].getLetter() != ' ') {
                currentCol++;
            }
        }
        return;
    }

    // GAMEPLAY ACTIV: Dacă niciun PopUp nu e vizibil, verificăm tastatura virtuală a jocului
    for (auto& key : keyboard) {
        if (key.isMouseOver(mousePos)) {
            key.click();
            addLetter(key.getLetter());
            break;
        }
    }
}

