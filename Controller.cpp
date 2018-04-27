//
// Created by Kevin Gaffney on 4/12/18.
//

#include <iostream>
#include <cmath>
#include <random>
#include <bitset>
#include "Controller.h"

using namespace std;

Controller::Controller() {

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<unsigned int> dist(0, 255);

    for (int i = 0; i < 33; i++) {
        genome[i] = dist(mt);
    }
}

Controller::Controller(const unsigned int *genome) {
    for (int i = 0; i < 33; i++) {
        this->genome[i] = genome[i];
    }
}

void Controller::control(double leftCameraValueR, double leftCameraValueG, double leftCameraValueB,
                         double rightCameraValueR, double rightCameraValueG, double rightCameraValueB,
                         double* controls) {

    double hidden[3];
    for (int i = 0; i < 3; i++) {
        hidden[i] = tanh(genome[i] * leftCameraValueR
                         + genome[3 + i] * leftCameraValueG
                         + genome[6 + i] * leftCameraValueB
                         + genome[9 + i] * rightCameraValueR
                         + genome[12 + i] * rightCameraValueG
                         + genome[15 + i] * rightCameraValueB
                         + genome[27 + i]);
    }

    for (int i = 0; i < 3; i++) {
        controls[i] = tanh(genome[18 + i] * hidden[0]
                           + genome[21 + i] * hidden[1]
                           + genome[24 + i] * hidden[2]
                           + genome[30 + i]);
    }
}

unsigned int* Controller::getGenome() {
    return genome;
}

vector<bitset<8>> Controller::getBinaryGenome() {
    vector<bitset<8>> binaryGenome;
    binaryGenome.reserve(33);
    for (unsigned int gene : genome) {
        binaryGenome.emplace_back(bitset<8>(gene));
    }
    return binaryGenome;
}

void Controller::setBinaryGenome(vector<bitset<8>> binaryGenome) {
    if (binaryGenome.size() != 33) return;
    for (unsigned long i = 0; i < 33; i++) {
        genome[i] = (unsigned int) binaryGenome.at(i).to_ulong();
    }
}
