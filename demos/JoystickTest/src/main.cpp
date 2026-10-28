#define SDL_MAIN_HANDLED

#include "Boy/Environment.h"
#include "JoystickTest.h"

int main(int argc, char* argv[])
{
	// initialize the platform:
	Boy::Environment::instance()->init(
		JoyStickTest::instance(),
		800, // width
		600, // height
		false, // fullscreen
		"Joystick test", // window title
		"demo1.dat", // persistence layer file name
		NULL); // no encryption

	// start the main loop
	Boy::Environment::instance()->startMainLoop();

	// destroy the game:
	JoyStickTest::destroy();

	// destroy the environment:
	Boy::Environment::instance()->destroy();

	return 0;
}

