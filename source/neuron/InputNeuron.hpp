#ifndef INPUTNEURON_H
#define INPUTNEURON_H

#include "Neuron.hpp"

class InputNeuron : public Neuron {
public:
    virtual int kernelFunction() {
//aaa++;
//for (int i = 0; i < aaa; i++) { printf("          ");} printf("%d\n", value);
//aaa--;
        return value;
    }

};


#endif INPUTNEURON_H
