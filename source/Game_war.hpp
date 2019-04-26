
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

// буду расчитывать из соображений, что на одного бота выходит bot_squre кв ед площади (не харомы, но и не общага в Долгопе)
int bot_squre  = 5 * 5;
int kol_bot = 1;//(H * W) / bot_squre;

void Game_war() {



    RenderWindow window(VideoMode(960, 960), "SFML works!");

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

     // куда я сохраняю нашу сетку. откуда
    ofstream f_net_out;
    f_net_out.open("f_net_out.txt");
    ifstream f_net_in;
    f_net_in.open("f_net_in.txt");
    int mas_weights[inputs * mids + mids * outs + 10];


    //читаю карту
    ifstream f_map("map_war.txt");
    for (int i = 0; i < H; i++) {
        std::string str;
        std::getline(f_map, str);
        TileMap[i] = str;
    }

    //определяю, где возрадится наш герой
    int x_start = 0; int y_start = 0;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (TileMap[i][j] == '3') {
                x_start = j; y_start = i;
                break;
            }
        }
    }


    Line L(t);  //нужно для рисования прицела и нахождний расстояний до стенок

    //массив всех объектов карты (прицел). Возможно, не нужен, пока хз
    std::list<Entity*>  entities;
    std::list<Bullet*>  bullets; //массив пуль, которые есть на карте
    std::list<Bot*>  Bots_life;//здесь я храню живых ботов. Каждый такт я обновляю их текущее положение
    std::list<Bot*> Bots_death;//здесь я храню мертвых. Тех, что столкнулись со стеной. Когда все боты умирают, я выбираю из этого списка ботов бота, который оказался ближе всех к финишу.
    std::list<Entity*>::iterator it;
    std::list<Entity*>::iterator it1;
    std::list<Bot*>::iterator Bbot;
    std::list<Bullet*>::iterator bul;
    std::list<Bullet*>::iterator bul1;

    Person p(1);
    p.Player_maker(t, x_start, y_start);

    for (int i = 0; i < kol_bot; i++) {
//всякие минусы поставлены для того, чтобы игрок рождался не в стенках карты
        Bots_life.push_back(new Bot(t, rand()%(W - 2) + 1, rand()%(H - 2) + 1));
    }


    Enemy Enemy(t); //хрень, в которую я стреляю. типа мишень



    //Driver Driver(0); // штука, которая управляет игроком!  (да-да-да, сама!)

    bool space = false; //контролит, нажат ли SPACE или нет. Надо для стрельбы, чтобы очереди не было
    bool Right_key = false; //контролит, нажат ли Right или нет. Надо для новой генерации ботов.
    Clock clock; // чтобы скорость была привязана ко времени, а не к тактовой частоте процессора

    bool next_generation = false; //когда true - убиваю всех ботов и перерождаю лучших!

    while (window.isOpen()){


        float time = clock.getElapsedTime().asMicroseconds();
        time = time / 500; //дать прошедшее время.
        if (time > 20) time = 20;
    time = 2;
        clock.restart(); //перезагружает время. Так мы измерим скорость такта.

        Vector2i pixelPos = Mouse::getPosition(window);//забираем коорд курсора
		Vector2f pos = window.mapPixelToCoords(pixelPos);//переводим их в игровые (уходим от коорд окна)
		//std::cout << pixelPos.x << "\n";//смотрим на координату Х позиции курсора в консоли (она не будет больше ширины окна)
		//std::cout << pos.x << "\n";//смотрим на Х,которая преобразовалась в мировые координаты


        //событие закрытия окна (след несколько строк)
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (who) {
    //здесь происходит считываиние клавиатуры. p.control == true, если произошел выстрел.
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



//здесь я управляет бот! NO_HUMAN!
        bool qqq = false;//проверяю, есть ли еще живые боты
        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();){

            qqq = true;
            if (next_generation) {qqq = false;}
            Bot *b = *Bbot;//для удобства, чтобы не писать (*it)->

            //здесь идет обработка стрельбы бота. ДА-ДА, у него есть пушка! гангста
            if (b->control()) {
                bullets.push_back(new Bullet(b->rect.left, b->rect.top, t, speed_bul*cos(b->da), speed_bul*sin(b->da), b));
            }
            b->life = (b->update(time)); //бот умирает, как только столкнулся со стеной
        //если бот столкнулся со стеной, то "убиваем его", заносим в список мертвых ботов, считывая дистанцию до фининша.
            if ((b->life == false) || (next_generation))	{
                b->dist_to_finish = Dijkstra(int((b->rect.left)/ 16), int((b->rect.top)/16));
                Bots_death.push_back(b);
                b->life = false;
             //   if (b->dist_to_finish < best_dist) {best_dist = b->dist_to_finish;}
                Bbot = Bots_life.erase(Bbot);
            }
            else Bbot++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}


//заново запускаю ботов, если все боты оказались мертвы.
		if (!qqq) {
            next_generation = false;
            unsigned long long int best_rot = 0;
            Bot *bb;
        //    printf("A1\n");
            int best_dist = 10000; // надо для определния лучшего бота (самого ближнего к финишу.

//нахожу наиболее "успешного" бота
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


            int rrr = -5; //первый ген будет без изменений. все последующие заменяться
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

        //чтобы запустить новый род (убить всех вращающихся на месте)
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


//обновляю текущее положение объектов карты (таких как пули, линии прицела)
        for (bul = bullets.begin(); bul != bullets.end();){
			Bullet *b = *bul;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false)	{ bul = bullets.erase(bul); delete b; }// если этот объект мертв, то удаляем его
			else bul++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}


//здесь происзодт обработка пуль. Пуля в пулю - дву пули погибают. Пуля в бота - оба помирают, к хозяину пули добавляется одно очко
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

        //рисую прицел
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





