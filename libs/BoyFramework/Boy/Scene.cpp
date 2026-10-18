#include "Scene.h"

#include <float.h>
#include "Camera.h"
#include "Environment.h"
#include "SceneObject.h"

using namespace Boy;

Scene::Scene(std::string id, bool withSimulation)
{
    mId = id;

    mCamera = new Camera("");

    mMinX = -FLT_MAX;
    mMaxX = FLT_MAX;
    mMinY = -FLT_MAX;
    mMaxY = FLT_MAX;

    mAutoboundsEnabled = false;
    mAutoMinX = -1e8f;
    mAutoMaxX = 1e8f;
    mAutoMinY = -1e8f;
    mAutoMaxY = 1e8f;

    mBackgroundColor = 0x00000000;
    mListener = NULL;

    if (withSimulation)
    {
        mSimulation = new PhysBoy::World();
    } else
    {
        mSimulation = NULL;
    }
}

Scene::~Scene()
{
    for (int i = 0; i < mObjects.size(); i++)
    {
        delete mObjects[i];
    }
    mObjects.clear();
    mObjectsById.clear();

    if (mSimulation != NULL)
    {
        delete mSimulation;
    }

    if (mCamera != NULL)
    {
        delete mCamera;
    }
}

void Scene::addObject(SceneObject *object)
{
    if (!object->mId.empty())
    {
        auto it = mObjectsById.find(object->mId);
        if (it != mObjectsById.end())
        {
            envDebugLog("warning: duplicate ids in scene file (%s)\n", object->mId.c_str());
        }
        mObjectsById[object->mId] = object;
    }

    mObjects.push_back(object);
    bubbleSortPass();

    object->handleAdd(this);
    if (mListener != NULL)
    {
        mListener->addedToScene(this);
    }
}

void Scene::removeObject(SceneObject *object)
{
    auto vectorIt = std::find(mObjects.begin(), mObjects.end(), object);
    if (vectorIt != mObjects.end()) {
        mObjects.erase(vectorIt);
    }

    if (!object->mId.empty())
    {
        auto mapIt = mObjectsById.find(object->mId);
        if (mapIt != mObjectsById.end())
        {
            mObjectsById.erase(mapIt);
        }
    }

    object->handleRemove(this);
    if (mListener != NULL)
    {
        mListener->removedFromScene(this);
    }
}

BoyLib::Cloneable *Scene::clone(std::map<BoyLib::Cloneable*, BoyLib::Cloneable*> *cloneMap)
{
    Scene* clone = new Scene(mId, false);

    clone->mMinX = mMinX;
    clone->mMaxX = mMaxX;
    clone->mMinY = mMinY;
    clone->mMaxY = mMaxY;

    clone->mAutoboundsEnabled = mAutoboundsEnabled;
    clone->mAutoMinX = mAutoMinX;
    clone->mAutoMaxX = mAutoMaxX;
    clone->mAutoMinY = mAutoMinY;
    clone->mAutoMaxY = mAutoMaxY;

    clone->mBackgroundColor = mBackgroundColor;

    if (cloneMap != NULL)
    {
        (*cloneMap)[this] = clone;
    }

    if (mCamera != NULL)
    {
        clone->mCamera = (Camera*)mCamera->clone(cloneMap);
    }

    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject *object = mObjects[i];
        if (object != NULL && !object->isTransient())
        {
            clone->addObject((SceneObject*)object->clone(cloneMap));
        }
    }

    if (mSimulation != NULL)
    {
        clone->mSimulation = (PhysBoy::World*)mSimulation->clone(cloneMap);
    }

    return clone;
}

void Scene::remap(std::map<Cloneable*, Cloneable*> &cloneMap)
{
    if (mCamera != NULL)
    {
        mCamera->remap(cloneMap);
    }

    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject *object = mObjects[i];
        if (object != NULL && !object->isTransient())
        {
            object->remap(cloneMap);
        }
    }

    if (mSimulation != NULL)
    {
        mSimulation->remap(cloneMap);
    }
}

void Scene::bubbleSortPass()
{
    if (mObjects.empty())
    {
        return;
    }

    SceneObject* addedObject = mObjects.back();
    int size = mObjects.size();

    for (int i = size - 2; i >= 0; --i) {
        if (addedObject->mDepth < mObjects[i]->mDepth) {
            mObjects[i + 1] = mObjects[i];
            mObjects[i] = addedObject;
        }
    }
}
