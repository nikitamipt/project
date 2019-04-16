#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace sf;



class Entity {
public:
    float dx, dy; //скорость
    FloatRect rect; //координаты нашего парнишки (x,y,ширнаЮ высота)
    Sprite sprite; // наша картинка
    virtual bool update(float time) = 0; //возвращает true, если во время передвижения не прозошло столкновения со стеной.
    bool life;

};

#endif ENTITY_H

