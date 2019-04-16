#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.hpp"

using namespace sf;



class Enemy : public Entity{
public:

    Enemy(Texture &image) {
        sprite.setTexture(image);
        life = false;
        update(0);
    }


    bool update(float time1) {
        if (!life) {
            srand(time(0));
            rect = FloatRect((1 + (rand()%(H-2)))*16, (1 + (rand()%(W-2)))*16, ZZ, ZZ);
            sprite.setTextureRect(IntRect(18, 0, 16, 16) );
            sprite.setPosition(rect.left, rect.top);
            life = true;
        }
    }

};

#endif ENEMY_H
