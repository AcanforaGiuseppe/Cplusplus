#include "PlatformScene.h"
#include "OGLQuadRenderer.h"
#include "RigidBody2D.h"
#include "BoxCollider.h"

void PlatformScene::Start()
{
    Camera = new OrthoCamera(Win.GetWidth(), Win.GetHeight(), 10);

    Quad *Floor = new Quad(Camera->GetOrthoWidth(), 1);
    Floor->Position = glm::vec3{0, -Camera->GetOrthoHeight() * 0.5f + Floor->Scale.y * 0.5f, 0};
    Floor->BoxCollider = new BoxCollider2D(Floor);

    Quad *Player = new Quad(1, 1);
    Player->Position = glm::vec3{0, 5, 0};
    Player->Color = Color_Red;
    Player->RigidBody = new RigidBody2D(Player);
    Player->BoxCollider = new BoxCollider2D(Player);

    Quads.push_back(Floor);
    Quads.push_back(Player);
}

void PlatformScene::Update()
{
    Renderer.Draw(Quads, Camera);
}

void PlatformScene::FixedUpdate()
{
    for (auto *Each : Quads)
        if (Each->RigidBody)
            Each->RigidBody->Update();
}

void PlatformScene::Destroy()
{
    delete Camera;
    
    for (auto *Each : Quads)
        delete Each;
}