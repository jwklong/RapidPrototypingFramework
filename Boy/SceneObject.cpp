#include "SceneObject.h"

#include <assert.h>
#include "BoyLib/BoyUtil.h"

using namespace Boy;

SceneObject::SceneObject(const std::string &id, float depth)
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

SceneObject::SceneObject(const SceneObject &obj) : BoyLib::Positionable(obj)
{
    mId = obj.mId;
    mDepth = obj.mDepth;
    mIsVisible = obj.mIsVisible;
    mScene = NULL;
    mIsOverlay = obj.mIsOverlay;
    mIsTransient = obj.mIsTransient;
    mContext = obj.mContext;
    mDeleteDrawableOnDestruction = obj.mDeleteDrawableOnDestruction;

    mDrawable = NULL;
    if (obj.mDrawable != NULL)
    {
        mDrawable = obj.mDrawable->clone();
    }

    mBoundary = NULL;
    if (obj.mBoundary)
    {
        mBoundary = obj.mBoundary->clone();
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

void SceneObject::draw(Graphics *g, Camera *camera)
{
    if (mDrawable != NULL)
    {
        mDrawable->draw(g, camera, this, mContext == SCREEN_COORDS);
    }
}

void SceneObject::handleAdd(Scene *scene)
{
    mScene = scene;
}

void SceneObject::handleRemove(Scene *scene)
{
    mScene = NULL;
}

bool SceneObject::contains(const BoyLib::Vector2 &coord)
{
    if (mBoundary != NULL)
    {
        return mBoundary->contains(rotate(coord - getPosition(), getAnchor()->getPosition(), -getRotation()));
    }
    return false;
}

void SceneObject::setContext(Context context)
{
    mContext = context;
}

void SceneObject::setVisible(bool visible)
{
    mIsVisible = visible;
}

void SceneObject::setTransient(bool transient)
{
    mIsTransient = transient;
}

BoyLib::Cloneable *SceneObject::clone(std::map<BoyLib::Cloneable*, BoyLib::Cloneable*> *cloneMap)
{
    SceneObject *clone = new SceneObject(*this);
    if (cloneMap!=NULL)
    {
        (*cloneMap)[this] = clone;
    }
    return clone;
}

bool SceneObject::tick()
{
    if (mDrawable != NULL)
    {
        return mDrawable->tick();
    }
    return false;
}
