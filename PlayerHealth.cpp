#include <iostream>
#include "PlayerHealth.h"

PlayerHealth::PlayerHealth()
{
    number_ = 0;
}

PlayerHealth::~PlayerHealth()
{

}

void PlayerHealth::AddPos(const int& xp)
{
    pos_list_.push_back(xp);
}

void PlayerHealth::Init(SDL_Renderer* screen)
{
    LoadImg("player_health.png", screen);
    number_ = 3;
    if(pos_list_.size() > 0)
    {
        pos_list_.clear();
    }

    AddPos(20);
    AddPos(60);
    AddPos(100);
}

void PlayerHealth::Show(SDL_Renderer* screen)
{
    for(int i=0; i<pos_list_.size(); i++)
    {
        rect_.x = pos_list_.at(i);
        rect_.y = 0;
        Render(screen);
    }
}

void PlayerHealth::Descrease()
{
    number_--;
    pos_list_.pop_back();
    number_ = 0;
}

void PlayerHealth::InitCrease()
{
    number_++;
    int last_pos = pos_list_.back();
    last_pos += 40;
    pos_list_.push_back(last_pos);
}
