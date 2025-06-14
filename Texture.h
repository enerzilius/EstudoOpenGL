#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
	GLuint ID;
	Texture();

	void LinkTexJPG(int width, int height, unsigned char* data);
	void LinkTexPNG(int width, int height, unsigned char* data);
	void ActiveTexture(GLuint unit);
	void SetTexParameters();
	void Bind();
	void Unbind();
	void Delete();
};

#endif
