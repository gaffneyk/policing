//
// Created by Kevin Gaffney on 3/3/18.
//

#include <iostream>
#include "Bot.h"

using namespace Enki;
using namespace std;

Bot::Bot():
        EPuck(CAPABILITY_BASIC_SENSORS | CAPABILITY_CAMERA) {
    setColor(Color(1, 1, 1));
};

void Bot::controlStep(double dt) {
    EPuck::controlStep(dt);
    double controls[3];
    controller.control(getFrontLeftInfraredValue(), getFrontFrontLeftInfraredValue(),
                       getFrontFrontRightInfraredValue(), getFrontRightInfraredValue(),
                       getLeftCameraValue(), getRightCameraValue(), controls);
    leftSpeed = maxSpeed * controls[0];
    rightSpeed = maxSpeed * controls[1];
}

double Bot::getFrontLeftInfraredValue() {
    return getInfraredValue(this->infraredSensor6);
}

double Bot::getFrontFrontLeftInfraredValue() {
    return getInfraredValue(this->infraredSensor7);
}

double Bot::getFrontFrontRightInfraredValue() {
    return getInfraredValue(this->infraredSensor0);
}

double Bot::getFrontRightInfraredValue() {
    return getInfraredValue(this->infraredSensor1);
}

double Bot::getLeftCameraValue() {
    auto image = this->camera.image;
    return getCameraValue(image[slice(image.size() / 2, image.size() / 2, 1)]);
}

double Bot::getRightCameraValue() {
    auto image = this->camera.image;
    return getCameraValue(image[slice(0, image.size() / 2, 1)]);
}

double Bot::getInfraredValue(Enki::IRSensor irSensor) {
    return irSensor.getDist() / irSensor.getRange() * 2 - 1;
}

double Bot::getCameraValue(valarray<Color> image) {
    double sum = 0;
    for (Color color : image) {
        sum += color.toGray();
    }
    return sum / image.size() * 2 - 1;
}

