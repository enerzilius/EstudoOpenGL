#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>
#include <string>

class Texture {
public:
	GLuint ID;
	std::string type;
	Texture();

	void LinkTexJPG(int width, int height, unsigned char* data);
	void LinkTexPNG(int width, int height, unsigned char* data);
	void LinkTexBuffer(int width, int height);
	void ActiveTexture(GLuint unit);
	void SetTexParameters();
	void Bind();
	void Unbind();
	void Delete();
};

#endif
