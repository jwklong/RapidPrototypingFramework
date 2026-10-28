#pragma once

#include "Boy/Game.h"
#include "Boy/GamePadListener.h"
#include "BoyLib/Vector2.h"

class JoyStickTest: public Boy::Game, public Boy::GamePadListener
{
public:

	JoyStickTest();
	virtual ~JoyStickTest();

	// singleton:
	static JoyStickTest *instance();
	static void destroy();

	// implementation of Game:
	virtual void update(float dt);
	virtual void draw(Boy::Graphics *g);
	virtual void handleGamePadAdded(int gamePadId);
	virtual void handleGamePadRemoved(int gamePadId);

	// implementation of GamePadListener:
	virtual void gamePadButtonDown(Boy::GamePad *pad, Boy::GamePad::Button button);
	virtual void gamePadButtonUp(Boy::GamePad *pad, Boy::GamePad::Button button);

private:

	static JoyStickTest *gInstance;

	BoyLib::Vector2 mPos;

};
