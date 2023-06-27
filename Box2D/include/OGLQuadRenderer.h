#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Quad.h"

class OrthoCamera
{
public:
    OrthoCamera(int InResoWidth, int InResoHeight, float InOrthoSize)
    {
        Width = InResoWidth;
        Height = InResoHeight;

        auto Pos = glm::vec3(0);
        auto Dir = glm::vec3(0.f, 0.f, -1.f);
        auto Up = glm::vec3(0.f, 1.f, 0.f);
        View = glm::lookAt(Pos, Pos + Dir, Up);

        float aspect = (float)Width / (float)Height;
        OrthoWidth = aspect * InOrthoSize;
        OrthoHeight = InOrthoSize;

        //(0, 0) in the middle of the screen
        float halfW = (float)OrthoWidth * 0.5f;
        float halfH = (float)OrthoHeight * 0.5f;

        float left = -halfW;
        float right = halfW;
        float bottom = -halfH;
        float top = halfH;
        Proj = glm::ortho(left, right, bottom, top, 0.f, 100.f);
        // Proj = glm::perspective(glm::radians(60.f), aspect, 0.f, 100.f);

        ViewProj = Proj * View;
    }

    glm::mat4 GetViewProjMat() const
    {
        return ViewProj;
    }

    int GetResoultionWidth() const { return Width; }
    int GetResolutionHeight() const { return Height; }

    float GetOrthoWidth() const { return OrthoWidth; }
    float GetOrthoHeight() const { return OrthoHeight; }

private:
    glm::mat4 View;
    glm::mat4 Proj;
    glm::mat4 ViewProj;
    int Width;
    int Height;
    float OrthoWidth;
    float OrthoHeight;
};

class OGLQuadRender
{
public:
    OGLQuadRender()
    {
        Program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");

        std::vector<float> Vertices =
            {
                0.5f, -0.5f, 0.0f,  // Bottom right
                -0.5f, -0.5f, 0.0f, // Bottom left
                -0.5f, 0.5f, 0.0f,  // Top left

                -0.5f, 0.5f, 0.0f, // Top left
                0.5f, 0.5f, 0.0f,  // Top right
                0.5f, -0.5f, 0.0f  // Bottom right
            };

        // 1. Create VAO
        glGenVertexArrays(1, &Vao);
        glBindVertexArray(Vao);

        // 2. Create VBO to load data
        glGenBuffers(1, &Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, Vbo);

        int DataSize = Vertices.size() * sizeof(float);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

        // 3. Link to Vertex Shader
        GLuint Location_0 = 0;
        glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(Location_0);

        glClearColor(0, 0, 0, 0);
    }

    void Draw(const std::vector<Quad *> &Quads, const OrthoCamera *Cam)
    {
        glViewport(0, 0, Cam->GetResoultionWidth(), Cam->GetResolutionHeight());
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(Vao);
        Program->Bind();
        auto VP = Cam->GetViewProjMat();

        for (size_t i = 0; i < Quads.size(); i++)
        {
            auto *Each = Quads[i];

            glm::mat4 Model = glm::mat4(1.f);
            Model = glm::translate(Model, Each->Position);
            // Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0.f, 1.f, 0.f));
            Model = glm::scale(Model, Each->Scale);

            glm::mat4 Mvp = VP * Model;

            Program->SetUniform("mvp", Mvp);
            Program->SetUniform("color", Each->Color);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

private:
    OGLProgram *Program;
    GLuint Vao;
    GLuint Vbo;
};