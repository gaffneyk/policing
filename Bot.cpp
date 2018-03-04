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
    cout << this->getCameraAverage(0) << endl;
}

double Bot::getCameraAverage(int rgba) {
    if (rgba < 0 || rgba > 3) {
        return 0;
    }

    double sum = 0;
    for (Color color : this->camera.image) {
        sum += color.components[rgba];
    }
    return sum / this->camera.image.size();
}



