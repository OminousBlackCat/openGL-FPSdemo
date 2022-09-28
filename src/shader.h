#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

using namespace std;

class Shader {
public:
	// program id
	unsigned int ID;

	//constructor
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	// using this shader
	void use();
};

#endif