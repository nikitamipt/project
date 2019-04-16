
#ifndef GAME_H
#define GAME_H



#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>

#include <iostream>

#include "Player.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"




//#include <SFML/Audio.hpp>

using namespace sf;





void RunGame() {

    RenderWindow window(VideoMode(480, 480), "SFML works!");

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);


    float currentFrame = 0; //����� �������� �����, ����� ������� ��������
//    list<Entity*>  entities;
/*    Sprite s;
    s.setTexture(t);
    s.setTextureRect(IntRect(0,244,40,50)); //�������� �� �������� Rect(x,y, width, height)
    s.setPosition(50,100);    //���� ������ ���� ��������
*/

    std::list<Entity*>  entities;
    std::list<Entity*>::iterator it;

    Player p(t);
    Enemy Enemy(t);

    bool space = false; //���������, ����� �� SPACE ��� ���. ���� ��� ��������, ����� ������� �� ����

    Clock clock; // ����� �������� ���� ��������� �� �������, � �� � �������� ������� ����������




    while (window.isOpen()){


        float time = clock.getElapsedTime().asMicroseconds();
        time = time / 500; //���� ��������� �����.
        if (time > 20) time = 20;
        clock.restart(); //������������� �����. ��� �� ������� �������� �����.

        Vector2i pixelPos = Mouse::getPosition(window);//�������� ����� �������
		Vector2f pos = window.mapPixelToCoords(pixelPos);//��������� �� � ������� (������ �� ����� ����)
		//std::cout << pixelPos.x << "\n";//������� �� ���������� � ������� ������� � ������� (��� �� ����� ������ ������ ����)
		//std::cout << pos.x << "\n";//������� �� �,������� ��������������� � ������� ����������


        //������� �������� ���� (���� ��������� �����)
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }




        if (Keyboard::isKeyPressed(Keyboard::Right) or (Keyboard::isKeyPressed(Keyboard::D))) { p.dx = 1;}
        if (Keyboard::isKeyPressed(Keyboard::Left)  or (Keyboard::isKeyPressed(Keyboard::A))) { p.dx = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Up)  or (Keyboard::isKeyPressed(Keyboard::W))) { p.dy = -1; }
        if (Keyboard::isKeyPressed(Keyboard::Down)  or (Keyboard::isKeyPressed(Keyboard::S))) { p.dy = 1; }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (space == false) {
                space = true;
                float dx2 = pos.x-p.rect.left; float dy2 = pos.y - p.rect.top;
                float m = sqrt(dx2*dx2 + dy2 * dy2);
                entities.push_back(new Bullet(p.rect.left, p.rect.top, t, speed_bul*dx2/m, speed_bul*dy2/m));

                printf("p.rect.left = %f, p.rect.top = %f, pos.x = %f, pos.y = %f, m = %f\n\n",
                        p.rect.left,       p.rect.top,     pos.x,      pos.y,      m);

            }
        } else { space = false;}

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {  window.close(); }


        p.update(time);
        Enemy.update(0);

        for (it = entities.begin(); it != entities.end();){
			Entity *b = *it;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false)	{ it = entities.erase(it); delete b; }// ���� ���� ������ �����, �� ������� ���
			else it++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}



        if (p.rect.intersects(Enemy.rect) && Enemy.life) {
            Enemy.life = false;
        }
        for (it = entities.begin(); it != entities.end(); it++){
			if ((*it)->rect.intersects(Enemy.rect) && Enemy.life) {
                Enemy.life = false;
            }
		}



        window.clear(Color::White);

        for (int i=0; i<H; i++) {
			 for (int j=0; j<W; j++)
			  {
				if (TileMap[i][j]=='1')  tile.setTextureRect( IntRect(143-ZZ*3,112,ZZ,ZZ) );
				if (TileMap[i][j]==' ') continue;

  			    tile.setPosition(j*ZZ,i*ZZ) ;
		        window.draw(tile);
		     }
        }
        window.draw(Enemy.sprite);
        window.draw(p.sprite);




        for (it = entities.begin(); it != entities.end(); it++){
			window.draw((*it)->sprite);
		}

        window.display();


    }


}

