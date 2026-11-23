#include"Mesh.h"
#include"OpenGLShaderInit.h"
#include<string>

// 처음 매쉬 셋팅용
void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (!vertices.empty()) {
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    if (!indices.empty()) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Arr3),
            &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// 매쉬의 기본 값(정점, 노멀, face 등) 업데이트 
void Mesh::updateBuffer() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Arr3), indices.data(), GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

glm::mat4 Mesh::getWorldTransform(const glm::mat4& baseTransform) {
    glm::mat4 parentWolrd = (parentMesh?parentMesh->getWorldTransform(baseTransform): baseTransform);

    return parentWolrd * transform * glm::translate(glm::mat4(1.f), -origin);
}

// draw(매쉬의 transform 업데이트 및 draw)
void Mesh::draw(glm::mat4 parentTransform) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    // 은면 효과 활성화
    glEnable(GL_DEPTH_TEST);

    glm::mat4 finalTransform = getWorldTransform(parentTransform);

    // glsl 파일에 transform 지정
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(finalTransform));

    // 텍스처 사용 유무 설정
    glUniform1i(useTextureLocation, !textures.empty() ? GL_TRUE : GL_FALSE);

    // 셰이더 id로 faceColor uniform 위치 번호 죄희
    GLint colorLocation = glGetUniformLocation(shaderProgramID, "faceColor");

    // 메시의 color & opacity 값을 GLSL의 faceColor uniform에 전달
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));
    glUniform1f(glGetUniformLocation(shaderProgramID, "opacity"), opacity);

    for (GLuint i = 0; i<textures.size(); i++) {
        // 활성화 할 텍스처 슬롯 설정
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].TextureType;
        //if (name == "texture_diffuse")
        number = std::to_string(diffuseNr++);

        // 현재 반사 강도 미사용
        //else if (name == "texture_specular")
        //    number = std::to_string(specularNr++);

        std::string uniformName = "material." + name + number;
        GLint textureLocation = glGetUniformLocation(shaderProgramID, uniformName.c_str());

        // 셰이더의 sampler uniform이 i번 텍스처 슬롯을 사용하도록 설정
        glUniform1i(textureLocation, static_cast<GLint>(i));

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, 0);

    // 지정 배열 리셋
    glBindVertexArray(0);
}
