#pragma once

#include <map>
#include <string>
#include <vector>
#include "BoyLib/Cloneable.h"
#include "PhysBoy/World.h"
#include "Graphics.h"
#include "SceneListener.h"

namespace Boy
{
    class Camera;
    class SceneObject;

    class Scene : public BoyLib::Cloneable
    {
    public:
        Scene(std::string id, bool withSimulation);
        ~Scene();

        void addObject(SceneObject *object);
        void removeObject(SceneObject *object);

        void addAutoBounds(float minX, float maxX, float minY, float maxY);
        bool boundsContain(const BoyLib::Vector2 &pos);
        void ensureCameraInBounds(bool force);

        //impl of cloneable
        virtual BoyLib::Cloneable *clone(std::map<BoyLib::Cloneable*,BoyLib::Cloneable*> *cloneMap);
        virtual void remap(std::map<Cloneable*,Cloneable*> &cloneMap);

    protected:
        void bubbleSortPass();

    public:
        std::string mId;

    private:
        Camera *mCamera;
        std::vector<SceneObject*> mObjects;
        std::map<std::string, SceneObject*> mObjectsById;

        PhysBoy::World *mSimulation;

        float mMinX;
        float mMaxX;
        float mMinY;
        float mMaxY;

        bool mAutoboundsEnabled;
        float mAutoMinX;
        float mAutoMaxX;
        float mAutoMinY;
        float mAutoMaxY;

        Color mBackgroundColor;
        SceneListener *mListener;
    };
}