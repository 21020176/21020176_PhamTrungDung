#ifndef PLAYERHEALTH_H_INCLUDED
#define PLAYERHEALTH_H_INCLUDED

#include "CommonFunc.h"
#include "BaseObject.h"

class PlayerHealth : public BaseObject
{
public:
    PlayerHealth();
    ~PlayerHealth();

    void SetNum(const int& num) {number_ = num;}
    void AddPos(const int& xPos);
    void Show(SDL_Renderer* screen);
    void Init(SDL_Renderer* screen);

    void InitCrease();
    void Descrease();
private:
    int number_;
    std::vector<int> pos_list_;
};

#endif // PLAYERHEALTH_H_INCLUDED
