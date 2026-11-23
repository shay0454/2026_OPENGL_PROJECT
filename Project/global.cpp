#include"OpenGLInit.h"
#include"global.h"

// 기본 셋팅
GLuint shaderProgramID = 0;
GLuint vertexShader = 0;
GLuint fragmentShader = 0;
GLuint VAO = 0, VBO = 0, EBO = 0;
GLuint modelTextureID = 0;

// 배경 값
glm::vec3 backgroundColor(0.53f, 0.81f, 0.92f);

bool curlingMode = false;

// shader 접근 변수
GLint modelLocation = 0;
GLint textureLocation = 0;
GLint uvLocation = 0;
GLint useTextureLocation = 0;
GLint lightColorLocation = 0;
GLint objColorLocation = 0;
GLint lightPosLocation = 0;
GLint viewPosLocation = 0;
GLboolean isPerspective = false;

// 카메라 변수
std::vector<CameraManager*> cameras = { new CameraManager() };
CameraManager* curCamera = cameras[0];
glm::mat4 view = glm::mat4(1.f);

// 빛 변수
LightManager lightManger = LightManager();


// 매쉬와 모델
std::vector<Mesh*> meshes;
std::vector<Model*> models;

// 축 변수
GLuint axisVAO = 0;
GLuint axisVBO = 0;

// 텍스처 로더
ParsingObj pg;
TextureLoader textureLoader;

Texture texture;