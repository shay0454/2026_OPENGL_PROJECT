#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include<glm/ext.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define HALF_SCREEN_WIDTH (SCREEN_WIDTH / 2.0f)
#define HALF_SCREEN_HEIGHT (SCREEN_HEIGHT / 2.0f)

extern GLFWwindow* window;

int OPENGLinit();
void inputProcess();
void moveBox();
void set();
void DrawScene();
