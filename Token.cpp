//
// Created by Kevin Gaffney on 4/12/18.
//

#include "Token.h"
#include <iostream>

using namespace Enki;
using namespace std;


Token::Token() {
    dryFrictionCoefficient = 10;
    setColor(Color(0, 0, 1));
    setCylindric(3, 3, 10);
}

void Token::collisionEvent(PhysicalObject *o) {
    PhysicalObject::collisionEvent(o);
    auto bot = dynamic_cast<Bot*>(o);
    if (bot) {
        if (!initialBotCollided) {
            initialBotCollided = bot;
            if (bot->getShouldShareToken()) {
                status = shared;
                setColor(Color(0, 1, 0));
            } else {
                status = kept;
                setColor(Color(1, 0, 0));
            }
        } else if (bot != initialBotCollided) {
            // TODO: handle collision by different bot
        }
    }
}

TokenStatus Token::getStatus() {
    return status;
}

Bot *Token::getInitialBotCollided() {
    return initialBotCollided;
}
