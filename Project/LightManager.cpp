#include"LightManager.h"
#include"global.h"
void LightManager::uploadLights() {
	GLint lightCountLocation = glGetUniformLocation(shaderProgramID, "lightCount");
	glUniform1i(lightCountLocation, lights.size());

	for (int i = 0; i < lights.size(); i++) {
		std::string name = "lights[" + std::to_string(i) + "]";
		glUniform3fv(glGetUniformLocation(shaderProgramID, (name + ".position").c_str()), 1, glm::value_ptr(lights[i]->position));
		glUniform3fv(glGetUniformLocation(shaderProgramID, (name + ".color").c_str()), 1, glm::value_ptr(lights[i]->color));
		glUniform1f(glGetUniformLocation(shaderProgramID, (name + ".intensity").c_str()), lights[i]->intensity);

	}
}