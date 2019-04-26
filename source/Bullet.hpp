#ifndef BULLET_H
#define BULLET_H



#include "Entity.hpp"
using namespace sf;



class Bullet : public Entity {
public:
    Player* owner; //������ ����. �� ���� ���, ��� �� ��������
    Bullet(float x, float y, Texture &image, float _dx, float _dy, Player *_owner) {
        owner = _owner;
        life = true;
        sprite.setTexture(image);  //������� � sprite �������� �������
        //sprite.setTextureRect(IntRect(0, 244, 40, 50)); // ������ � ����������� ��� �������� ��������� ������������
        rect = FloatRect(x,y,12,12);//������� ����������, ��� � ����������
    //    sprite.setPosition(40, 40); /
        sprite.setTextureRect(IntRect(112, 144, 12, 12));
        dx = _dx; dy = _dy;
    }

    bool update(float time) {
        rect.left += dx * time / 10;
        Collision(1);
        rect.top += dy * time / 10; //���������� y
        Collision(0);
        sprite.setPosition(rect.left, rect.top ); // ���������� x, y
    }

    void Collision(int dir) {
        for (int i = rect.top/16; i < (rect.top + rect.height)/16; i++) {
            for (int j = rect.left/16; j < (rect.left + rect.width)/16; j++) {
				 if (TileMap[i][j]=='1') {life = false;       }

            }
        }
    }
};

#endif  BULLET_H
