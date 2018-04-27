//
// Created by Kevin Gaffney on 3/3/18.
//

#ifndef HAPLODIPLO_BOT_H
#define HAPLODIPLO_BOT_H

#include <enki/robots/e-puck/EPuck.h>
#include "Controller.h"

class Bot: public Enki::EPuck {
public:
    Bot();
    Bot(Bot &obj);
    void controlStep(double dt) override;
    double getLeftCameraValue(int rgb);
    double getRightCameraValue(int rgb);
    void increaseFitnessScore(double amount);
    double getFitnessScore();
    bool getShouldShareToken();
    Controller* getController();

private:
    Controller *controller;
    double fitnessScore;
    double getCameraValue(std::valarray<Enki::Color> image, int rgb);
    bool shouldShareToken;
};


#endif //HAPLODIPLO_BOT_H
