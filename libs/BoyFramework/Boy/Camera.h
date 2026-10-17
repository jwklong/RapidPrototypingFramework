#pragma once

#include "SceneObject.h"

namespace Boy
{
    class Camera : public SceneObject
    {
    public:
        Camera(const std::string &id);
        Camera(const Camera &original);

        //impl of cloneable
        virtual BoyLib::Cloneable *clone(std::map<BoyLib::Cloneable*,BoyLib::Cloneable*> *cloneMap);

    private:
        float mZoom;
        float mBaseZoom;
        float mFov;
        float mZoomMin;
        float mZoomMax;

        BoyLib::Vector2 mOffset;
        BoyLib::Vector2 mOffsetPositionRegister;

        BoyLib::Vector2 mTargetVel;
        BoyLib::Vector2 mVel;
    };
}
