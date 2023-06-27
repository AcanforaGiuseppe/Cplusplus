#include "RigidBody2D.h"
#include "ServiceLocator.h"
#include <iostream>

RigidBody2D::RigidBody2D(Quad *InQuad)
{
    Object = InQuad;
    b2BodyDef BodyDef;
    BodyDef.type = b2BodyType::b2_dynamicBody;
    BodyDef.position.Set(Object->Position.x, Object->Position.y);
    Body = ServiceLocator::GetInstance().Physics->CreateBody(&BodyDef);

    // At least one fixture per RigidBody (Tatoo)
    b2PolygonShape box;
    box.SetAsBox(0.5f, 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 1.f;           // Kg/m2  for dynamic body must be > 0.f
    fixtureDef.filter.categoryBits = 0; // No collision detect
    fixtureDef.filter.maskBits = 0;     // No collision detect
    Fixture = Body->CreateFixture(&fixtureDef);
}

void RigidBody2D::Update()
{
    b2Vec2 b2Pos = Body->GetPosition();
    Object->Position.x = b2Pos.x;
    Object->Position.y = b2Pos.y;
}

void RigidBody2D::SetFixture(b2FixtureDef *InDef)
{
    float Dens = Fixture->GetDensity();
    //if (Fixture != nullptr)
    //{
    Body->DestroyFixture(Fixture);
    //}

    Fixture = Body->CreateFixture(InDef);
    Fixture->SetDensity(Dens);
    Body->ResetMassData();
}