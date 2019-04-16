#ifndef KOHONENNET_H
#define KOHONENNET_H

#include <vector>
#include <iomanip>
#include <fstream>
using std::stringstream;
#include <iostream>


#include "InputNeuron.hpp"
#include "Neuron.hpp"

const int inputs = 5; //5 входов
const int mids = 3;   //3 средних
const int outs = 1;    //1 выход

int chislo = 10; //максимальный коэффициент весов

using namespace std;


class KohonenNet{
public:

    std::vector<InputNeuron*> inNeurons;
    std::vector<Neuron*> outNeurons;
    std::vector<Neuron*> secNeurons;


    KohonenNet() {
        for (int i = 0; i < inputs; i++) {
            inNeurons.push_back(new InputNeuron());
        }
        for (int i = 0; i < mids; i++) {
            secNeurons.push_back(new Neuron());
        }
        for (int i = 0; i < outs; i++) {
            outNeurons.push_back(new Neuron());
        }
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {
                secNeurons[i]->dendrits.push_back(inNeurons[j]);
                secNeurons[i]->weights.push_back(/*1.0);*/rand()%chislo - chislo/2);
            }
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {
                outNeurons[j]->dendrits.push_back(secNeurons[i]);
                outNeurons[j]->weights.push_back(/*1.0);//*/rand()%chislo - chislo/2);
            }
        }
    }

//сохраняет сетку в файл
    void fSaveKohonet(ofstream &fout) {
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {
              //  fout << std::fixed <<  std :: setprecision ( 5 )<< secNeurons[i]->weights[j] << " ";
                fout << secNeurons[i]->weights[j] << " ";
            }
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {
                //fout << std::fixed << std :: setprecision ( 5 )<<  outNeurons[j]->weights[j] << " ";
                fout << outNeurons[j]->weights[i] << " ";

            }
        }
        fout << '\n';
    }
    void rSaveKohonet() {
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {
               // cout << std::fixed <<  std :: setprecision ( 5 )<< secNeurons[i]->weights[j] << " ";
                cout << secNeurons[i]->weights[j] << " ";
            }
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {
              //  cout << std::fixed << std :: setprecision ( 5 )<<  outNeurons[j]->weights[j] << " ";
                cout << outNeurons[j]->weights[i] << " ";

            }
        }
        cout << '\n';
    }
    //загружает сетку из файла
    void fGetKohonet(ifstream &fin) {
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {
                fin >> secNeurons[i]->weights[j];
                //fin >> std::fixed >> std :: setprecision ( 5 ) >>  secNeurons[i]->weights[j] >> " ";
            }
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {
                fin >> outNeurons[j]->weights[i];
               // fin >> std::fixed >> std :: setprecision ( 5 ) >>  outNeurons[j]->weights[j] >> " ";
            }
        }
    }

    //сохраняет сетку в массив
    void mSaveKohonet(int* mas) {
        int q = 0;
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {  mas[q] = secNeurons[i]->weights[j]; q++;}
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {mas[q] = outNeurons[j]->weights[i]; q++;}
        }
    }

    //загружает сетку из массива  //rnd = 1 - когда 10% массива надо заменить на рандомные числа
    //заметь, что сетка будет симметричная
    void mGetKohonet(int* mas, int rnd) {
        int q = 0;
        for (int i = 0; i < mids; i++) {
            for (int j = 0; j < inputs; j++) {
                int w = rand()%3;//с вероятностью 50% происходит замена числа
                if ((w==0) && (rnd > 0)) {secNeurons[i]->weights[j] = rand()%chislo - chislo/2; }
                else {secNeurons[i]->weights[j] = mas[q];}
                q++;
            }
        }
        for (int j = 0; j < outs; j++) {
            for (int i = 0; i < mids; i++) {
                int w = rand()%3;
                if ((w == 0) && (rnd > 0)) {outNeurons[j]->weights[i]  = rand()%chislo - chislo/2; }
                else {outNeurons[j]->weights[i] = mas[q]; }
                q++;
            }
        }

        for (int i = 0; i < mids/2; i++) {
            for (int j = 0; j < inputs/2; j++) {
                secNeurons[mids - i - 1]->weights[inputs - j - 1] = secNeurons[i]->weights[j];

            }
        }
        for (int j = 0; j < outs/2; j++) {
            for (int i = 0; i < mids/2; i++) {
                outNeurons[outs - j - 1]->weights[mids - i - 1] = outNeurons[j]->weights[i];
            }
        }
    }

};

#endif KOHONENNET_H
