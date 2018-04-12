#include <iostream>
#include <enki/PhysicalEngine.h>
#include <viewer/Viewer.h>
#include <QApplication>
#include "Bot.h"

using namespace Enki;
using namespace std;


void addWall(Color color, double sizeX, double sizeY, Point position, World* world) {
    auto *wall = new PhysicalObject;
    wall->setColor(color);
    wall->setRectangular(sizeX, sizeY, 10, -1);
    wall->pos = position;
    world->addObject(wall);
}

int main(int argc, char *argv[]) {

    const int worldWidth = 100;
    const int worldHeight = 100;

    QApplication app(argc, argv);

    World world(worldWidth, worldHeight, Color(1, 1, 1), World::GroundTexture());

    auto *bot = new Bot;
    bot->pos = Point(20, 20);
    world.addObject(bot);
    bot->rightSpeed = 0;
    bot->leftSpeed = 0;

    addWall(Color(1, 1, 1), worldWidth, 0.1, Point(0, worldHeight), &world); // Top wall
    addWall(Color(0, 0, 0), worldWidth, 0.1, Point(0, 0), &world); // Bottom wall
    addWall(Color(0, 0, 0), 0.1, worldHeight, Point(worldWidth, 0), &world); // Right wall
    addWall(Color(0, 0, 0), 0.1, worldHeight, Point(0, 0), &world); // Left wall

    ViewerWidget viewer(&world);
    viewer.show();
    return app.exec();
}