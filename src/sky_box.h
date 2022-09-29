#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <vector>
#include "shader.h"

using namespace std;

// 使用此h文件内的类创建一个skybox(一个VAO, 并需要有对应的shader来画)

class SkyBox{
public:
    SkyBox(const vector<string> file_urls){
        glGenTextures


    }

    void draw(Shader& shader){

    }
    

private:
    unsigned int main_vao;
    unsigned int sky_tex_positive_x;
    unsigned int sky_tex_negative_x;
    unsigned int sky_tex_positive_y;
    unsigned int sky_tex_negative_y;
    unsigned int sky_tex_positive_z;
    unsigned int sky_tex_negative_z;

    float cube_vertices[] = {
		 0.0f,  0.0f,  0.0f,	//0
		 1.0f,  0.0f,  0.0f,  	//1
		 0.0f,  1.0f,  0.0f,	//2
		 1.0f,  1.0f,  0.0f,	//3
		 0.0f,  0.0f,  1.0f,	//4
		 1.0f,  0.0f,  1.0f,	//5
		 0.0f,  1.0f,  1.0f,	//6
		 1.0f,  1.0f,  1.0f, 	//7
	};
    float cube_elementIndex[] = {	
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
		0,1,4,
		1,4,5,
		2,3,6,
		3,6,7,
		1,3,5,
		3,5,7,
		0,2,4,
		2,4,6
	};





};