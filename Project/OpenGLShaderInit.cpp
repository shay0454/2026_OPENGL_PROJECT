#pragma once
#include "OpenGLShaderInit.h"

// 위치와 색상을 한 개의 데이터로 저장
std::string filetobuf(const char* file) {
	std::ifstream shaderFile(file);

	if (!shaderFile.is_open()) {
		return "open file failed\n";
	}
	std::string source((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	return source;
}

GLvoid InitUniform() {
	lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
	lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
	objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor");
	glUniform3f(objColorLocation, 1.0, 0.5, 0.3);
	viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3f(viewPosLocation, curCamera->cameraPosition.x, curCamera->cameraPosition.y, curCamera->cameraPosition.z);
	useTextureLocation = glGetUniformLocation(shaderProgramID, "useTexture");
	modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
}

GLvoid InitBuffer() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 은면 효과 활성화
	glEnable(GL_DEPTH_TEST);
	InitUniform();
}

GLvoid UpdateBuffer() {
	for (Mesh* mesh : meshes) {
		mesh->updateBuffer();
	}
	for (Model* model : models) {
		model->updateBuffer();
	}
}

void drawScene() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	if (width <= 0 || height <= 0) return;

	glViewport(0, 0, width, height);
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.f);
	// GL_COLOR_BUFFER_BIT : 이전 색 초기화
	// GL_DEPTH_BUFFER_BIT : 깊이 버퍼 초기화
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	lightManger.uploadLights();

	view = curCamera->getView();
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniform3fv(viewPosLocation, 1,glm::value_ptr(curCamera->cameraPosition));
	const float aspect = static_cast<float>(width) / height;
	glm::mat4 projection = curCamera->getProjection(aspect);

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
	
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);

	for (Mesh* mesh : meshes) {
		mesh->draw();
	}

	for (Model* m : models) {
		m->draw();
	}

	glBindVertexArray(0);
}

void make_vertexShaders() {
	std::string vertexSource = filetobuf("vertex.glsl");
	const char* source = vertexSource.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errlog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	// 셰이더 컴파일 실패
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errlog);
		std::cerr << "err:vertex shader compile failed\n" << errlog << '\n';
		return;
	}
}

GLuint make_shaderProgram() {
	GLint result;
	GLchar errlog[512];

	GLuint shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errlog);
		std::cerr << "err:shader program connect filed\n" << errlog << '\n';
		return 0;
	}
	glUseProgram(shaderID);							// shader program 사용
	return shaderID;
}

void make_fragmentShaders() {
	std::string fragmentSource = filetobuf("fragment.glsl");
	const char* source = fragmentSource.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &source, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errlog[512];

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	// 파면 컴파일 실패
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errlog);
		std::cerr << "err:fragment shader compile failed\n" << errlog << '\n';
		return;
	}
}
