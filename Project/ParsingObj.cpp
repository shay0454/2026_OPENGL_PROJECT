#include"ParsingObj.h"

void ParsingObj::clear() {
	lines.clear();
	positions.clear();
	textures.clear();
	normals.clear();
	faces.clear();
}

void ParsingObj::inputVertex(std::stringstream& ss) {
	std::string temp[3];
	for (int i = 0; i < 3; i++) {
		ss >> temp[i];
	}
	glm::vec3 v = { std::stod(temp[0]),std::stod(temp[1]) ,std::stod(temp[2]) };
	positions.push_back(v);
}

void ParsingObj::inputUV(std::stringstream& ss) {
	std::string temp[2];
	for (int i = 0; i < 2; i++) {
		ss >> temp[i];
	}
	glm::vec2 v = { std::stod(temp[0]),std::stod(temp[1]) };
	textures.push_back(v);
}

void ParsingObj::inputNormal(std::stringstream& ss) {
	std::string temp[3];
	for (int i = 0; i < 3; i++) {
		ss >> temp[i];
	}
	glm::vec3 v = { std::stod(temp[0]),std::stod(temp[1]) ,std::stod(temp[2]) };
	normals.push_back(v);
}

void ParsingObj::inputFace(std::stringstream& ss, int lineNum) {
	std::string temp;
	std::string vertexStr;
	std::string uvStr;
	std::string normalStr;
	Face f;
	for (int i = 0; i < 3; i++) {
		ss >> temp;

		// 각 /로 분리 후, 각 벡터에 저장
		splitNum(temp, vertexStr, uvStr, normalStr);

		f.vertexIdx[i] = std::stoi(vertexStr);

		f.uv[i] = (!uvStr.empty() ? std::stoi(uvStr) : 0);

		f.normalIdx[i] = (!normalStr.empty() ? std::stoi(normalStr) : 0);
	}
	faces.push_back(f);
}

Mesh* ParsingObj::loadMeshlFromObj(std::string fileName) {
	std::string temp;
	clear();
	fs.open(fileName);
	if (!fs.is_open()) return NULL;
	while (std::getline(fs, temp)) {
		lines.push_back(temp);
	}
	fs.close();

	for (int i = 0; i < lines.size(); i++) {
		std::string command;
		std::stringstream ss(lines[i]);
		ss >> command;
		// 주석 처리 넘김
		if (command.empty())continue;
		if (command[0] == '#')continue;
		if (command == "v") {
			inputVertex(ss);
		}
		else if (command == "vt") {
			inputUV(ss);
		}
		else if (command == "vn") {
			inputNormal(ss);
		}
		else if (command == "f") {
			inputFace(ss, i + 1);
		}
		else {
			printf("미존재 형식\n");
		}
	}

	std::vector<Vertex> vertices;
	std::vector<Arr3> indices;

	for (const Face& face : faces) {
		Arr3 indice = {};
		for (int i = 0; i < 3; ++i) {
			Vertex vertex;
			vertex.position = positions.at(face.vertexIdx[i] - 1);
			vertex.Normal = glm::vec3(0.f);
			vertex.TexCoords = glm::vec2(0.f);

			if (face.normalIdx[i] != 0) {
				vertex.Normal = normals.at(face.normalIdx[i] - 1);
			}

			if (face.uv[i] != 0) {
				vertex.TexCoords = textures.at(face.uv[i] - 1);
			}
			indice[i] = static_cast<GLuint>(vertices.size());
			vertices.push_back(vertex);
		}
		indices.push_back(indice);
	}

	// 현재 Mesh는 빈 벡터를 안전하게 처리하지 못하므로 방지
	if (vertices.empty()) {
		return nullptr;
	}

	std::vector<Texture> meshTextures;

	return new Mesh(vertices, indices, meshTextures);
}