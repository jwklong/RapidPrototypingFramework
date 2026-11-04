#pragma once

#include <map>
#include <string>
#include <vector>
#include <algorithm>
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
        Scene(std::string id, bool createSim);
        ~Scene();

        void addObject(SceneObject *object);
        void removeObject(SceneObject *object);
        void handleDepthChanged(SceneObject *obj);

        SceneObject *getObjectById(const std::string &id);
        SceneObject *getObjectAt(const BoyLib::Vector2 &screenPos);

        void addAutoBounds(float minx, float maxx, float miny, float maxy);
        bool boundsContain(const BoyLib::Vector2 &point);
        void ensureCameraInBounds(bool ignoreAutobounds);

        void tick(float time, float stepSize, bool fastStep);
        void draw(Graphics *g, float minDepth, float maxDepth, bool inclusive);
        void drawOverlays(Graphics *g);
        void drawOverlays2(Graphics *g);

        float getMinY();

        //impl of cloneable
        virtual BoyLib::Cloneable *clone(std::map<BoyLib::Cloneable*,BoyLib::Cloneable*> *cloneMap);
        virtual void remap(std::map<Cloneable*,Cloneable*> &cloneMap);

    protected:
        void bubbleSortPass();

    private:
        std::vector<SceneObject*> mObjects;
        std::map<std::string, SceneObject*> mObjectsById;

    public:
        std::string mId;
        Camera *mCamera;
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
