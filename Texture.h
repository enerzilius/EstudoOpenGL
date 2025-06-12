#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

class Texture {
public:
	GLuint ID;
	Texture();

	void LinkTex(const char* path);
	void ActiveTexture();
	void SetTexParameters();
	void Bind();
	void Unbind();
	void Delete();
};

#endif
