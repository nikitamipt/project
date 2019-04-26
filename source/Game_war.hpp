
#ifndef GAME_WAR_H
#define GAME_WAR_H



#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <map>


#include <iostream>

#include "Player.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Lines.hpp"
#include "Dijkstra.hpp"
#include "Person.hpp"
#include "Bot.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <string>



//#include <SFML/Audio.hpp>

using namespace sf;

// ���� ����������� �� �����������, ��� �� ������ ���� ������� bot_squre �� �� ������� (�� ������, �� � �� ������ � �������)
int bot_squre  = 5 * 5;
int kol_bot = 1;//(H * W) / bot_squre;

void Game_war() {



    RenderWindow window(VideoMode(960, 960), "SFML works!");

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

     // ���� � �������� ���� �����. ������
    ofstream f_net_out;
    f_net_out.open("f_net_out.txt");
    ifstream f_net_in;
    f_net_in.open("f_net_in.txt");
    int mas_weights[inputs * mids + mids * outs + 10];


    //����� �����
    ifstream f_map("map_war.txt");
    for (int i = 0; i < H; i++) {
        std::string str;
        std::getline(f_map, str);
        TileMap[i] = str;
    }

    //���������, ��� ���������� ��� �����
    int x_start = 0; int y_start = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (TileMap[i][j] == '3') {
                x_start = j; y_start = i;
                break;
            }
        }
    }


    Line L(t);  //����� ��� ��������� ������� � ��������� ���������� �� ������

    //������ ���� �������� ����� (������). ��������, �� �����, ���� ��
    std::list<Entity*>  entities;
    std::list<Bullet*>  bullets; //������ ����, ������� ���� �� �����
    std::list<Bot*>  Bots_life;//����� � ����� ����� �����. ������ ���� � �������� �� ������� ���������
    std::list<Bot*> Bots_death;//����� � ����� �������. ���, ��� ����������� �� ������. ����� ��� ���� �������, � ������� �� ����� ������ ����� ����, ������� �������� ����� ���� � ������.
    std::list<Entity*>::iterator it;
    std::list<Entity*>::iterator it1;
    std::list<Bot*>::iterator Bbot;
    std::list<Bullet*>::iterator bul;
    std::list<Bullet*>::iterator bul1;

    Person p(1);
    p.Player_maker(t, x_start, y_start);

    for (int i = 0; i < kol_bot; i++) {
//������ ������ ���������� ��� ����, ����� ����� �������� �� � ������� �����
        Bots_life.push_back(new Bot(t, rand()%(W - 2) + 1, rand()%(H - 2) + 1));
    }


    Enemy Enemy(t); //�����, � ������� � �������. ���� ������



    //Driver Driver(0); // �����, ������� ��������� �������!  (��-��-��, ����!)

    bool space = false; //���������, ����� �� SPACE ��� ���. ���� ��� ��������, ����� ������� �� ����
    bool Right_key = false; //���������, ����� �� Right ��� ���. ���� ��� ����� ��������� �����.
    Clock clock; // ����� �������� ���� ��������� �� �������, � �� � �������� ������� ����������

    bool next_generation = false; //����� true - ������ ���� ����� � ���������� ������!

    while (window.isOpen()){


        float time = clock.getElapsedTime().asMicroseconds();
        time = time / 500; //���� ��������� �����.
        if (time > 20) time = 20;
    time = 2;
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

        if (who) {
    //����� ���������� ����������� ����������. p.control == true, ���� ��������� �������.
printf("AAA\n");

            if (p.control(pos)) {
printf("AAA1\n");

                float dx2 = pos.x-p.rect.left; float dy2 = pos.y - p.rect.top;
                float m = sqrt(dx2*dx2 + dy2 * dy2);
                bullets.push_back(new Bullet(p.rect.left, p.rect.top, t, speed_bul*dx2/m, speed_bul*dy2/m, &p));
            }
            p.update(time);
printf("AAA2\n");

        }



//����� � ��������� ���! NO_HUMAN!
        bool qqq = false;//��������, ���� �� ��� ����� ����
        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();){

            qqq = true;
            if (next_generation) {qqq = false;}
            Bot *b = *Bbot;//��� ��������, ����� �� ������ (*it)->

            //����� ���� ��������� �������� ����. ��-��, � ���� ���� �����! �������
            if (b->control()) {
                bullets.push_back(new Bullet(b->rect.left, b->rect.top, t, speed_bul*cos(b->da), speed_bul*sin(b->da), b));
            }
            b->life = (b->update(time)); //��� �������, ��� ������ ���������� �� ������
        //���� ��� ���������� �� ������, �� "������� ���", ������� � ������ ������� �����, �������� ��������� �� �������.
            if ((b->life == false) || (next_generation))	{
                b->dist_to_finish = Dijkstra(int((b->rect.left)/ 16), int((b->rect.top)/16));
                Bots_death.push_back(b);
                b->life = false;
             //   if (b->dist_to_finish < best_dist) {best_dist = b->dist_to_finish;}
                Bbot = Bots_life.erase(Bbot);
            }
            else Bbot++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}


