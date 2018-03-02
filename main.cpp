#include <iostream>
#include <enki/PhysicalEngine.h>
#include <viewer/Viewer.h>
#include <QApplication>

using namespace Enki;
using namespace std;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    World world(120, Color(0.9, 0.9, 0.9), World::GroundTexture());
    ViewerWidget viewer(&world, nullptr);
    viewer.show();

    return app.exec();
}