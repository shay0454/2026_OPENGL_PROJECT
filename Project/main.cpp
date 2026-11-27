#include<iostream>
#include"OpenGLInit.h"
#include"OpenGLShaderInit.h"
#include"Texture.h"
#include"Mesh.h"
#include"global.h"
#include"Random.h"
#include"TextureLoader.h"
#include"Light.h"
#include"LowPolyTank.h"

void resetGame() {

}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}

void inputProcess(float deltaTime, LowPolyTank& tank) {
	tank.moveDirection = {
		static_cast<float>(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - static_cast<float>(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS),
		0.f,
		static_cast<float>(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) - static_cast<float>(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	};
}

void aimTankAtMouse(LowPolyTank& tank) {
	int windowWidth, windowHeight;
	int framebufferWidth, framebufferHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	if (windowWidth == 0 || windowHeight == 0 || framebufferWidth == 0 || framebufferHeight == 0) return;

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	float x = 2.f * static_cast<float>(mouseX) / windowWidth - 1.f;
	float y = 1.f - 2.f * static_cast<float>(mouseY) / windowHeight;

	// 화면 좌표를 카메라의 가까운 면과 먼 면에 있는 월드 좌표로 변환.
	float aspect = static_cast<float>(framebufferWidth) / framebufferHeight;
	glm::mat4 inverseViewProjection = glm::inverse(curCamera->getProjection(aspect) * curCamera->getView());
	glm::vec4 nearPoint = inverseViewProjection * glm::vec4(x, y, -1.f, 1.f);
	glm::vec4 farPoint = inverseViewProjection * glm::vec4(x, y, 1.f, 1.f);
	nearPoint /= nearPoint.w;
	farPoint /= farPoint.w;

	// 마우스 방향의 직선과 바닥(y = 0)이 만나는 위치를 구한다.
	glm::vec3 direction = glm::vec3(farPoint - nearPoint);
	if (glm::abs(direction.y) < 1e-6f) return;
	float distance = -nearPoint.y / direction.y;
	if (distance < 0.f) return;
	tank.aimAt(glm::vec3(nearPoint) + direction * distance);
}

void update(float deltaTime) {

}

int main() {
	if (OPENGLinit() != 0) {
		glfwTerminate();
		return 1;
	}

	texture = textureLoader.loadImg("assets/textures/palettes/gradient.png");
	make_vertexShaders();
	make_fragmentShaders();
	shaderProgramID = make_shaderProgram();

	float mapSize = 10.f;
	Mesh* plane = pg.loadMeshlFromObj("assets/models/primitives/plane.obj");
	plane->scale(mapSize);
	plane->color = { 0.776f, 0.710f, 0.541f };
	meshes.push_back(plane);

	LowPolyTank* tank = new LowPolyTank(pg);
	tank->syncTopLightPosition();
	models.push_back(tank);
	lightManger.add(tank->getTopLight());
	

	InitBuffer();
	glfwSetWindowUserPointer(window, curCamera);
	glfwSetKeyCallback(window, callback);

	float wallHeight = 1.f;
	float cameraMargin = 0.3f;
	glm::vec3 cameraOffset(0.f, 8.f, 6.f);
	glm::vec3 viewDirection = glm::normalize(cameraOffset);
	curCamera->cameraTarget = { 0.f, wallHeight * 0.5f, 0.f };
	curCamera->cameraPosition = curCamera->cameraTarget + cameraOffset;
	curCamera->perspectiveMode = false;
	curCamera->orthoHalfHeight =
		(mapSize * viewDirection.y + wallHeight * viewDirection.z) * 0.5f + cameraMargin;
	curCamera->orthoMinHalfWidth = mapSize * 0.5f + cameraMargin;

	float curTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float deltaTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();
		inputProcess(deltaTime, *tank);
		update(deltaTime);
		tank->update(deltaTime);
		aimTankAtMouse(*tank);
		UpdateBuffer();
		drawScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lightManger.remove(tank->getTopLight());
	models.clear();
	delete tank;
	meshes.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
