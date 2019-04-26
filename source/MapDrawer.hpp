
#ifndef MAPDRAWER_H
#define MAPDRAWER_H



#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>


#include <iostream>


#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <fstream>



//#include <SFML/Audio.hpp>

using namespace sf;

const int H1 = 60;//высота
const int W1 = 60;//ширина

const int ZZ1 = 16;

void Draw() {
    RenderWindow window(VideoMode(960, 960), "SFML works!");

    int MapGame[H1 * W1];
    for (int i = 0; i < H1; i++) {
        for (int j = 0; j < W1; j++) {
            MapGame[j + i * W1] = 1;
        }
    }

    bool click = false;

    Texture t;
    t.loadFromFile("Mario_Tileset.png");
    Sprite tile(t);

     // куда я сохраняю нашу сетку. откуда

    ofstream fout;
    fout.open("map1.txt");

    ifstream fin;
    fin.open("map2.txt");

    printf("To change range:\nR = 3\nT = 5\nY = 7\n");



    Clock clock; // чтобы скорость была привязана ко времени, а не к тактовой частоте процессора

    int hhh = 1;

    while (window.isOpen()){



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

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            for (int i = -hhh; i <= hhh; i++) {
                for (int j = - hhh; j <= hhh; j++) {
                    MapGame[int(pos.x/16) + i + (int(pos.y/16) + j) * W1] = 0;
                }
            }
        }

//Ширина кисти, я не смог сделать напрямую через цифры, поэтому написано черех жопу. как костыль сойдет))
        if (Keyboard::isKeyPressed(Keyboard::R)) {hhh = 1;}
        if (Keyboard::isKeyPressed(Keyboard::T)) {hhh = 2;}
        if (Keyboard::isKeyPressed(Keyboard::Y)) {hhh = 4;}

        if (Keyboard::isKeyPressed(Keyboard::Z)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 1; //стенки
        }
        if (Keyboard::isKeyPressed(Keyboard::X)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 2; //финишняя черта
        }
        if (Keyboard::isKeyPressed(Keyboard::C)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 3; //место старта человечка
        }
        if (Keyboard::isKeyPressed(Keyboard::V)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 4; //эта штука нужна для корректной работы Дейкстры. Между стартом человека и линией финиша должна быть линия этой штуки
        }



        if (Keyboard::isKeyPressed(Keyboard::Escape)) {  window.close(); }


        window.clear(Color::White);

        for (int i = 0; i < H1; i++) {
			 for (int j = 0; j < W1; j++)
			  {
				if (MapGame[j + i * W1]== 1)  tile.setTextureRect( IntRect(143-ZZ*3,112,ZZ,ZZ) );
                if (MapGame[j + i * W1]== 2)  tile.setTextureRect( IntRect(143-ZZ*2,112 + 16,ZZ,ZZ) );
                if (MapGame[j + i * W1]== 3)  tile.setTextureRect( IntRect(143-ZZ*3,112 + 16,ZZ,ZZ) );
                if (MapGame[j + i * W1]== 4)  tile.setTextureRect( IntRect(143-ZZ*2,112,ZZ,ZZ) );

				if (MapGame[j + i * W1]== 0) continue;
  			    tile.setPosition(j*ZZ,i*ZZ) ;
		        window.draw(tile);
		     }
        }
        window.display();


    }

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            fout << MapGame[j + i * W];
        }
        fout << "\n";
    }
    fout.close();
}
#endif MAPDRAWER_H




