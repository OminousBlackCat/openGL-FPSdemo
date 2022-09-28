#include<string>
#include<glad/glad.h>
#include<fstream>
#include<sstream>
#include<iostream>
#include "shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	// I.read code from files
	string vertexCode;
	string fragmentCode;
	ifstream vertexCodeFile;
	ifstream fragmentCodeFile;

	vertexCodeFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentCodeFile.exceptions(ifstream::failbit | ifstream::badbit);

	try{
		// open file
		vertexCodeFile.open(vertexShaderPath);
		fragmentCodeFile.open(fragmentShaderPath);
		stringstream vertexStream, fragmentStream;
		// read
		vertexStream << vertexCodeFile.rdbuf();
		fragmentStream << fragmentCodeFile.rdbuf();
		// close file
		vertexCodeFile.close();
		fragmentCodeFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (ifstream::failure e){
		cout << "failed to read or open code file!" << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();


	// II.then compile the shader
	unsigned int vertex, fragment;
	int success; // success flag
	char infoLog[512]; // error info

	// compile the vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL); // parameter 3 is a pointer of a pointer(arrary) because the parameter 4 hint that there could be a array of a char* passed in
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "failed to compile vertex shader because:" << infoLog << endl;
	}
	
	// then compile the frag shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "failed to compile fragment shader because:" << infoLog << endl;
	}

	// construct the shader program
	Shader::ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		cout << "failed to link vertex shader and fragment shader because:" << infoLog << endl;
	}

	//delete the temp shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// then we can using the ID to use this program
}


void Shader::use() {
	glUseProgram(this->ID);
}


