

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
 Постановка задачи: надо обучить бота бегать по лабиринту.
 У бота есть 5 входов - расстояния до ближайших стенок. Во время операции control каждый из ботов должен выдать число - угол, на который он повернетсяотносительно текущего направления.
 Этот угол находтся с помощью нейронной сети, обучающейся по принципу генетичекого алгоритма - побеждает лучший.
 1) Запускаю много ботов с рандомными коэффициентами в нейронке
 2) Боты пробегают некоторое расстояние и, касаясь стенок, помирают.
 3) Выбираем бота, который пробежал дальше всех и при этом заработал больше всех очков (смотри score в классе Bot)
 4) Берем его коэффициенты нейронки.
 5) Создаем новую партию ботов, только теперь их нейронки имеют коэффициента предыдущего бота с рандомным изменением 30% от общего числа коэффициентов.
 6) Перехожу к пункту 2), пока не достигну финиша.
*/



//#include <SFML/Audio.hpp>

using namespace sf;


void Game_with_Runners() {



    RenderWindow window(VideoMode(960, 960), "SFML works!");

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

     // куда я сохраняю нашу сетку. откуда
    ofstream f_net_out;
    f_net_out.open("f_net_out_Runner.txt");
    ifstream f_net_in;
    f_net_in.open("f_net_in_Runner.txt");
    int mas_weights[inputs * mids + mids * outs + 10];


    //читаю карту
    ifstream f_map("map1.txt");
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

    std::list<Runner*>  Bots_life;//здесь я храню живых ботов. Каждый такт я обновляю их текущее положение
    std::list<Runner*> Bots_death;//здесь я храню мертвых. Тех, что столкнулись со стеной. Когда все боты умирают, я выбираю из этого списка ботов бота, который оказался ближе всех к финишу.
    std::list<Entity*>::iterator it;
    std::list<Runner*>::iterator Bbot;


//создаю всех ботов в начале
    for (int i = 0; i < num_of_bots; i++) {
        Bots_life.push_back(new Runner(t, x_start, y_start));
    }


    bool Right_key = false; //контролит, нажат ли Right или нет. Надо для новой генерации ботов. Ее нажимать можно тогда, когда все боты стоят на месте и не помирают.
    Clock clock; // чтобы скорость была привязана ко времени, а не к тактовой частоте процессора


    bool next_generation = false; //когда true - убиваю всех ботов и перерождаю лучшЕГО!

    while (window.isOpen()){

        float time = clock.getElapsedTime().asMicroseconds(); // time нужен для того, чтобы игра запускал с одной и той же скоростью внезависимости от загруженности процессора
        time = time / 500; //дать прошедшее время.
        if (time > 20) time = 20;
   // time = 20;
        clock.restart(); //перезагружает время. Так мы измерим скорость такта.

        //событие закрытия окна (след несколько строк)
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

//здесь управляет бот! NO_HUMAN!
        bool qqq = false;//проверяю, есть ли еще живые боты
        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();){
            qqq = true;
            if (next_generation) {qqq = false;}  //То есть если была нажата кнопочка Right
            Runner *b = *Bbot;//для удобства, чтобы не писать (*it)->
            b->control();
            b->life = (b->update(time)); //бот умирает, как только столкнулся со стеной
        //если бот столкнулся со стеной, то "убиваем его", заносим в список мертвых ботов, считывая дистанцию до фининша.
            if ((b->life == false) || (next_generation))	{
                b->dist_to_finish = Dijkstra(int((b->rect.left)/ 16), int((b->rect.top)/16));
                Bots_death.push_back(b);
                b->life = false;
                Bbot = Bots_life.erase(Bbot);
            }
            else Bbot++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}



// Если все боты оказались мертвы, заново запускаю ботов
        if (!qqq) {
            next_generation = false;
            unsigned long long int best_rot = 0;
            Runner *bb;
            int best_dist = 10000; // надо для определния лучшего бота (самого ближнего к финишу.

//нахожу наиболее "успешного" бота
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


            int rrr = -1; //первый ген будет без изменений. все последующие заменяться
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

        //чтобы запустить новый род (убить всех вращающихся на месте)
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            //чтобы несколько раз подряд не перерождать. От залипания клавиши
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




