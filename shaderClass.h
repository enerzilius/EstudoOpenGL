#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>	
#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <string>
#include <iostream>

using namespace std;
string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
	void setBool(const string& name, bool value);
	void setInt(const string& name, int value);
	void setFloat(const string& name, float value);
};

#endif 