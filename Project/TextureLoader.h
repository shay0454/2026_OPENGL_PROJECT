#pragma once
#include"OpenGLInit.h"
#include"Texture.h"
#define STB_IMAGE_IMPLEMENTATION


class TextureLoader {
public:
	int width, height, nrChannels;

	Texture loadImg(std::string fileName);
};