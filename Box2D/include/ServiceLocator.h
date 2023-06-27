#pragma once
#include "Physics2D.h"

class ServiceLocator
{
public:
    static ServiceLocator &GetInstance()
    {
        static ServiceLocator Instance{};
        return Instance;
    }

    Physics2D *Physics;
};