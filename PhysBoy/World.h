#pragma once

#include "BoyLib/Cloneable.h"

namespace PhysBoy
{
    class World : public BoyLib::Cloneable
    {
    public:
        // stub so i can make Boy::Scene
        World() {};
        ~World() {};
        virtual Cloneable *clone(std::map<Cloneable*,Cloneable*> *cloneMap);
        virtual void remap(std::map<Cloneable*,Cloneable*> &cloneMap);
    };
}