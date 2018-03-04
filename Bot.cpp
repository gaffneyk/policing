//
// Created by Kevin Gaffney on 3/3/18.
//

#include <iostream>
#include "Bot.h"

using namespace Enki;
using namespace std;

Bot::Bot(): EPuck(CAPABILITY_CAMERA) {
    this->setColor(Color(1, 1, 1));
};

void Bot::controlStep(double dt) {
    EPuck::controlStep(dt);
    cout << this->getCameraAverage(1) << endl;
}

double Bot::getCameraAverage(int rgb) {
    double sum = 0;
    for (Color color : this->camera.image) {
        switch(rgb) {
            case 0:
                sum += color.r();
            case 1:
                sum += color.g();
            case 2:
                sum += color.b();
            default:
                break;
        }
    }
    return sum / this->camera.image.size();
}



