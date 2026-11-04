#pragma once

#include "BoyLib/Tickable.h"
#include "Graphics.h"

namespace Boy
{
    class Camera;
    class SceneObject;

	class Drawable : public BoyLib::Tickable
	{
	public:

		Drawable() {}
        virtual ~Drawable() {}

        virtual void draw(Graphics *g, Camera *camera, SceneObject *anchor, bool inScreenSpace) = 0;
        virtual Drawable *clone() = 0;
	};
}
