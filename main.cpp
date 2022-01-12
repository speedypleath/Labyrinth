#include "include/Scene.h"

int main(int argc, char* argv[])
{
    Scene *scene = Scene::getInstance();
    scene->start(&argc, argv);
    delete scene;
    return 0;
}