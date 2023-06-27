#include "BoxCollider.h"
#include "ServiceLocator.h"
#include "Quad.h"
#include "RigidBody2D.h"

BoxCollider2D::BoxCollider2D(Quad *InQuad)
{
    Object = InQuad;

    glm::vec3 boxSize = InQuad->Scale;
    boxSize *= 0.5f;

    b2PolygonShape box;
    box.SetAsBox(boxSize.x, boxSize.y);
    b2FixtureDef colliderDef;
    colliderDef.shape = &box;
    //colliderDef.isSensor = true; //trigger

    if (!Object->RigidBody)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2BodyType::b2_staticBody;
        bodyDef.position.Set(Object->Position.x, Object->Position.y);
        Body = ServiceLocator::GetInstance().Physics->CreateBody(&bodyDef);
        Fixture = Body->CreateFixture(&colliderDef);
    }
    else
    {
        Object->RigidBody->SetFixture(&colliderDef);
    }
}