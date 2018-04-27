//
// Created by Kevin Gaffney on 4/12/18.
//

#ifndef HAPLODIPLO_CONTROLLER_H
#define HAPLODIPLO_CONTROLLER_H

#include <iostream>
#include <bitset>

class Controller {
public:
    Controller();
    Controller(const unsigned int *genome);
    void control(double leftCameraValueR, double leftCameraValueG,
                   double leftCameraValueB, double rightCameraValueR,
                   double rightCameraValueG, double rightCameraValue, double *controls);
    unsigned int* getGenome();
    std::vector<std::bitset<8>> getBinaryGenome();
    void setBinaryGenome(std::vector<std::bitset<8>> binaryGenome);
private:
    unsigned int genome[33];
};


#endif //HAPLODIPLO_CONTROLLER_H