// ���������� ������ � �������������
/*
#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>

using namespace sf;

float offsetX = 0, offsetY = 0;

const int H = 17;
const int W = 150;




String TileMap[H] = {
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"0                                                                                                                                                    0",
"0                                                                                    w                                                               0",
"0                   w                                  w                   w                                                                         0",
"0                                      w                                       kk                                                                    0",
"0                    cthu                                                         k  k    k    k                                                         0",
"0                      c                                                      k      kkk  kkk  w                                                     0",
"0                                                                       r     k       k    k                                                         0",
"0                                                                      rr     k  k                                                                   0",
"0                                                                     rrr      kk                                                                    0",
"0               c    kckck                                           rrrr                                                                            0",
"0   PPPPP                              t0                           rrrrr                                                                            0",
"0G                                     00              t0          rrrrrr            G                                                               0",
"0           d    g       d             00              00         rrrrrrr                                                                            0",
"PPPPPPPPPPPPPPPPPPPPPP         PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
};



class Enemy {

public:
float dx, dy;
FloatRect rect;
Sprite sprite;
float currentFrame;
bool life;

void set(Texture &image, int x, int y) {
    sprite.setTexture(image);
    rect = FloatRect(x, y, 16, 16);
    dx = 0.05;
    currentFrame = 0;
    life = true;
}

void update(float time) {
    rect.left += dx * time;
    Collision();

    currentFrame += time * 0.005;
    if (currentFrame > 2) currentFrame -= 2;

    sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16) );
    if (!life)sprite.setTextureRect(IntRect(58, 0, 16, 16));

    sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}

void Collision() {
	for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++) {
		for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++) {
			 if ((TileMap[i][j]=='P') || (TileMap[i][j]=='0')) {
                if (dx>0) { rect.left =  j*16 - rect.width; dx*=-1; }
                else if (dx<0){ rect.left =  j*16 + 16;  dx*=-1; }
            }
        }
    }
  }
};

class Player {
public:

    float dx, dy; //��������
    FloatRect rect; //���������� ������ �������� (x,y,������ ������)
    bool onGround; // �� ������ �� �� ���������?
    Sprite sprite; // ���� ��������
    float currentFrame; // ������ ��������� ��������

    Player(Texture &image) {
        sprite.setTexture(image);  //������� � sprite �������� �������
        //sprite.setTextureRect(IntRect(0, 244, 40, 50)); // ������ � ����������� ��� �������� ��������� ������������
        rect = FloatRect(100,180,16,16);//������� ����������, ��� � ����������
    //    sprite.setPosition(40, 40); /
        dx = dy = 0.1;
        currentFrame = 0;
    }

    void update(float time) {
        rect.left += dx * time;
        Collision(1);
        if (!onGround) {dy = dy + 0.0005 * time;}
        rect.top += dy * time; //���������� y
        onGround = false;
        Collision(0);
/*        if (rect.top > ground) {
            rect.top = ground;
            dy = 0;
            onGround = true;
        }*/
