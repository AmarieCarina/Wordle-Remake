#include "WM.h"
#include<vector>
#include<fstream>
#include<iostream>
#include<random>
#include <algorithm>
#include <cctype>
#include<WordleExceptions.h>
WM::WM(const std::string& filename) {
    std::ifstream f(filename);
    std::string word;

    if (!f.is_open()) {
        throw DictionaryException("[Eroare] Nu am putut deschide "+filename);
    }
    while (f>>word) {
        std::ranges::transform(word.begin(), word.end(), word.begin(),
            [](const unsigned char c){return std::toupper(c); });

        if (word.length()==5) {
            dictionary.insert(word);
            words.push_back(word);
        }
    }
    f.close();
    if (words.empty()) {
        throw DictionaryException("[Eroare] Dictionarul e gol sau nu contine cuvinte de 5 litere!");
    }
}

// cppcheck-suppress unusedFunction
bool WM::isValidWord(const std::string& word) const {
    if (dictionary.find(word)!=dictionary.end()) {
        return true;}
    return false;
}

// cppcheck-suppress unusedFunction
std::string WM::getRandomWord() const {
    if (words.empty()) {
        return "PIZZA";
    }

    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<std::size_t> dist{0,words.size()-1};

    return words[dist(rng)];
}