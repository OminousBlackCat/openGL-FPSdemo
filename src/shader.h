#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<glm/glm.hpp>
#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
	// program id
	unsigned int ID;

	//constructor
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath){
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

		// then we can use the ID to use this program
	};
	// using this shader
	void use() const{
		glUseProgram(this->ID);
	};

    void uniform_mat4(glm::mat4 input, const char* valueName) const{
        glUniformMatrix4fv(glGetUniformLocation(this->ID, valueName), 1, GL_FALSE, glm::value_ptr(input));
    };

    void uniform_vec3(glm::vec3 input, const char* valueName) const{
        glUniform3f(glGetUniformLocation(this->ID, valueName), input.x, input.y, input.z);
    }

    void uniform_vec3(float input1, float input2, float input3, const char* valueName) const{
        glUniform3f(glGetUniformLocation(this->ID, valueName), input1, input2, input3);
    }

    void uniform_vec4(glm::vec4 input, const char* valueName) const{
        glUniform4f(glGetUniformLocation(this->ID, valueName), input.x, input.y, input.z, input.w);
    }

    void uniform_vec4(float inputX, float inputY, float inputZ, float inputW, const char* valueName) const{
        glUniform4f(glGetUniformLocation(this->ID, valueName), inputX, inputY, inputZ, inputW);
    }

    void uniform_float(float input, const char* valueName) const{
        glUniform1f(glGetUniformLocation(this->ID, valueName), input);
    }

};

#endif