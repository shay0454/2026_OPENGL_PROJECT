#include"OpenGLInit.h"

extern GLFWwindow* window = nullptr;


int OPENGLinit() {
	// GLFW init
	if (!glfwInit()) {
		std::cerr << "GLFW init failed\n";
		return -1;
	}

	// OPENGL version setting (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// create window
	window = glfwCreateWindow(1120, 750, "OpenGL window", NULL, NULL);
	if (!window) {
		std::cerr << "window init failed\n";
		return -1;
	}

	// CONTEXT SETTING
	glfwMakeContextCurrent(window);

	// GLEW init
	glewExperimental = GL_TRUE; // 최신 기능 사용
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW init failed\n";
		return -1;
	}

	return 0;
}