
#ifndef PLAYER_H
#define PLAYER_H


#include "Entity.hpp"

#pragma once
using namespace sf;


float offsetX = 0, offsetY = 0;

const int W = 60;

const int ZZ = 16;
const int speed_bul = 3;
const int H = 60;

String TileMap[H];


class Player : public Entity {
public:
    float dsp;  //будет ли он двигаться или нет?
    float da;   //угол относительно Ox


    bool onGround; // на замеле ли мы находимся?
    float currentFrame; // екущее положение анимации


    void Player_maker(Texture &image, int x_start, int y_start) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        rect = FloatRect(x_start * 16, y_start * 16,16,16);//текущие координаты, где я заспамлюсь
        sprite.setPosition(rect.left, rect.top);
        sprite.setTextureRect(IntRect(5*16, 9*16, 16, 16));
        dx = 0; dy = 0; da = 0;
        currentFrame = 0;
       // sprite.setOrigin(16/2, 16 / 2);
    }

// возвращает true, есл произошло столкновение со стеной..
    bool update(float time) {
        int result = 0;
        rect.left += dx * time / 10;
        result += Collision(1);
        rect.top += dy * time / 10; //координата y
        result += Collision(0);
        sprite.setPosition(rect.left, rect.top ); // координаты x, y
      //  sprite.setRotation(da * 180 /3.1415);
        dx = 0; dy = 0;
        if (result) return false;
        return true;
    }

//возвращает 1, если произошло столкновение со стеной.
    int  Collision(int dir) {
        int result = 0;
        for (int i = rect.top/16; i < (rect.top + rect.height)/16; i++) {
            for (int j = rect.left/16; j < (rect.left + rect.width)/16; j++) {
				 if (TileMap[i][j]=='1') {
                    result = 1;
                    if (dir == 1) {
                        if (dx > 0) rect.left = j*16 - rect.width;
                        if (dx < 0) rect.left = j*16 + 16;
                    } else if (dir == 0) {
                        if (dy > 0) {rect.top = i*16 - rect.height; dy = 0;}
                        if (dy < 0) {rect.top = i*16 + 16; dy = 0;}
                    }
                }

            }
        }
        return result;
    }

};



#endif PLAYER_H
