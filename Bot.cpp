//
// Created by Kevin Gaffney on 3/3/18.
//

#include <iostream>
#include "Bot.h"

using namespace Enki;
using namespace std;

Bot::Bot():
        EPuck(CAPABILITY_CAMERA),
        highCamera(this, Vector(3.7, 0), 3, 0, M_PI/6, 60) {
    addLocalInteraction(&highCamera);
    setColor(Color(1, 1, 1));
};

void Bot::controlStep(double dt) {
    EPuck::controlStep(dt);
    cout << getLowCameraAverage(0) << endl;
}

double Bot::getLowCameraAverage(int rgba) {
    return getCameraAverage(rgba, camera);
}

double Bot::getHighCameraAverage(int rgba) {
    return getCameraAverage(rgba, highCamera);
}

double Bot::getCameraAverage(int rgba, Enki::CircularCam camera) {
    if (rgba < 0 || rgba > 3) {
        return 0;
    }
    double sum = 0;
    for (Color color : camera.image) {
        sum += color.components[rgba];
    }
    return sum / camera.image.size();
}
