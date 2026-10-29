#include "Test.h"

#include <assert.h>
#include "Boy/Environment.h"
#include "Boy/Graphics.h"
#include "Boy/ResourceManager.h"
#include "CheckeredDrawable.h"
#include "Boy/Camera.h"
#include "Boy/Keyboard.h"
#include "Boy/SceneObject.h"

Test *Test::gInstance = NULL;

Test::Test()
{
    mScene = new Boy::Scene("", false);
    mScene->mMinX = -450;
    mScene->mMaxX = 450;
    mScene->mMinY = -350;
    mScene->mMaxY = 350;

    mCameraSpeedX = 0;
    mCameraSpeedY = 0;
}

Test::~Test()
{
	Boy::Environment::instance()->getKeyboard(0)->removeListener(this);
}

Test *Test::instance()
{
    if (gInstance==NULL)
    {
        gInstance = new Test();
    }

    return gInstance;
}

void Test::destroy()
{
    assert(gInstance!=NULL);
    delete gInstance;
    gInstance = NULL;
}

void Test::load()
{
    Boy::ResourceManager *rm = Boy::Environment::instance()->getResourceManager();
    rm->parseResourceFile("res/resources.xml");
    rm->loadResourceGroup("common");
}

void Test::loadComplete()
{
    Boy::Environment::instance()->getKeyboard(0)->addListener(this);

    Boy::SceneObject *objectA = new Boy::SceneObject("", 1);
    objectA->mDrawable = new CheckeredDrawable(1);
    objectA->setPosition(-100, 0);
    mScene->addObject(objectA);

    Boy::SceneObject *objectB = new Boy::SceneObject("", 3);
    objectB->mDrawable = new CheckeredDrawable(0);
    objectB->setPosition(300, 0);
    mScene->addObject(objectB);

    Boy::SceneObject *objectC = new Boy::SceneObject("", 1);
    objectC->mDrawable = new CheckeredDrawable(0);
    objectC->setPosition(50, 50);
    objectC->mIsOverlay = true;
    objectC->mContext = Boy::SceneObject::SCREEN_COORDS;
    mScene->addObject(objectC);
}

void Test::update(float dt)
{
    if (mCameraSpeedX != 0 || mCameraSpeedY != 0)
    {
        mScene->mCamera->addPosition(BoyLib::Vector2(mCameraSpeedX * dt, mCameraSpeedY * dt));
        mScene->ensureCameraInBounds(false);
    }

    mScene->tick(dt);
}

void Test::draw(Boy::Graphics *g)
{
    mScene->draw(g, 0, 100, true);
    mScene->drawOverlays(g);
}

void Test::keyDown(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
    switch (key)
    {
        case Boy::Keyboard::KEY_LEFT:
            mCameraSpeedX -= 100;
            break;
        case Boy::Keyboard::KEY_RIGHT:
            mCameraSpeedX += 100;
            break;
        case Boy::Keyboard::KEY_DOWN:
            mCameraSpeedY -= 100;
            break;
        case Boy::Keyboard::KEY_UP:
            mCameraSpeedY += 100;
            break;
    }
}

void Test::keyUp(wchar_t unicode, Boy::Keyboard::Key key, Boy::Keyboard::Modifiers mods)
{
    switch (key)
    {
        case Boy::Keyboard::KEY_LEFT:
            mCameraSpeedX += 100;
            break;
        case Boy::Keyboard::KEY_RIGHT:
            mCameraSpeedX -= 100;
            break;
        case Boy::Keyboard::KEY_DOWN:
            mCameraSpeedY += 100;
            break;
        case Boy::Keyboard::KEY_UP:
            mCameraSpeedY -= 100;
            break;
    }
}
