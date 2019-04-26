

#ifndef GAME_WITH_RUNNERS
#define GAME_WITH_RUNNERS



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
#include "Bot_stupid_runner.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <string>

/*
 ���������� ������: ���� ������� ���� ������ �� ���������.
 � ���� ���� 5 ������ - ���������� �� ��������� ������. �� ����� �������� control ������ �� ����� ������ ������ ����� - ����, �� ������� �� ���������������������� �������� �����������.
 ���� ���� �������� � ������� ��������� ����, ����������� �� �������� ������������ ��������� - ��������� ������.
 1) �������� ����� ����� � ���������� �������������� � ��������
 2) ���� ��������� ��������� ���������� �, ������� ������, ��������.
 3) �������� ����, ������� �������� ������ ���� � ��� ���� ��������� ������ ���� ����� (������ score � ������ Bot)
 4) ����� ��� ������������ ��������.
 5) ������� ����� ������ �����, ������ ������ �� �������� ����� ������������ ����������� ���� � ��������� ���������� 30% �� ������ ����� �������������.
 6) �������� � ������ 2), ���� �� �������� ������.
*/



//#include <SFML/Audio.hpp>

using namespace sf;


void Game_with_Runners() {



    RenderWindow window(VideoMode(960, 960), "SFML works!");

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

     // ���� � �������� ���� �����. ������
    ofstream f_net_out;
    f_net_out.open("f_net_out_Runner.txt");
    ifstream f_net_in;
    f_net_in.open("f_net_in_Runner.txt");
    int mas_weights[inputs * mids + mids * outs + 10];


    //����� �����
    ifstream f_map("map1.txt");
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

    std::list<Runner*>  Bots_life;//����� � ����� ����� �����. ������ ���� � �������� �� ������� ���������
    std::list<Runner*> Bots_death;//����� � ����� �������. ���, ��� ����������� �� ������. ����� ��� ���� �������, � ������� �� ����� ������ ����� ����, ������� �������� ����� ���� � ������.
    std::list<Entity*>::iterator it;
    std::list<Runner*>::iterator Bbot;


//������ ���� ����� � ������
    for (int i = 0; i < num_of_bots; i++) {
        Bots_life.push_back(new Runner(t, x_start, y_start));
    }


    bool Right_key = false; //���������, ����� �� Right ��� ���. ���� ��� ����� ��������� �����. �� �������� ����� �����, ����� ��� ���� ����� �� ����� � �� ��������.
    Clock clock; // ����� �������� ���� ��������� �� �������, � �� � �������� ������� ����������


    bool next_generation = false; //����� true - ������ ���� ����� � ���������� �������!

    while (window.isOpen()){

        float time = clock.getElapsedTime().asMicroseconds(); // time ����� ��� ����, ����� ���� �������� � ����� � ��� �� ��������� �������������� �� ������������� ����������
        time = time / 500; //���� ��������� �����.
        if (time > 20) time = 20;
   // time = 20;
        clock.restart(); //������������� �����. ��� �� ������� �������� �����.

        //������� �������� ���� (���� ��������� �����)
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

//����� ��������� ���! NO_HUMAN!
        bool qqq = false;//��������, ���� �� ��� ����� ����
        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();){
            qqq = true;
            if (next_generation) {qqq = false;}  //�� ���� ���� ���� ������ �������� Right
            Runner *b = *Bbot;//��� ��������, ����� �� ������ (*it)->
            b->control();
            b->life = (b->update(time)); //��� �������, ��� ������ ���������� �� ������
        //���� ��� ���������� �� ������, �� "������� ���", ������� � ������ ������� �����, �������� ��������� �� �������.
            if ((b->life == false) || (next_generation))	{
                b->dist_to_finish = Dijkstra(int((b->rect.left)/ 16), int((b->rect.top)/16));
                Bots_death.push_back(b);
                b->life = false;
                Bbot = Bots_life.erase(Bbot);
            }
            else Bbot++;//� ���� �������� (����������) � ���� �������. ��� ������ �� ����� ��������� ������
		}



// ���� ��� ���� ��������� ������, ������ �������� �����
        if (!qqq) {
            next_generation = false;
            unsigned long long int best_rot = 0;
            Runner *bb;
            int best_dist = 10000; // ���� ��� ���������� ������� ���� (������ �������� � ������.

//������ �������� "���������" ����
            for (Bbot = Bots_death.begin(); Bbot != Bots_death.end(); Bbot++){
                Runner *b = *Bbot;
                if ((b->dist_to_finish < (best_dist + 1)) && (b->score > best_rot)) {
                    best_rot = b->score;
                    if (b->dist_to_finish < (best_dist)) {best_dist = b->dist_to_finish;}
                    bb = b;
                }
            }
            bb->Net.mSaveKohonet(mas_weights);
            bb->Net.fSaveKohonet(f_net_out);
            printf("bb->dist_to_finish = %d  bb->score = %d\n",  Dijkstra(int((bb->rect.left)/ 16), int((bb->rect.top)/16)), bb->score);


            int rrr = -1; //������ ��� ����� ��� ���������. ��� ����������� ����������
            for (Bbot = Bots_death.begin(); Bbot != Bots_death.end();){
                (*Bbot)->Net.mGetKohonet(mas_weights, rrr); rrr++;
                (*Bbot)->rect.left = float(x_start * 16);
                (*Bbot)->rect.top = float(y_start * 16);
                (*Bbot)->score = 0;
                Bots_life.push_back(*Bbot);
                Bbot = Bots_death.erase(Bbot);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {  window.close(); }

        //����� ��������� ����� ��� (����� ���� ����������� �� �����)
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            //����� ��������� ��� ������ �� �����������. �� ��������� �������
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

        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end(); Bbot++) {
            window.draw((*Bbot)->sprite);
        }
        window.display();
}

    }
    f_net_out.close();
}
#endif GAME_WITH_RUNNERS




