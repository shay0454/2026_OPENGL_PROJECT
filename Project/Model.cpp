#include "Model.h"
#include "global.h"

void Model::draw() {
	// 현재 modelTransform uniform 위치값 가져오기
	modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniform1i(useTextureLocation, GL_TRUE);

	// 상속용
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (Mesh* mesh : meshes) {
		mesh->draw(transform*glm::translate(glm::mat4(1.f),-origin));
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Model::loadModel(std::string sourceFileName) {
	ParsingObj pg;
	pg.loadMeshlFromObj(sourceFileName);
}

void Model::updateBuffer() {
	for (Mesh* mesh : meshes) {
		mesh->updateBuffer();
	}
}

void Robot::draw() {
	modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");

	// 상속용
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	drawNode(root, transform * glm::translate(glm::mat4(1.f), -origin));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}