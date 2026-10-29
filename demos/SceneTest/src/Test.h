#pragma once

#include "Boy/Game.h"
#include "Boy/KeyboardListener.h"
#include "Boy/Scene.h"

class Test : public Boy::Game, public Boy::KeyboardListener
{
public:
    Test();
    virtual ~Test();

    // singleton:
    static Test *instance();
    static void destroy();

    // implementation of Game:
    virtual void load();
    virtual void loadComplete();
    virtual void update(float dt);
    virtual void draw(Boy::Graphics *g);

    // implementation of KeyboardListener:
    virtual void keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);
    virtual void keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods);

private:
    static Test* gInstance;

    Boy::Scene *mScene;

    float mCameraSpeedX;
    float mCameraSpeedY;
};