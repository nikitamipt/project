

#ifndef PERSON_H
#define PERSON_H


#include "Player.hpp"


using namespace sf;







class Person : public Player {
public:

    bool control(Vector2f pos) {
        bool space1 = false;
        if (Keyboard::isKeyPressed(Keyboard::Right) or (Keyboard::isKeyPressed(Keyboard::D))) { da += 0.003; }
        if (Keyboard::isKeyPressed(Keyboard::Left)  or (Keyboard::isKeyPressed(Keyboard::A))) { da -= 0.003; }
        if (Keyboard::isKeyPressed(Keyboard::Up)    or (Keyboard::isKeyPressed(Keyboard::W))) { dsp = 1; }
        if (Keyboard::isKeyPressed(Keyboard::Down)  or (Keyboard::isKeyPressed(Keyboard::S))) { dsp = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {space1 = true;}
        dx = dsp*cos(da); dy = dsp*sin(da); dsp = 0;
        if (space1) {return true;}
        return false;
    }


};



#endif PERSON_H

