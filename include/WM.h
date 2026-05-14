#ifndef POO2_WM_H
#define POO2_WM_H
#include<vector>
#include<string>
#include<unordered_set>
class WM {
    std::vector<std::string> words; //pt alegere cuvant random
    std::unordered_set<std::string> dictionary; //toate cuvintele disponibile

public:
    explicit WM(const std::string& filename);

    bool isValidWord(const std::string& word) const;

    std::string getRandomWord() const;
};

#endif //POO2_WM_H