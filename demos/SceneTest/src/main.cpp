#define SDL_MAIN_HANDLED

#include "Boy/Environment.h"
#include "Test.h"

int main(int argc, char* args[]) {

    Boy::Environment::instance()->init(
        Test::instance(),
        800, // width
        600, // height
        false, // fullscreen
        "Scene Test", // window title
        "scenetest.dat", // persistence layer file name
        NULL); // no encryption

    Boy::Environment::instance()->startMainLoop();

    Test::destroy();
    Boy::Environment::instance()->destroy();

    return 0;
}
