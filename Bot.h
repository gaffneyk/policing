//
// Created by Kevin Gaffney on 3/3/18.
//

#ifndef HAPLODIPLO_BOT_H
#define HAPLODIPLO_BOT_H

#include <enki/robots/e-puck/EPuck.h>

class Bot: public Enki::EPuck {
public:
    Bot();
    void controlStep(double dt) override;
    double getCameraAverage(int rgb);
};


#endif //HAPLODIPLO_BOT_H
