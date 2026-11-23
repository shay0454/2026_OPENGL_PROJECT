#include "TextureLoader.h"
#include"stb_image.h"

Texture TextureLoader::loadImg(std::string fileName) {

	Texture texture;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	if (!data) {
		texture.id = -1;
		return texture;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	//texture.TextureType = fileName.substr(fileName.find('.')+1);
	texture.TextureType = "texture_diffuse";
	return texture;
}