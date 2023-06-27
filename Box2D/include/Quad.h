#pragma once
#include <glm/glm.hpp>
#include "Common.h"

class RigidBody2D;
class BoxCollider2D;

class Quad
{
public:
  Quad(int w, int h) : Position(glm::vec3(0)), Rotation(glm::vec3(0)), Scale(glm::vec3(w, h, 0)), Color(Color_White), RigidBody(nullptr), BoxCollider(nullptr){};
  glm::vec3 Position;
  glm::vec3 Rotation;
  glm::vec3 Scale;
  Color Color;
  RigidBody2D *RigidBody;
  BoxCollider2D *BoxCollider;
};