//������ �������� �����, ���� ��� ���� ��������� ������.
		if (!qqq) {
            next_generation = false;
            unsigned long long int best_rot = 0;
            Bot *bb;
        //    printf("A1\n");
            int best_dist = 10000; // ���� ��� ���������� ������� ���� (������ �������� � ������.

//������ �������� "���������" ����
            for (Bbot = Bots_death.begin(); Bbot != Bots_death.end(); Bbot++){
                Bot *b = *Bbot;
                if ((b->dist_to_finish < (best_dist + 1)) && (b->score > best_rot)) {
                    best_rot = b->score;
                    if (b->dist_to_finish < (best_dist)) {best_dist = b->dist_to_finish;}
                    bb = b;
                }
            }
            bb->Net.mSaveKohonet(mas_weights);
            bb->Net.fSaveKohonet(f_net_out);
            printf("bb->dist_to_finish = %d  bb->score = %d\n",  Dijkstra(int((bb->rect.left)/ 16), int((bb->rect.top)/16)), bb->score);


            int rrr = -5; //������ ��� ����� ��� ���������. ��� ����������� ����������
            for (Bbot = Bots_death.begin(); Bbot != Bots_death.end();){
                (*Bbot)->Net.mGetKohonet(mas_weights, rrr); rrr++;
                (*Bbot)->rect.left = float((rand() % (W-2) + 1) * 16);
                (*Bbot)->rect.top  = float((rand() % (H-2) + 1) * 16);
                (*Bbot)->score = 0;
                Bots_life.push_back(*Bbot);
                Bbot = Bots_death.erase(Bbot);
            }

         /*   for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();Bbot++){
                (*Bbot)->Net.rSaveKohonet();
                printf("\n");
            }*/


        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {  window.close(); }

        //����� ��������� ����� ��� (����� ���� ����������� �� �����)
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (Right_key == false) {
                Right_key = true;
                for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();Bbot++){
                    (*Bbot)->life = false;
                }
                next_generation = true;
            }
        } else {
            Right_key = false;
        }




        Enemy.update(0);


//�������� ������� ��������� �������� ����� (����� ��� ����, ����� �������)
        for (bul = bullets.begin(); bul != bullets.end();){
			Bullet *b = *bul;//��� ��������, ����� �� ������ (*it)->
			b->update(time);//�������� �-��� update ��� ���� �������� (�� ���� ��� ���, ��� ���)
			if (b->life == false)	{ bul = bullets.erase(bul); delete b; }// ���� ���� ������ �����, �� ������� ���
			else bul++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}


//����� ��������� ��������� ����. ���� � ���� - ��� ���� ��������. ���� � ���� - ��� ��������, � ������� ���� ����������� ���� ����
        for (bul = bullets.begin(); bul != bullets.end(); bul++){
            for (bul1 = bullets.begin(); bul1 != bullets.end(); bul1++){
                if (bul1 != bul && (*bul)->rect.intersects((*bul1)->rect) && (*bul)->life && (*bul1)->life) {
                    (*bul)->life = false; (*bul1)->life = false;
                }
            }
		}

printf("AAA33333\n");
    for (bul1 = bullets.begin(); bul1 != bullets.end(); bul1++){
            for (Bbot = Bots_life.begin(); Bbot != Bots_life.end(); Bbot++){
                if ((*bul)->rect.intersects((*Bbot)->rect) && (*bul)->life && (*Bbot)->life && ((*bul)->owner != (*Bbot))) {
                    // (*Bbot)->life = false;
                    (*bul)->life = false;
                }
            }
		}


if (visual)  {
        window.clear(Color::White);

        for (int i=0; i<H; i++) {
			 for (int j=0; j<W; j++)
			  {
				if (TileMap[i][j]=='1')  tile.setTextureRect( IntRect(143-ZZ*3,112,ZZ,ZZ) );
                if (TileMap[i][j]=='2')  tile.setTextureRect( IntRect(143-ZZ*2,112 + 16,ZZ,ZZ) );
                if (TileMap[i][j]=='3')  tile.setTextureRect( IntRect(143-ZZ*3,112 + 16,ZZ,ZZ) );
                if (TileMap[i][j]=='4')  tile.setTextureRect( IntRect(143-ZZ*2,112,ZZ,ZZ) );
				if (TileMap[i][j]=='0') continue;

  			    tile.setPosition(j*ZZ,i*ZZ) ;
		        window.draw(tile);
		     }
        }

        //����� ������
        if(who) {L.rect.left = p.rect.left; L.rect.top = p.rect.top + 4;
        while (TileMap[int(L.rect.top/16)][int(L.rect.left/16)] == '0') {
            L.rect.left += 8 * cos(p.da); L.rect.top += 8 * sin(p.da); L.update(0);
            window.draw(L.sprite);
        }}

        window.draw(Enemy.sprite);
        if (who) {window.draw(p.sprite);}


        for (it = entities.begin(); it != entities.end(); it++){ window.draw((*it)->sprite);}
        for (bul = bullets.begin(); bul != bullets.end(); bul++){ window.draw((*bul)->sprite);}


        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end(); Bbot++) {
            window.draw((*Bbot)->sprite);
        }
        window.display();
}

    }


    /*Bot.Net.GetKohonet(f_net_in);
    Bot.Net.SaveKohonet(f_net_out);*/
    f_net_out.close();




}
#endif GAME_WAR_H





