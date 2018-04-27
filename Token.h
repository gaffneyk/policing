//
// Created by Kevin Gaffney on 4/12/18.
//

#ifndef HAPLODIPLO_TOKEN_H
#define HAPLODIPLO_TOKEN_H


#include <enki/PhysicalEngine.h>
#include "Bot.h"

enum TokenStatus {
    unclaimed,
    kept,
    shared
};

class Token: public Enki::PhysicalObject {
public:
    Token();
    TokenStatus getStatus();
    Bot *getInitialBotCollided();
protected:
    void collisionEvent(Enki::PhysicalObject *o) override;
private:
    Bot *initialBotCollided = nullptr;
    TokenStatus status;
};


#endif //HAPLODIPLO_TOKEN_H
