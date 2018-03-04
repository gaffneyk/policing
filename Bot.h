//
// Created by Kevin Gaffney on 3/3/18.
//

#ifndef HAPLODIPLO_BOT_H
#define HAPLODIPLO_BOT_H

#include <enki/robots/e-puck/EPuck.h>

class Bot: public Enki::EPuck {
public:
    Enki::CircularCam highCamera;
    Bot();
    void controlStep(double dt) override;
    double getLowCameraAverage(int rgba);
    double getHighCameraAverage(int rgba);

private:
    double getCameraAverage(int rgba, Enki::CircularCam camera);
};


#endif //HAPLODIPLO_BOT_H
