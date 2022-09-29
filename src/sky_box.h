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
        // 绑定特定纹理的指针
        glGenTextures(1, &sky_tex);
        glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, sky_tex);


        // 再输入正确的图片(文件)
        // 一个立方体贴图包含六张图 需要使用六次TexImage2D
        int image_width, image_height, image_channel;
        unsigned char* image;
        for(int i = 0; i < file_urls.size(); i++){
            image = stbi_load(file_urls[i], &image_width, &image_height, &image_channel, 0);
            if(image_channel == 3){
                glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            }
            if(image_channel == 4){
                glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                        0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            }
        }

        // 配置此立方体贴图的属性
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //解绑

        // 配置VAO
        const float cube_vertices[] = {
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

        glGenVertexArrays(1, &main_vao);
        glBindVertexArray(main_vao);
        unsigned int vertices, elements;
        glGenBuffers(1, &vertices); // 第一个存顶点数据
        glGenBuffers(1, &elements); // 第二个存绘制顺序

        glBindBuffer(GL_ARRAY_BUFFER, vertices); // 绑定第一个
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW); // 输入缓存
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (cube_elementIndex), cube_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(0); //enable location = 0
        glBindVertexArray(0);
    }

    // 需要第一个绘制天空盒
    void draw(Shader& shader){
        glDepthMask(GL_FALSE); //关闭深度写入
        shader.use();

        glBindVertexArray(this->main_vao);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->sky_tex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
    }
    

private:
    unsigned int main_vao;
    unsigned int sky_tex;
};