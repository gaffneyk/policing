#include <iostream>
#include <enki/PhysicalEngine.h>
#include <viewer/Viewer.h>
#include <QApplication>
#include "Bot.h"

using namespace Enki;
using namespace std;

int main(int argc, char *argv[]) {

    const int worldWidth = 100;
    const int worldHeight = 100;

    QApplication app(argc, argv);

    World world(worldWidth, worldHeight, Color(1, 1, 1), World::GroundTexture());

    auto *bot = new Bot;
    bot->pos = Point(20, 20);
    world.addObject(bot);
    bot->rightSpeed = 1;
    bot->leftSpeed = -1;

    auto *bottomWall = new PhysicalObject;
    bottomWall->setRectangular(worldWidth, 0.1, 10, -1);
    bottomWall->pos = Point(0, 0);
    world.addObject(bottomWall);

    auto *topWall = new PhysicalObject;
    topWall->setColor(Color(1, 0, 0));
    topWall->setRectangular(worldWidth, 0.1, 10, -1);
    topWall->pos = Point(0, worldHeight);
    world.addObject(topWall);

    auto *leftWall = new PhysicalObject;
    leftWall->setColor(Color(0, 1, 0));
    leftWall->setRectangular(0.1, worldHeight, 10, -1);
    leftWall->pos = Point(0, 0);
    world.addObject(leftWall);

    auto *rightWall = new PhysicalObject;
    rightWall->setColor(Color(0, 0, 1));
    rightWall->setRectangular(0.1, worldHeight, 10, -1);
    rightWall->pos = Point(worldWidth, 0);
    world.addObject(rightWall);

    auto *redThing = new PhysicalObject;
    redThing->setColor(Color(1, 0, 0));
    redThing->setCylindric(3, 10, -1);
    redThing->pos = Point(worldWidth / 2, 3);
//    world.addObject(redThing);

    ViewerWidget viewer(&world);
    viewer.show();
    return app.exec();
}