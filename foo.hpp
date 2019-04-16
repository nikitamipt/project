
#ifndef FOO_H
#define FOO_H

#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <string>


#include <iostream>

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;



class A {
public:
    FloatRect rect; //координаты нашего парнишки (x,y,ширнаЮ высота)
    Sprite sprite; // наша картинка
    bool life;
    int qqq;
};



class B : public A {
public:
    void Player_maker(Texture &image, int x_start, int y_start) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        rect = FloatRect(x_start * 16, y_start * 16,16,16);//текущие координаты, где я заспамлюсь
        sprite.setPosition(rect.left, rect.top);
        sprite.setTextureRect(IntRect(5*16, 9*16, 16, 16));
    }
};

class C : public B {
public:
    C(int q, Texture &image, int x_start, int y_start) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        rect = FloatRect(x_start * 16, y_start * 16,16,16);//текущие координаты, где я заспамлюсь
        sprite.setPosition(rect.left, rect.top);
        sprite.setTextureRect(IntRect(5*16, 9*16, 16, 16));
        qqq = q;
        printf("C(%d)\n", q);
    }
    ~C(){
        printf("~C(%d)\n", qqq);
    }
};

//как нормально создавать объект C. Так, чтобы можно было его отом запихивать в массив?

void foo() {
    RenderWindow window(VideoMode(480, 480), "SFML works!");
    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

    std::list<C*>  Cs;
    std::list<C*>::iterator C1;

    for (int i = 0; i < 5; i++) {
/*        C C111(i);
        C111.Player_maker(t, 20, 20);*/
        Cs.push_back(new C(i, t, 20, 20));
    }
    for (C1 = Cs.begin(); C1 != Cs.end(); C1++) {
       // printf("x = %d\n", (*C1)->qqq);
        window.draw((*C1)->sprite);
    }

}


#endif FOO_H

