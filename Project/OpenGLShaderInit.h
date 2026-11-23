#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include"OpenGLInit.h"
#include"Vertex.h"
#include"Model.h"
#include"global.h"
// 내부 구조 변경 (vertices는 origin 기준 로컬 좌표)
void getAllVerteicesAndIndices(std::vector<Vertex>& allVertices, std::vector<GLuint>& allIndices);

std::string filetobuf(const char* file);

extern GLuint VAO, VBO, EBO;

GLvoid InitBuffer();

GLvoid UpdateBuffer();

GLuint make_shaderProgram();

void drawScene();

void make_vertexShaders();

void make_fragmentShaders();;