/*
        currentFrame += 0.005 * time;
        if (currentFrame > 3) currentFrame -= 3;

        if (dx > 0) sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame), 144, 16, 16));
        if (dx < 0) sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame) + 16, 144, -16, 16));

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY); // ���������� x, y

        dx = 0;
    }

    void Collision(int dir) {
        for (int i = rect.top/16; i < (rect.top + rect.height)/16; i++) {
            for (int j = rect.left/16; j < (rect.left + rect.width)/16; j++) {
				 if ((TileMap[i][j]=='P') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t')) {
                    if (dir == 1) {
                        if (dx > 0) rect.left = j*16 - rect.width;
                        if (dx < 0) rect.left = j*16 + 16;
                    } else if (dir == 0) {
                        if (dy > 0) {rect.top = i*16 - rect.height; dy = 0; onGround = true; }
                        if (dy < 0) {rect.top = i*16 + 16; dy = 0;}
                    }
                }

            }
        }
    }

};


int main() {

    RenderWindow window(VideoMode(400, 250), "SFML works!");


    CircleShape MyCircle(100.f); //������� ���������� ���������� ������� 100
   // MyCircle.setFillColor(Color::Green); //���������� ����� �������


    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);


    float currentFrame = 0; //����� �������� �����, ����� ������� ��������
*/
/*    Sprite s;
    s.setTexture(t);
    s.setTextureRect(IntRect(0,244,40,50)); //�������� �� �������� Rect(x,y, width, height)
    s.setPosition(50,100);    //���� ������ ���� ��������
*//*

    Player p(t);
    Enemy Enemy;
    Enemy.set(t, 48 * 16, 13 * 16);


    Clock clock; // ����� �������� ���� ��������� �� �������, � �� � �������� ������� ����������




    while (window.isOpen()){


        float time = clock.getElapsedTime().asMicroseconds();
        time = time / 500; //���� ��������� �����.
        if (time > 20) time = 20;
        clock.restart(); //������������� �����. ��� �� ������� �������� �����.

        //������� �������� ���� (���� ��������� �����)
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            //s.move(U * time, 0); //�������� �� time, ����� �������� ���� ��������� �� �������. �� ����� ����� = �������� * �����
            p.dx = 0.1;
*//*
// ������� �������� ����
            currentFrame += 0.5 * U * time;
            if (currentFrame > 6) currentFrame -= 6;
            s.setTextureRect(IntRect(40 * int(currentFrame), 244, 40, 50)); // �� ���� ������� �������� ���������� ������ ��� �� 40. ��������� ���� �����.
*//*
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) { p.dx = -0.1; }

        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            if (p.onGround) {
                p.dy = - 0.3;
                p.onGround = false;
            }
        }
        p.update(time);
        Enemy.update(time);

        if (p.rect.intersects(Enemy.rect) && Enemy.life) {
            if (p.dy > 0) {Enemy.dx = 0; p.dy = -0.2; Enemy.life = false;}
            else {p.sprite.setColor(Color::Red);}
        }


        if (p.rect.left > 600/2) {offsetX = p.rect.left - 600/2;}//����� ��� ����������� �����
       //  if (p.rect.top > 400/2)  {offsetY = p.rect.top - 400/2;}



        RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(16, 16));

          window.clear(Color(107,140,255));

		 for (int i=0; i<H; i++) {
			 for (int j=0; j<W; j++)
			  {
				if (TileMap[i][j]=='P')  tile.setTextureRect( IntRect(143-16*3,112,16,16) );

				if (TileMap[i][j]=='k')  tile.setTextureRect( IntRect(143,112,16,16) );

   			    if (TileMap[i][j]=='c')  tile.setTextureRect( IntRect(143-16,112,16,16) );

				if (TileMap[i][j]=='t')  tile.setTextureRect( IntRect(0,47,32,95-47) );

				if (TileMap[i][j]=='g')  tile.setTextureRect( IntRect(0,16*9-5,3*16,16*2+5) );

				if (TileMap[i][j]=='G')  tile.setTextureRect( IntRect(145,222,222-145,255-222) );

			    if (TileMap[i][j]=='d')  tile.setTextureRect( IntRect(0,106,74,127-106) );

				if (TileMap[i][j]=='w')  tile.setTextureRect( IntRect(99,224,140-99,255-224) );

				if (TileMap[i][j]=='r')  tile.setTextureRect( IntRect(143-32,112,16,16) );

				if ((TileMap[i][j]==' ') || (TileMap[i][j]=='0')) continue;

  			    tile.setPosition(j*16-offsetX,i*16 - offsetY) ;
		        window.draw(tile);
		     }
        }
        window.draw(Enemy.sprite);
        window.draw(p.sprite);
        window.display();

    }
}

String TileMap[H] = {
"111111111111111111111111111111",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1                            1",
"1           111              1",
"1          1   1             1",
"1          1   1             1",
"1          1   1             1",
"1          1   1             1",
"1          1   1             1",
"1          1   1             1",
"1          1   1             1",
"1         111 111            1",
"1        1   1   1           1",
"1        1   11  1           1",
"1         111  11            1",
"1                            1",
"1                            1",
"111111111111111111111111111111",
};


*/


#endif GAME_H


