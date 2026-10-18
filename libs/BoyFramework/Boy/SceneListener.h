#pragma once

namespace Boy
{
    class Scene;

    class SceneListener
    {
    public:
        SceneListener() {}
        virtual ~SceneListener() {}

        virtual void addedToScene(Scene *scene);
        virtual void removedFromScene(Scene *scene);
    };
}