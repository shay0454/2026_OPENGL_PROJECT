#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include"OpenGLInit.h"
#include"Vertex.h"
#include "Model.h"
class ParsingObj {
private:
	std::ifstream fs;
	std::vector<std::string> lines;
	std::vector<Position> positions;
	std::vector<UV> textures;
	std::vector<Normal> normals;
	std::vector<Face> faces;

public:
	void clear();

	// 정점 input
	void inputVertex(std::stringstream& ss);

	// UV 위치값 input
	void inputUV(std::stringstream& ss);

	// 법선 input
	void inputNormal(std::stringstream& ss);

	// 면(face) input
	void inputFace(std::stringstream& ss, int lineNum);

	Mesh* loadMeshlFromObj(std::string fileName);

	void splitNum(std::string temp, std::string& vertex, std::string& uv, std::string& normal) {
		int slash1 = temp.find('/');
		int slash2 = temp.find('/', slash1 + 1);
		vertex = temp.substr(0, slash1);
		uv = temp.substr(slash1 + 1, slash2 - slash1 - 1);
		normal = temp.substr(slash2 + 1);
	}

};
