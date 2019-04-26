#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#pragma once
#include <queue>

using namespace std;

//гарантировано, что на карте будет граница! (чтобы не уходить за края массива)


struct kub_1 {
    int x; int y;
    int s; //расстоние от первого до него
};

typedef struct kub_1 kub;


int checker(int Map2[H*W], queue <kub> *q, kub kk1, int i, int j) {
    if ((kk1.x + i >= W) || (kk1.x + i < 0) || (kk1.y + j < 0) || (kk1.y + j >= H)) {return 0;}
    if (Map2[(kk1.x + i) + W*(kk1.y + j)] == 0) {
       if (TileMap[kk1.y + j][kk1.x + i] == '0') {
            Map2[(kk1.x + i) + W*(kk1.y + j)] = 1;
            kub kk2;
            kk2.x = (kk1.x + i); kk2.y = (kk1.y + j); kk2.s = kk1.s + 1;
            q->push(kk2);
        } else if (TileMap[kk1.y + j][kk1.x + i] == '2') {
            return kk1.s + 1;
        } else {
            Map2[(kk1.x + i) + W*(kk1.y + j)] = 2;
        }
    }
    return 0;
}

int Dijkstra(int x_start, int y_start) {
    int Map2[H*W];
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            Map2[i + j * W] = 0;
        }
    }

    queue <kub> q;
    kub starter;
    starter.x = x_start;  starter.y = y_start; starter.s = 0;
    Map2[x_start + W*y_start] = 1;
    q.push(starter);
    int result = 0;
    bool finish = false;
    while (!q.empty() and !result) {
        kub kk1 = q.front();
        result = checker(Map2, &q, kk1, 1, 0);if (result) break;
        result = checker(Map2, &q, kk1, -1, 0);if (result) break;
        result = checker(Map2, &q, kk1, 0, 1);if (result) break;
        result = checker(Map2, &q, kk1, 0, -1);if (result) break;
        q.pop();

/*for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
        printf("%d", Map2[j + i*W]);
    }
    printf("\n");
}*/

    }

    if (result) {
        return result;
    } else {
        return -1;
    }
}



#endif DIJKSTRA_H
