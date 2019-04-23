
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

const int H1 = 60;//������
const int W1 = 60;//������

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

     // ���� � �������� ���� �����. ������

    ofstream fout;
    fout.open("map1.txt");

    ifstream fin;
    fin.open("map2.txt");

    printf("To change range:\nR = 3\nT = 5\nY = 7\n");



    Clock clock; // ����� �������� ���� ��������� �� �������, � �� � �������� ������� ����������

    int hhh = 1;

    while (window.isOpen()){



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

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            for (int i = -hhh; i <= hhh; i++) {
                for (int j = - hhh; j <= hhh; j++) {
                    MapGame[int(pos.x/16) + i + (int(pos.y/16) + j) * W1] = 0;
                }
            }
        }

//������ �����, � �� ���� ������� �������� ����� �����, ������� �������� ����� ����. ��� ������� ������))
        if (Keyboard::isKeyPressed(Keyboard::R)) {hhh = 1;}
        if (Keyboard::isKeyPressed(Keyboard::T)) {hhh = 2;}
        if (Keyboard::isKeyPressed(Keyboard::Y)) {hhh = 4;}

        if (Keyboard::isKeyPressed(Keyboard::Z)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 1; //������
        }
        if (Keyboard::isKeyPressed(Keyboard::X)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 2; //�������� �����
        }
        if (Keyboard::isKeyPressed(Keyboard::C)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 3; //����� ������ ���������
        }
        if (Keyboard::isKeyPressed(Keyboard::V)) {
            MapGame[int(pos.x/16) + int(pos.y/16) * W1] = 4; //��� ����� ����� ��� ���������� ������ ��������. ����� ������� �������� � ������ ������ ������ ���� ����� ���� �����
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




