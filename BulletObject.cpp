#include <iostream>
#include "BulletObject.h"
#include "BaseObject.h"
#include "CommonFunc.h"
BulletObject::BulletObject()
{
    x_val_ = 0;
    y_val_ = 0;
    is_move_ = false;
    bullet_type_ =SPHERE_BULLET;
}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
    bool ret = false;
    if(bullet_type_ == LAZER_BULLET)
    {
        ret = LoadImg("lazer_bullet.png", des);
    }
    else
    {
        ret = LoadImg("sphere_bullet.png", des);
    }
    return ret;
}
void BulletObject::HandleMove(const int& x_border,const int& y_border)
{
    if(bullet_dir_ == DIR_RIGHT)
    {
        rect_.x += x_val_;
        if (rect_.x > x_border)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_LEFT)
    {
        rect_.x -= x_val_;
        if (rect_.x  < 0)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_UP)
    {
        rect_.y -= y_val_;
        if(rect_.y < 0)
        {
            is_move_ = false;
        }
    }
    else if (bullet_dir_ == DIR_DOWN)
    {
        rect_.y += y_val_;
        if (rect_.y > y_border)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_UP_LEFT)
    {
        rect_.x -= x_val_;
        if(rect_.x < 0)
        {
            is_move_ = false;
        }
        rect_.y -= y_val_;
        if(rect_.y < 0)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_UP_RIGHT)
    {
        rect_.x += x_val_;
        if(rect_.x > x_border)
        {
            is_move_ = false;
        }

        rect_.y -= y_val_;
        if(rect_.y < 0)
        {
            is_move_ = false;
        }
    }
    else if( bullet_dir_ == DIR_DOWN_LEFT)
    {
        rect_.x -= x_val_;
        if(rect_.x <0 )
        {
            is_move_ = false;
        }
        rect_.y += y_val_;
        if(rect_.y > y_border)
        {
            is_move_ = false;
        }
    }
    else if(bullet_dir_ == DIR_DOWN_RIGHT)
    {
        rect_.x += x_val_;
        if(rect_.x > x_border)
        {
            is_move_ = false;
        }
        rect_.y += y_val_;
        if(rect_.y > y_border)
        {
            is_move_ = false;
        }
    }
}