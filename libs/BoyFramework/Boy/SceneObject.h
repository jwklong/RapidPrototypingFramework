#pragma once

#include <string>

#include "BoyLib/Boundary.h"
#include "BoyLib/Positionable.h"
#include "BoyLib/Tickable.h"
#include "BoyLib/Vector2.h"
#include "Drawable.h"
#include "Mouse.h"

namespace Boy
{
    class Graphics;
    class Scene;

    class SceneObject : public BoyLib::Positionable, public BoyLib::Tickable
    {
    public:

        enum Context
        {
            SCREEN_COORDS,
            WORLD_COORDS
        };

        SceneObject(std::string id, float depth);
        SceneObject(const SceneObject &original);
        virtual ~SceneObject();

        virtual void draw(Graphics *graphics, Camera *camera);

        void handleAdd(Scene *scene);
        void handleRemove();

        bool contains(const BoyLib::Vector2 &vector);

        bool isVisible() { return mIsVisible; }
        bool isTransient() { return mIsTransient; }

        virtual void mouseDown(Mouse *mouse, Vector2 point, Mouse::Button button) {}
        virtual void mouseUp(Mouse *mouse, Vector2 point, Mouse::Button button) {}
        virtual void mouseMove(Mouse *mouse, Vector2 point) {}
        virtual void mouseEnter(Mouse *mouse) {}
        virtual void mouseLeave(Mouse *mouse) {}


    protected:
        std::string mId;
        float mDepth;
        bool mIsOverlay;
        Drawable *mDrawable;
        bool mDeleteDrawableOnDestruction;
        bool mIsVisible;
        Scene *mScene;
        bool mIsTransient;
        BoyLib::Boundary *mBoundary;
        Context mContext;
    };
}
