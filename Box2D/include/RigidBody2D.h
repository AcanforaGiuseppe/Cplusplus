#pragma once
#include "Quad.h"
#include <box2d/box2d.h>

class RigidBody2D
{
public:
    RigidBody2D(Quad *InQuad);
    void Update();
    void SetFixture(b2FixtureDef *InDef);

private:
    Quad *Object;
    b2Body *Body;
    b2Fixture *Fixture;
};