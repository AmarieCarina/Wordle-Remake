#ifndef POO2_WORDLEEXCEPTIONS_H
#define POO2_WORDLEEXCEPTIONS_H

#include<exception>
#include<string>

class WordleException : public std::exception {
protected:
    std::string message;
public:
    explicit WordleException(std::string msg):
        message{std::move(msg)}{}

    //suprascriem what() din std::exception
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ResourceException : public WordleException {
public:
    explicit ResourceException(const std::string& resName)
    :WordleException{"[Eroare] Nu s a putut incarca resursa " + resName}{}
};


class DictionaryException : public WordleException{
public:
    explicit DictionaryException(const std::string& details)
        :WordleException{"[Eroare] Nu s a putut incarca dictionarul "+details}{}
};

class GameLogicException : public WordleException {
public:
    explicit GameLogicException(const std::string& details):WordleException{"[Eroare] Logica joc: "+details}{}
};

#endif //POO2_WORDLEEXCEPTIONS_H