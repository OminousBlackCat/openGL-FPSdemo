#pragma once

#include "stb_image.h"
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;


class Texture {
public:
	unsigned textureID;
	unsigned char* imageConetent;
	int width;
	int height;
	int nrChannels;

	Texture(const char* imageFile, const unsigned int RGBmode) {
		imageConetent = stbi_load(imageFile, &this->width, &this->height, &this->nrChannels, 0);
		glGenTextures(1, &this->textureID);
		glBindTexture(GL_TEXTURE_2D, this->textureID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, RGBmode, this->width, this->height, 0, RGBmode, GL_UNSIGNED_BYTE, this->imageConetent); // full the data into the texture object
		glGenerateMipmap(GL_TEXTURE_2D); //indiate the mipmaps,  this procecss is necessary
		stbi_image_free(this->imageConetent); //free the memory space
		glBindTexture(GL_TEXTURE_2D, 0);
	}


};