#pragma once
#include"OpenGLInit.h"
#include<vector>
#include"Model.h"
#include"Camera.h"
#include "LightManager.h"
#include"ParsingObj.h"
#include"TextureLoader.h"

// 기본 셋팅
extern GLuint shaderProgramID ;
extern GLuint vertexShader ;
extern GLuint fragmentShader ;
extern GLuint VAO , VBO , EBO ;
extern GLuint modelTextureID ;

// 배경 값
extern glm::vec3 backgroundColor;

extern bool curlingMode;

// shader 접근 변수
extern GLint modelLocation ;
extern GLint textureLocation ;
extern GLint uvLocation ;
extern GLint useTextureLocation ;

extern GLint lightPosLocation;
extern GLint lightColorLocation;
extern GLint objColorLocation;
extern GLint viewPosLocation;

extern GLboolean isPerspective;

// 카메라 변수
extern std::vector<CameraManager*> cameras;
extern CameraManager* curCamera;
extern glm::mat4 view;

// 빛 변수
extern LightManager lightManger;

// 축 변수
extern GLuint axisVAO;
extern GLuint axisVBO;

// 매쉬와 모델
extern std::vector<class Mesh*> meshes;
extern std::vector<class Model*> models;

// 텍스처 로더
extern ParsingObj pg;
extern TextureLoader textureLoader;

extern Texture texture;