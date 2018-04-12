//
// Created by Kevin Gaffney on 4/12/18.
//

#ifndef HAPLODIPLO_CONTROLLER_H
#define HAPLODIPLO_CONTROLLER_H


class Controller {
public:
    Controller();
    void control(double frontLeftInfraredValue, double frontFrontLeftInfraredValue,
                   double frontFrontRightInfraredValue, double frontRightInfraredValue,
                   double leftCameraValue, double rightCameraValue, double *controls);
private:
    int genome[33];
};


#endif //HAPLODIPLO_CONTROLLER_H
