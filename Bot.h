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
    void controlStep(double dt) override;
    double getFrontLeftInfraredValue();
    double getFrontFrontLeftInfraredValue();
    double getFrontFrontRightInfraredValue();
    double getFrontRightInfraredValue();
    double getLeftCameraValue();
    double getRightCameraValue();

private:
    Controller controller;
    double getInfraredValue(Enki::IRSensor irSensor);
    double getCameraValue(std::valarray<Enki::Color> image);
};


#endif //HAPLODIPLO_BOT_H
