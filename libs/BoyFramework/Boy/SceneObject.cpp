#include "SceneObject.h"

#include <assert.h>
#include "BoyLib/BoyUtil.h"

using namespace Boy;

SceneObject::SceneObject(std::string id, float depth)
{
    mId = id;
    mDepth = depth;
    mDrawable = NULL;
    mIsVisible = true;
    mScene = NULL;
    mIsOverlay = false;
    mIsTransient = false;
    mBoundary = NULL;
    mContext = WORLD_COORDS;
    mDeleteDrawableOnDestruction = false;
}

SceneObject::SceneObject(const SceneObject &original) : BoyLib::Positionable(original)
{
    mId = original.mId;
    mDepth = original.mDepth;
    mIsVisible = original.mIsVisible;
    mScene = NULL;
    mIsOverlay = original.mIsOverlay;
    mIsTransient = original.mIsTransient;
    mContext = original.mContext;
    mDeleteDrawableOnDestruction = original.mDeleteDrawableOnDestruction;

    mDrawable = NULL;
    if (original.mDrawable != NULL)
    {
        mDrawable = original.mDrawable->clone();
    }

    mBoundary = NULL;
    if (original.mBoundary)
    {
        mBoundary = original.mBoundary->clone();
    }
}

SceneObject::~SceneObject()
{
    if (mDrawable != NULL && mDeleteDrawableOnDestruction)
    {
        delete mDrawable;
    }

    if (mBoundary != NULL)
    {
        delete mBoundary;
    }
}

void SceneObject::draw(Graphics *graphics, Camera *camera)
{
    if (mDrawable != NULL)
    {
        mDrawable->draw(graphics, camera, this, mContext == SCREEN_COORDS);
    }
}

void SceneObject::handleAdd(Scene *scene)
{
    mScene = scene;
}

void SceneObject::handleRemove()
{
    mScene = NULL;
}


bool SceneObject::contains(const BoyLib::Vector2 &vector)
{
    if (mBoundary != NULL)
    {
        return mBoundary->contains(rotate(vector - getPosition(), getAnchor()->getPosition(), -getRotation()));
    }
    return false;
}
