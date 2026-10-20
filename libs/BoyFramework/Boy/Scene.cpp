#include "Scene.h"

#include <assert.h>
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
    if (vectorIt != mObjects.end())
    {
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

void Scene::handleDepthChanged(SceneObject* object)
{
    auto it = std::find(mObjects.begin(), mObjects.end(), object);
    if (it != mObjects.end()) {
        mObjects.erase(it);
    }
    mObjects.push_back(object);
    bubbleSortPass();
}

SceneObject *Scene::getObjectById(std::string id)
{
    assert(mObjectsById.find(id)!=mObjectsById.end());
    return mObjectsById[id];
}

SceneObject *Scene::getObjectAt(const BoyLib::Vector2 &point)
{
    SceneObject* bestMatch = nullptr;

    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject* object = mObjects[i];

        if (!object->isVisible())
        {
            continue;
        }

        BoyLib::Vector2 targetPos = point;
        if (object->mContext == SceneObject::WORLD_COORDS) {
            targetPos = mCamera->screenToWorld(point, object->mDepth);
        }

        if (object->contains(targetPos)) {
            bool shouldReplace = false;
            if (!bestMatch) {
                shouldReplace = true;
            } else if (!bestMatch->mIsOverlay && object->mIsOverlay) {
                shouldReplace = true;
            } else if (bestMatch->mIsOverlay == object->mIsOverlay) {
                if (bestMatch->mDepth < object->mDepth) {
                    shouldReplace = true;
                }
            }

            if (shouldReplace) {
                bestMatch = object;
            }
        }
    }

    return bestMatch;
}

void Scene::addAutoBounds(float minX, float maxX, float minY, float maxY)
{
    if (minX < mAutoMinX) mAutoMinX = minX;
    if (maxX > mAutoMaxX) mAutoMaxX = maxX;
    if (minY < mAutoMinY) mAutoMinY = minY;
    if (maxY > mAutoMaxY) mAutoMaxY = maxY;
}

bool Scene::boundsContain(const BoyLib::Vector2 &pos)
{
    float minX = mMinX;
    float maxX = mMaxX;
    float minY = mMinY;
    float maxY = mMaxY;

    if (mAutoboundsEnabled)
    {
        if (mAutoMinX < mAutoMaxX)
        {
            if (minX < mAutoMinX) minX = mAutoMinX;
            if (maxX > mAutoMaxX) maxX = mAutoMaxX;
        }
        if (mAutoMinY < mAutoMaxY)
        {
            if (minY < mAutoMinY) minY = mAutoMinY;
            if (maxY > mAutoMaxY) maxY = mAutoMaxY;
        }
    }

    return (pos.x >= minX && pos.x <= maxX && pos.y >= minY && pos.y <= maxY);
}

