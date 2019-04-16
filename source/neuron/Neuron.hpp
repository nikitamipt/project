#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

//using namespace std;

int aaa = 0;


class Neuron {
public:
    std::vector <int> weights;
    std::vector <Neuron*> dendrits;
    int value;
    Neuron() {
        value = 1;
    };

    int getSignal() {
        //return (1 / (1 + exp(-(kernelFunction())))) ;
        return kernelFunction();
    }

/*Суммирующая функция, которая суммирует(В самом простом случае)
входные сигналы*/
    virtual int kernelFunction() {
        int result = 0;
//aaa++;
        for(int i = 0; i < dendrits.size(); i++){
            int dentr = dendrits[i]->getSignal();
            result += dentr * weights[i];
//for (int i = 0; i < aaa; i++) { printf("          ");} printf("dentris[%d] = %d    weight = %d    cerresult = %d\n", i, dentr, weights[i], result);
        }
//for (int i = 0; i < aaa; i++) { printf("          ");} printf("%d\n", result);
//aa--;
        return result;
    }
};



#endif NEURON_H
