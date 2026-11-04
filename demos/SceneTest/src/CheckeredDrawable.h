#pragma once

#include "Boy/Drawable.h"
#include "Boy/Graphics.h"

class CheckeredDrawable : public Boy::Drawable
{
public:
    CheckeredDrawable(float rotSpeed);

    // impl of Boy::Drawable
    virtual void draw(Boy::Graphics *g, Boy::Camera *camera, Boy::SceneObject *anchor, bool inScreenSpace);
    virtual CheckeredDrawable *clone();

    // impl of Boy::Tickable
    virtual bool tick();

public:
    Boy::Image *mImage;
    float mRotSpeed;
    float mRotation;
};