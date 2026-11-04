#include "CheckeredDrawable.h"

#include "Boy/Camera.h"
#include "Boy/Environment.h"
#include "Boy/SceneObject.h"

CheckeredDrawable::CheckeredDrawable(float rotSpeed)
{
    mImage = Boy::Environment::getImage("IMAGE_CHECKERED");
    mRotSpeed = rotSpeed;
    mRotation = 0;
}

void CheckeredDrawable::draw(Boy::Graphics *g, Boy::Camera *camera, Boy::SceneObject *anchor, bool inScreenSpace)
{
    g->pushTransform();

    g->rotateDeg(mRotation);
    if (inScreenSpace)
    {
        BoyLib::Vector2 pos = anchor->getPosition();
        g->translate(pos.x, pos.y);
    }
    else
    {
        float cameraZoom = camera->getZoom();
        g->scale(cameraZoom, cameraZoom);
        BoyLib::Vector2 pos = camera->worldToScreen(anchor->getPosition(), anchor->mDepth);
        g->translate(pos.x, pos.y);
    }

    g->drawImage(mImage);
    g->popTransform();
}

CheckeredDrawable *CheckeredDrawable::clone()
{
    return new CheckeredDrawable(mRotSpeed);
}

bool CheckeredDrawable::tick()
{
    mRotation += mRotSpeed;

    return false;
}
