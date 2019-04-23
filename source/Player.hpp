
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
    unsigned int last_shot; //показывает время, когда в последний раз стрельнул персонаж.

    bool onGround; // на замеле ли мы находимся?
    float currentFrame; // екущее положение анимации
    int num_of_kills; //число убийств этим персонажем. "и судимы были мертвые по написанному в книгах, сообразно с делами своими"

    void Player_maker(Texture &image, int x_start, int y_start) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        rect = FloatRect(int(x_start * 16), int(y_start * 16),int(16),int(16));//текущие координаты, где я заспамлюсь
        sprite.setPosition(rect.left, rect.top);
        sprite.setTextureRect(IntRect(5*16, 9*16, 16, 16));
        dx = 0; dy = 0; da = 0;
        currentFrame = 0;
        last_shot = 0; //так как изначально наш парнишка не стрелял!
       // sprite.setOrigin(16/2, 16 / 2);
    }

// возвращает true, есл произошло столкновение со стеной..
    bool update(float time) {
printf("AAA4\n");
        int result = 0;
        rect.left += dx * time / 10;
        result += Collision(1);
printf("AAA5\n");
        rect.top += dy * time / 10; //координата y
        result += Collision(0);
printf("AAA6\n");
        if (rect.left < 16 || rect.top < 16 || rect.left >= 16 * W || rect.top >= 16 * H) {result += 1;}
 //проверка на выход из карты.
        else { sprite.setPosition(rect.left, rect.top );
} // координаты x, y
      //  sprite.setRotation(da * 180 /3.1415);
        dx = 0; dy = 0;

        if (result) return false;
        return true;
    }

//возвращает 1, если произошло столкновение со стеной.
    int  Collision(int dir) {
        int result = 0;
        for (int i = int(rect.top/16); i < int((rect.top + rect.height)/16); i++) {
            for (int j = int(rect.left/16); j < int((rect.left + rect.width)/16); j++) {
/*printf("i = %d; j = %d; rect.top/16 = %d; (rect.top + rect.height)/16 = %d; rect.left/16 = %d; (rect.left + rect.width)/16 = %d\n",
        i,      j,      int(rect.top/16),      int((rect.top + rect.height)/16),      int(rect.left/16),      int((rect.left + rect.width)/16));*/
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
