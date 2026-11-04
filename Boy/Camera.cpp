#include "Camera.h"

#include <float.h>
#include "Environment.h"
#include "Graphics.h"

using namespace Boy;

Camera::Camera(const std::string &id) : SceneObject(id, 800)
{
    mOffset = BoyLib::Vector2(0, 0);
    mOffsetPositionRegister = BoyLib::Vector2(0, 0);
    mTargetVel = BoyLib::Vector2(0, 0);
    mVel = BoyLib::Vector2(0, 0);

    mFov = 0.9272952f;
    mZoom = 1.0f;
    mBaseZoom = 1.0f;
    mZoomMin = 0.0f;
    mZoomMax = FLT_MAX;
}

Camera::Camera(const Camera &original) : SceneObject(original)
{
    mOffset = BoyLib::Vector2(0, 0);
    mOffsetPositionRegister = BoyLib::Vector2(0, 0);
    mTargetVel = BoyLib::Vector2(0, 0);
    mVel = BoyLib::Vector2(0, 0);

    mFov = original.mFov;
    mZoom = original.mZoom;
    mBaseZoom = original.mBaseZoom;
    mZoomMin = original.mZoomMin;
    mZoomMax = original.mZoomMax;
}

const BoyLib::Vector2 &Camera::getPosition()
{
    mOffsetPositionRegister = BoyLib::Positionable::getPosition() + mOffset;
    return mOffsetPositionRegister;
}

float Camera::getZoom()
{
    Graphics *graphics = Boy::Environment::instance()->getGraphics();
    int screenWidth = graphics->getWidth();

    return (float)screenWidth / (mDepth * 2 * tanf(mFov * 0.5f));
}

void Camera::setZoom(float zoom)
{
    if (zoom < mZoomMin)
    {
        zoom = mZoomMin;
    }
    else if (zoom > mZoomMax)
    {
        zoom = mZoomMax;
    }
    mZoom = zoom;
    updateFov();
}

void Camera::updateFov()
{
    Graphics *graphics = Boy::Environment::instance()->getGraphics();
    int screenWidth = graphics->getWidth();

    mFov = atanf((float)(screenWidth / 2) / mDepth / (mZoom * mBaseZoom)) * 2;
}

bool Camera::canSee(const BoyLib::Vector2 &p, float depth, float marginH, float marginY)
{
    BoyLib::Vector2 screenPos = worldToScreen(p, depth);

    Graphics *graphics = Boy::Environment::instance()->getGraphics();
    float screenWidth = static_cast<float>(graphics->getWidth());
    float screenHeight = static_cast<float>(graphics->getHeight());

    return (screenPos.x > -marginH) &&
        (screenPos.x < screenWidth + marginH) &&
        (screenPos.y > -marginY) &&
        (screenPos.y < screenHeight + marginY);
}

BoyLib::Vector2 Camera::worldToScreen(const BoyLib::Vector2 &worldCoord, float depth)
{
    Graphics *graphics = Boy::Environment::instance()->getGraphics();
    int screenWidth = graphics->getWidth();
    int screenHeight = graphics->getHeight();

    float scaleFactor = (float)screenWidth / (tanf(mFov * 0.5f) * 2.0f);
    float worldCoordDist = mDepth - depth;

    BoyLib::Vector2 relativePos = getPosition() - worldCoord;
    BoyLib::Vector2 projectPos = (relativePos * scaleFactor) / worldCoordDist;

    return BoyLib::Vector2(
        (float)screenWidth * 0.5f + projectPos.x,
        (float)screenHeight * 0.5f - projectPos.y
    );
}

BoyLib::Vector2 Camera::screenToWorld(const BoyLib::Vector2 &screenCoord, float depth)
{
    Graphics *graphics = Boy::Environment::instance()->getGraphics();
    int screenWidth = graphics->getWidth();
    int screenHeight = graphics->getHeight();

    float scaleFactor = (float)screenWidth / (tanf(mFov * 0.5f) * 2.0f);
    float worldCoordDist = mDepth - depth;

    BoyLib::Vector2 projectPos = BoyLib::Vector2(
        screenCoord.x - ((float)screenWidth * 0.5f),
        ((float)screenHeight * 0.5f) - screenCoord.y
    );

    BoyLib::Vector2 relativePos = (projectPos * worldCoordDist) / scaleFactor;

    return getPosition() - relativePos;
}

void Camera::tick(float timestep)
{
    if (mVel != mTargetVel)
    {
        BoyLib::Vector2 diff = mTargetVel - mVel;
        float distance = diff.magnitude();

        if (timestep * 0.25f <= distance)
        {
            BoyLib::Vector2 direction = diff.normalize();
            mVel += diff.normalize() * timestep * 0.25f;
        }
        else
        {
            mVel = mTargetVel;
        }
    }
}

BoyLib::Cloneable *Camera::clone(std::map<BoyLib::Cloneable*, BoyLib::Cloneable*> *cloneMap)
{
    Camera *clone = new Camera(*this);
    if (cloneMap!=NULL)
    {
        (*cloneMap)[this] = clone;
    }
    return clone;
}