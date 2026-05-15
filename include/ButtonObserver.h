#ifndef WORDLE_BUTTONOBSERVER_H
#define WORDLE_BUTTONOBSERVER_H
#include <string>

///CERINTA: OBSERVER PATTERN
class ButtonObserver {
public:
    virtual ~ButtonObserver() = default;
    virtual void onButtonClick(const std::string& buttonAction) = 0;
};

#endif //WORDLE_BUTTONOBSERVER_H