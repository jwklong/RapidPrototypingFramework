#pragma once

#include "SceneObject.h"

namespace Boy
{
    class Camera : public SceneObject
    {
    public:
        Camera(const std::string &id);
        Camera(const Camera &camera);

        const BoyLib::Vector2 &getPosition();
        float getZoom();
        void setZoom(float zoom);
        void updateFov();

        bool canSee(const BoyLib::Vector2 &p, float depth, float marginH, float marginY);
        BoyLib::Vector2 worldToScreen(const BoyLib::Vector2 &worldCoord, float depth);
        BoyLib::Vector2 screenToWorld(const BoyLib::Vector2 &screenCoord, float depth);

        void tick(float timestep);

        //impl of cloneable
        virtual BoyLib::Cloneable *clone(std::map<BoyLib::Cloneable*,BoyLib::Cloneable*> *cloneMap);

    public:
        float mBaseZoom;

        BoyLib::Vector2 mVel;
        BoyLib::Vector2 mOffset;

    private:
        float mZoom;
        float mFov;
        float mZoomMin;
        float mZoomMax;

        BoyLib::Vector2 mOffsetPositionRegister;

        BoyLib::Vector2 mTargetVel;
    };
}
