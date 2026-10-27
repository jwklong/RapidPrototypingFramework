#include "JoystickTest.h"

#include <assert.h>
#include "Boy/Environment.h"
#include "Boy/GamePad.h"
#include "Boy/Graphics.h"
#include "Boy/Mouse.h"
#include <string>

JoyStickTest *JoyStickTest::gInstance = NULL;

JoyStickTest::JoyStickTest()
{
}

JoyStickTest::~JoyStickTest()
{
}

JoyStickTest *JoyStickTest::instance()
{
	if (gInstance==NULL)
	{
		gInstance = new JoyStickTest();
	}

	return gInstance;
}

void JoyStickTest::destroy()
{
	assert(gInstance!=NULL);
	delete gInstance;
	gInstance = NULL;
}

void JoyStickTest::update(float dt)
{
	Boy::Graphics *g = Boy::Environment::instance()->getGraphics();
	float w = (float)g->getWidth();
	float h = (float)g->getHeight();

	Boy::GamePad *pad = Boy::Environment::instance()->getGamePad(0);
	mPos = pad->getAnalogR();
	mPos.x = w/2 + mPos.x*w/2;
	mPos.y = h/2 - mPos.y*h/2;
}

void JoyStickTest::draw(Boy::Graphics *g)
{
	g->setColor(0xFFFFFFFF);
	g->fillRect((int)mPos.x-5,(int)mPos.y-5,10,10);
}

void JoyStickTest::handleGamePadAdded(int gamePadId)
{
	Boy::Environment::instance()->getGamePad(gamePadId)->addListener(this);
}

void JoyStickTest::handleGamePadRemoved(int gamePadId)
{
	Boy::Environment::instance()->getGamePad(gamePadId)->removeListener(this);
}

void JoyStickTest::gamePadButtonDown(Boy::GamePad *pad, Boy::GamePad::Button button)
{
	Boy::Environment::instance()->stopMainLoop();
}

void JoyStickTest::gamePadButtonUp(Boy::GamePad *pad, Boy::GamePad::Button button)
{
}
