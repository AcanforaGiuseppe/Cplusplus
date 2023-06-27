#include "Physics2D.h"

Physics2D::Physics2D(glm::vec2 InGravity, float InFixedDeltaTime)
{
    Gravity = InGravity;
    FixedDeltaTime = InFixedDeltaTime;
    b2Vec2 b2Grav{Gravity.x, Gravity.y};
    World = new b2World(b2Grav);
}

void Physics2D::Step()
{
    World->Step(FixedDeltaTime, 8, 3);
}

b2Body *Physics2D::CreateBody(b2BodyDef *InDef)
{
    return World->CreateBody(InDef);
}