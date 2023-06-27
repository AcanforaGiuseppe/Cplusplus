#pragma once
#include <string>
#include <glm/glm.hpp>

typedef struct GLFWwindow GLFWwindow;

enum Key
{
    KEY_0 = 48,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_RIGHT = 262,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP
};

class Window
{
public:
    Window(int InWidth, int InHeight, const std::string &InTitle);
    ~Window();
    void SetTitle(const std::string &InTitle);
    bool IsOpened() const;
    float GetDeltaTime() const;
    void Update();
    void PrintInfo() const;
    glm::vec2 MousePosition() const;
    glm::vec2 Size() const;
    bool IsKeyPressed(Key InKey) const;
    int GetWidth() const { return Width; }
    int GetHeight() const { return Height; }
    float GetAspectRatio() const { return (float)Width / (float)Height; }

private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow *RawWindow;
    float DeltaTime;
    float LastTime;
};