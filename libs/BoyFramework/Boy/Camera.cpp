#include "Camera.h"

#include <float.h>

using namespace Boy;

Camera::Camera(const std::string &id) : SceneObject(id, 0)
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

BoyLib::Cloneable *Camera::clone(std::map<BoyLib::Cloneable*, BoyLib::Cloneable*> *cloneMap)
{
    Camera *clone = new Camera(*this);
    if (cloneMap!=NULL)
    {
        (*cloneMap)[this] = clone;
    }
    return clone;
}