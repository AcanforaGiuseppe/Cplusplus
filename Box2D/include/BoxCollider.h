#pragma once
#include "Quad.h"
#include <box2d/box2d.h>

class BoxCollider2D
{
public:
    BoxCollider2D(Quad *InQuad);

private:
    Quad *Object;
    b2Body *Body;
    b2Fixture *Fixture;
};