void Scene::ensureCameraInBounds(bool force)
{
    if (Environment::instance()->isDebugEnabled())
    {
        return;
    }

    float minX = mMinX;
    float maxX = mMaxX;
    float minY = mMinY;
    float maxY = mMaxY;

    if (mAutoboundsEnabled && !force)
    {
        if (mAutoMinX < mAutoMaxX)
        {
            if (minX < mAutoMinX) minX = mAutoMinX;
            if (maxX > mAutoMaxX) maxX = mAutoMaxX;
        }
        if (mAutoMinY < mAutoMaxY)
        {
            if (minY < mAutoMinY) minY = mAutoMinY;
            if (maxY > mAutoMaxY) maxY = mAutoMaxY;
        }
    }

    Graphics *graphics = Environment::instance()->getGraphics();

    // horizontal clamping
    float halfWidth = (float)graphics->getWidth() / mCamera->getZoom() * 0.5f;
    BoyLib::Vector2 camPos = mCamera->getPosition();
    bool fitsRight = (camPos.x + halfWidth <= maxX);
    if (camPos.x - halfWidth < minX) {
        if (fitsRight) {
            mCamera->setPosition(minX + halfWidth, camPos.y);
            mCamera->mVel = BoyLib::Vector2(0.0f, mCamera->mVel.y);
        }
    } else if (!fitsRight) {
        mCamera->setPosition(maxX - halfWidth, camPos.y);
        mCamera->mVel = BoyLib::Vector2(0.0f, mCamera->mVel.y);
    }

    // vertical clamping
    float halfHeight = (float)graphics->getHeight() / mCamera->getZoom() * 0.5f;
    camPos = mCamera->getPosition();
    float camY = camPos.y - mCamera->mOffset.y;
    bool fitsBottom = (camY + halfHeight <= maxY);
    if (camY - halfHeight < minY) {
        if (fitsBottom) {
            mCamera->setPosition(camPos.x, minY + halfHeight + mCamera->mOffset.y);
            mCamera->mVel = BoyLib::Vector2(mCamera->mVel.x, 0.0f);
        }
    } else if (!fitsBottom) {
        mCamera->setPosition(camPos.x, maxY - halfHeight + mCamera->mOffset.y);
        mCamera->mVel = BoyLib::Vector2(mCamera->mVel.x, 0.0f);
    }

    // zoom aaah
    BoyLib::Vector2 worldMin = mCamera->screenToWorld(BoyLib::Vector2(0.0f, 0.0f), 1.0f);
    BoyLib::Vector2 worldMax = mCamera->screenToWorld(BoyLib::Vector2(graphics->getWidth(), graphics->getHeight()), 1.0f);
    float baseZoom = mCamera->mBaseZoom;
    float currentZoom = mCamera->getZoom();
    float zoomRatio = currentZoom / baseZoom;
    float targetZoom = zoomRatio;

    if ((maxX - minX) < (worldMax.x - worldMin.x)) {
        targetZoom = ((worldMax.x - worldMin.x) / (maxX - minX)) * zoomRatio;
        mCamera->setZoom(targetZoom);
    }
    if ((maxY - minY) < (worldMax.y - worldMin.y)) {
        float heightZoom = ((worldMax.y - worldMin.y) / (maxY - minY)) * zoomRatio;
        if (targetZoom < heightZoom) {
            targetZoom = heightZoom;
            mCamera->setZoom(targetZoom);
        }
    }

    if (!force) {
        ensureCameraInBounds(true);
    }
}

void Scene::tick(float dt)
{
    if (dt != 0.0f)
    {
        if (mSimulation)
        {
            // mSimulation->step(dt);
        }

        for (int i = mObjects.size() - 1; i >= 0; i--) // go backwards
        {
            SceneObject* obj = mObjects[i];

            if (obj->tick()) {
                removeObject(obj);

                delete obj;
            }
        }
    }
}

void Scene::draw(Graphics *graphics, float minDepth, float maxDepth, bool inclusive)
{
    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject *object = mObjects[i];

        if (object->mIsOverlay)
        {
            continue;
        }

        if (maxDepth < object->mDepth) // exit out loop since depth will only increase from here
        {
            return;
        }

        if (minDepth <= object->mDepth)
        {
            if (inclusive || (object->mDepth != minDepth && object->mDepth != maxDepth))
            {
                if (object->isVisible())
                {
                    object->draw(graphics, mCamera);
                }
            }
        }
    }
}

void Scene::drawOverlays(Graphics *graphics)
{
    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject *object = mObjects[i];

        if (object->mIsOverlay)
        {
            object->draw(graphics, mCamera);
        }
    }
}

void Scene::drawOverlays2(Graphics *graphics)
{
    for (int i = 0; i < mObjects.size(); i++)
    {
        SceneObject *object = mObjects[i];

        if (object->mIsOverlay2)
        {
            object->draw(graphics, mCamera);
        }
    }
}

float Scene::getMinY()
{
    if (mAutoboundsEnabled && mAutoMinY < mAutoMaxY && mAutoMinY > mMinY)
    {
        return mAutoMinY;
    }
    return mMinY;
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

    for (int i = size - 2; i >= 0; i--) {
        if (addedObject->mDepth < mObjects[i]->mDepth) {
            mObjects[i + 1] = mObjects[i];
            mObjects[i] = addedObject;
        }
    }
}
