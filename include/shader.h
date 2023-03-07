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
#include "material.h"
#include "light.h"

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

    void uniform_mat4(glm::mat4 input, const string& valueName) const{
        glUniformMatrix4fv(glGetUniformLocation(this->ID, valueName.c_str()), 1, GL_FALSE, glm::value_ptr(input));
    };

    void uniform_vec3(glm::vec3 input, const string& valueName) const{
        glUniform3f(glGetUniformLocation(this->ID, valueName.c_str()), input.x, input.y, input.z);
    }

    void uniform_vec3(float input1, float input2, float input3, const string& valueName) const{
        glUniform3f(glGetUniformLocation(this->ID, valueName.c_str()), input1, input2, input3);
    }

    void uniform_vec4(glm::vec4 input, const string& valueName) const{
        glUniform4f(glGetUniformLocation(this->ID, valueName.c_str()), input.x, input.y, input.z, input.w);
    }

    void uniform_vec4(float inputX, float inputY, float inputZ, float inputW, const string& valueName) const{
        glUniform4f(glGetUniformLocation(this->ID, valueName.c_str()), inputX, inputY, inputZ, inputW);
    }

    void uniform_float(float input, const string& valueName) const{
        glUniform1f(glGetUniformLocation(this->ID, valueName.c_str()), input);
    }

    void uniform_int(const int input, const string& valueName) const{
        glUniform1i(glGetUniformLocation(this->ID, valueName.c_str()), input);
    }

    void uniform_material(const Material& input, const string& valueName) const{
        if(input.kdContent.validate)
            uniform_int(1, valueName + ".isColorTexVal");
        else
            uniform_int(0, valueName + ".isColorTexVal");
        if(input.ksContent.validate)
            uniform_int(1, valueName + ".isSpecTexVal");
        else
            uniform_int(0, valueName + ".isSpecTexVal");
        if(input.bumpContent.validate)
            uniform_int(1, valueName + ".dumpTexture");
        else
            uniform_int(0, valueName + ".dumpTexture");
        uniform_float(input.ns_value, valueName + ".ns_value");
        uniform_float(input.d_value, valueName + ".d_value");
        uniform_float(input.ni_value, valueName + ".ni_value");
        uniform_vec3(input.ka_value, valueName + ".ka_value");
        uniform_vec3(input.ks_value, valueName + ".ks_value");
        uniform_vec3(input.kd_value, valueName + ".kd_value");
    }

    void uniform_pointLight(const PointLight& input, const string& valueName) const{
        uniform_vec3(input.position, valueName + ".position");
        uniform_vec3(input.ambient, valueName + ".ambient");
        uniform_vec3(input.diffuse, valueName + ".diffuse");
        uniform_vec3(input.specular, valueName + ".specular");
        uniform_float(input.attenuation_constant, valueName + ".attenuation_constant");
        uniform_float(input.attenuation_linear, valueName + ".attenuation_linear");
        uniform_float(input.attenuation_quadratic, valueName + ".attenuation_quadratic");
    }

    void uniform_flashLight(const FlashLight& input, const string& valueName) const{
        uniform_int(input.flashSwitch, valueName + ".flashSwitch");
        uniform_vec3(input.position, valueName + ".position");
        uniform_vec3(input.ambient, valueName + ".ambient");
        uniform_vec3(input.diffuse, valueName + ".diffuse");
        uniform_vec3(input.specular, valueName + ".specular");
        uniform_vec3(input.direction, valueName + ".direction");
        uniform_float(input.cutOff, valueName + ".cutOff");
        uniform_float(input.outerCutOff, valueName + ".outerCutOff");
        uniform_float(input.attenuation_constant, valueName + ".attenuation_constant");
        uniform_float(input.attenuation_linear, valueName + ".attenuation_linear");
        uniform_float(input.attenuation_quadratic, valueName + ".attenuation_quadratic");
    }

};

#endif