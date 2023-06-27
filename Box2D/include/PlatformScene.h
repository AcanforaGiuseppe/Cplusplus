#pragma once
#include <vector>
#include "Quad.h"
#include "Window.h"
#include "OGLQuadRenderer.h"

class PlatformScene
{
public:
    PlatformScene(const Window &InWin) : Win(InWin) {}
    void Start();
    void Update();
    void FixedUpdate();
    void Destroy();

private:
    std::vector<Quad *> Quads;
    const Window &Win;
    OGLQuadRender Renderer;
    OrthoCamera *Camera;
};