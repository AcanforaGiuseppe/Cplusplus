#pragma once
#include <glm/glm.hpp>
#include <box2d/box2d.h>

class Physics2D
{
public:
    Physics2D(glm::vec2 InGravity, float InFixedDeltaTime);
    void Step();
    b2Body *CreateBody(b2BodyDef *InDef);

private:
    b2World *World;
    float FixedDeltaTime;
    glm::vec2 Gravity;
};