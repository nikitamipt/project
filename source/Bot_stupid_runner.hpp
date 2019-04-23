

#ifndef RUNNER_H
#define RUNNER_H


#include "Player.hpp"
#include "neuron/KohonenNet.hpp"
#pragma once

using namespace sf;


float dist1[5] = {3.1415/2, 3.1415/4, 0, -3.1415/4, -3.1415/2};


int GetDistace(int x, int y, float qqq, float da) {
    int s = 0;
    if ((y/16 < 0) || (y/16 >= H) || (x/16 < 0) || (x/16 >=W)) {return s;}
    while (TileMap[int(y/16)][int(x/16)] == '0') {
        x += 8 * cos(da + qqq); y += 8 * sin(da + qqq); s++;
        if ((y/16 < 0) || (y/16 >= H) || (x/16 < 0) || (x/16 >=W)) {return s;}
    }
   // printf("s = %d\n\n", s);
    return s;
}



class Runner : public Player {
public:
    unsigned long long  int score;  //эта штука показывает, насколько далеко от стенок находился наш игрок
    int dist_to_finish;
    KohonenNet Net;
    ofstream fout;

// ЭТО КОСТЫЛЬ. КАК НЕ ПОВТОРЯТЬ ТОТ ЖЕ ТЕКСТ, что и PERSON
    Runner(Texture &image, int x_start, int y_start) {
        sprite.setTexture(image);  //сначала в sprite загружаю картину
        rect = FloatRect(x_start * 16, y_start * 16,16,16);//текущие координаты, где я заспамлюсь
        sprite.setPosition(rect.left, rect.top);
        sprite.setTextureRect(IntRect(5*16, 9*16, 16, 16));
        dx = 0; dy = 0; da = 0;
        currentFrame = 0;
        score = 0;
        dsp = 1; da = 0;
        life = true;
    }

    bool control() {
        int score1 = 0;
        for (int i = 0; i < 5; i++) {
            int sc = GetDistace(rect.left + 8, rect.top + 8, dist1[i], da);
//printf("%d ", sc);
            score1 += sc;
            Net.inNeurons[i]->value = sc;
        }
//printf("\n");
        score += score1;
        float da1 = float(Net.outNeurons[0]->kernelFunction()) / 700;
        if (da1 < -3.1415/2){da1 = -3.1415/2;} if (da1 > 3.1415/2) {da1 = 3.1415/2;}
        da += da1;
        dx = dsp*cos(da); dy = dsp*sin(da);
        return false;
    }

};



#endif RUNNER_H


