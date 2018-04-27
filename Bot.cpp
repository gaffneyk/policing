//
// Created by Kevin Gaffney on 3/3/18.
//

#include <iostream>
#include "Bot.h"

using namespace Enki;
using namespace std;

Bot::Bot():
        EPuck(CAPABILITY_BASIC_SENSORS | CAPABILITY_CAMERA) {
    controller = new Controller();
    setColor(Color(0.2, 0.2, 0.2));
    fitnessScore = 0;
};

Bot::Bot(Bot &bot):
        Bot() {
    this->controller = new Controller(bot.getController()->getGenome());
}

void Bot::controlStep(double dt) {
    EPuck::controlStep(dt);
    double controls[3];
    controller->control(getLeftCameraValue(0), getLeftCameraValue(1), getLeftCameraValue(2),
                       getRightCameraValue(0), getRightCameraValue(1), getRightCameraValue(2),
                       controls);
    leftSpeed = maxSpeed * controls[0];
    rightSpeed = maxSpeed * controls[1];
    shouldShareToken = controls[2] > 0;
}

double Bot::getLeftCameraValue(int rgb) {
    auto image = this->camera.image;
    return getCameraValue(image[slice(image.size() / 2, image.size() / 2, 1)], rgb);
}

double Bot::getRightCameraValue(int rgb) {
    auto image = this->camera.image;
    return getCameraValue(image[slice(0, image.size() / 2, 1)], rgb);
}

double Bot::getCameraValue(valarray<Color> image, int rgb) {
    double sum = 0;
    for (Color color : image) {
        sum += color.components[rgb];
    }
    return sum / image.size() * 2 - 1;
}

void Bot::increaseFitnessScore(double amount) {
    fitnessScore += amount;
}

bool Bot::getShouldShareToken() {
    return shouldShareToken;
}

double Bot::getFitnessScore() {
    return fitnessScore;
}

Controller* Bot::getController() {
    return controller;
}


