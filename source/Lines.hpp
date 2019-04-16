#ifndef LINES_H
#define LINES_H



#include "Entity.hpp"
using namespace sf;



class Line {
public:
    FloatRect rect; //координаты нашего парнишки (x,y,ширнаЮ высота)
    Sprite sprite; // наша картинка

    Line(Texture &image) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        //sprite.setTextureRect(IntRect(0, 244, 40, 50)); // теперь я ограничиваю эту картинку заданными координатами
        rect = FloatRect(0,0,4,4);//текущие координаты, где я заспамлюсь
    //    sprite.setPosition(40, 40); /
        sprite.setTextureRect(IntRect(112, 144, 4, 4));
    }

    void update(float time) {
        sprite.setPosition(rect.left, rect.top ); // координаты x, y
    }


};

#endif  LINES_H

