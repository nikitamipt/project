
#ifndef GAME_H
#define GAME_H



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
//-31 -50 -8 -47 -48 4 33 -37 0 34 36 46 -49 -11 9 25 46 20 45 -23 -48 23 47 49
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>
#include <string>



//#include <SFML/Audio.hpp>

using namespace sf;

bool who = true; //рещает, будет ли тут ручное управление или нет.
const int num_of_bots = 10000;
bool visual = true;

void RunGame1() {



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

    //массив всех объектов карты (игрок, пули, прицел)
    std::list<Entity*>  entities;
    std::list<Bot*>  Bots_life;//здесь я храню живых ботов. Каждый такт я обновляю их текущее положение
    std::list<Bot*> Bots_death;//здесь я храню мертвых. Тех, что столкнулись со стеной. Когда все боты умирают, я выбираю из этого списка ботов бота, который оказался ближе всех к финишу.
    std::list<Entity*>::iterator it;
    std::list<Bot*>::iterator Bbot;
 //   map <int, Bot*> Best_bots; //здесь будут хранится топ 5 процентов от ботов.

    Person p(1);
    p.Player_maker(t, x_start, y_start);

//НЕКРАСИВЫЙ КОСТЫЛЬ В СОЗДАНИИ. ЧТО ДЕЛАТЬ?
    for (int i = 0; i < num_of_bots; i++) {
        Bots_life.push_back(new Bot(t, x_start, y_start));
    }
  /*  for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();Bbot++){
        (*Bbot)->Net.fGetKohonet(f_net_in);
    }*/




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
    time = 20;
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
            if (p.control(pos) && !space) {
                space = true;
                float dx2 = pos.x-p.rect.left; float dy2 = pos.y - p.rect.top;
                float m = sqrt(dx2*dx2 + dy2 * dy2);
                entities.push_back(new Bullet(p.rect.left, p.rect.top, t, speed_bul*dx2/m, speed_bul*dy2/m));
            } else { space = false;}
        }

//ВНИМАНИЕ ТУТ БОЛЬШОЙ ВОПРОС. Напиши b->update  !!!


//здесь я управляет бот! NO_HUMAN!
        bool qqq = false;//проверяю, есть ли еще живые боты
        for (Bbot = Bots_life.begin(); Bbot != Bots_life.end();){

            qqq = true;
            if (next_generation) {qqq = false;}
            Bot *b = *Bbot;//для удобства, чтобы не писать (*it)->
            b->control();
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
                (*Bbot)->rect.left = float(x_start * 16);
                (*Bbot)->rect.top = float(y_start * 16);
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



        if (who) {p.update(time);}
        Enemy.update(0);

//обновляю текущее положение объектов карты (таких как пули, линии прицела)
        for (it = entities.begin(); it != entities.end();){
			Entity *b = *it;//для удобства, чтобы не писать (*it)->
			b->update(time);//вызываем ф-цию update для всех объектов (по сути для тех, кто жив)
			if (b->life == false)	{ it = entities.erase(it); delete b; }// если этот объект мертв, то удаляем его
			else it++;//и идем курсором (итератором) к след объекту. так делаем со всеми объектами списка
		}


//смотрю, не переклись ли два каких либо объекта на карту. intersects - наложены ли две картинки?
        if (who) {if (p.rect.intersects(Enemy.rect) && Enemy.life) {
            Enemy.life = false;
        }}
        for (it = entities.begin(); it != entities.end(); it++){
			if ((*it)->rect.intersects(Enemy.rect) && Enemy.life) {
                Enemy.life = false;
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
#endif GAME_H



