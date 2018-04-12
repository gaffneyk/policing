//
// Created by Kevin Gaffney on 4/12/18.
//

#include <iostream>
#include <cmath>
#include <random>
#include "Controller.h"

using namespace std;

Controller::Controller() {

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 256);

    for (int i = 0; i < 33; i++) {
        genome[i] = dist(mt);
    }
}

void Controller::control(double frontLeftInfraredValue, double frontFrontLeftInfraredValue,
                            double frontFrontRightInfraredValue, double frontRightInfraredValue,
                            double leftCameraValue, double rightCameraValue, double* controls) {

    double hidden[3];
    for (int i = 0; i < 3; i++) {
        hidden[i] = tanh(genome[i] * frontLeftInfraredValue
                         + genome[3 + i] * frontFrontLeftInfraredValue
                         + genome[6 + i] * frontFrontRightInfraredValue
                         + genome[9 + i] * frontRightInfraredValue
                         + genome[12 + i] * leftCameraValue
                         + genome[15 + i] * rightCameraValue
                         + genome[27 + i]);
    }

    for (int i = 0; i < 3; i++) {
        controls[i] = tanh(genome[18 + i] * hidden[0]
                           + genome[21 + i] * hidden[1]
                           + genome[24 + i] * hidden[2]
                           + genome[30 + i]);
    }
}
