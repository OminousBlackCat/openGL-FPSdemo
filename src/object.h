//
// Created by seu-wxy on 2022/10/7.
//

#ifndef OPENGL_FPSDEMO_OBJECT_H
#define OPENGL_FPSDEMO_OBJECT_H

#endif //OPENGL_FPSDEMO_OBJECT_H

#include<string>
#include<glm/glm.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"shader.h"


class Object{
public:
    Object(const std::string& textureURL, const std::string& specTextureURL){
        // 在构造函数里初始化贴图与模型矩阵, 以及物体的初始位置
        this->bufferTexture(textureURL, specTextureURL);
        this->modelMat = glm::mat4(1.0f);
        this->position = glm::vec3(1.0f);
    }

    virtual void bufferVAO() = 0;

    virtual void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat) = 0;

    void bufferTexture(const std::string& textureURL, const std::string& specTextureURL){
        // bind texture
        int width, height, channelCount;
        unsigned char* textureContent = stbi_load(textureURL.c_str(), &width, &height, &channelCount, 0);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int RGBMode = GL_RGB;
        if(channelCount == 4)
            RGBMode = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, width, height, 0, RGBMode, GL_UNSIGNED_BYTE, textureContent);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureContent);
        glBindTexture(GL_TEXTURE_2D, 0);

        textureContent = stbi_load(specTextureURL.c_str(), &width, & height, &channelCount, 0);
        glGenTextures(1, &this->specTextureID);
        glBindTexture(GL_TEXTURE_2D, this->specTextureID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if(channelCount == 4)
            RGBMode = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, width, height, 0, RGBMode, GL_UNSIGNED_BYTE, textureContent);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureContent);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void translation(glm::vec3 translation_value){
        modelMat = glm::translate(modelMat, translation_value);
        this->position += translation_value;
    }

protected:
    unsigned int VAO;
    unsigned int textureID;
    unsigned int specTextureID;
    glm::vec3 position;
    glm::mat4 modelMat;
    float vertex_array;
};


class CubeWithTex final : public Object{
public:
    CubeWithTex(const std::string& textureURL, const std::string& specTextureURL): Object(textureURL, specTextureURL){
        this->bufferVAO();
    }
    void bufferVAO() final{
        // bind VAO
        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);
        // gen VBO
        unsigned int verticesID;
        glGenBuffers(1, &verticesID);
        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->cube_vertex), this->cube_vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glBindVertexArray(0);
    }
    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat) final{
        shader.use();

        // 输入uniform变量
        shader.uniform_mat4(projectionMat, "projectionMat");
        shader.uniform_mat4(viewMat, "viewMat");
        shader.uniform_mat4(this->modelMat, "modelMat");


        glBindVertexArray(this->VAO);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glUniform1i(glGetUniformLocation(shader.ID, "material.objectTexture"), 2);
        glActiveTexture(GL_TEXTURE3); // 启用纹理1 作为镜面反射纹理
        glBindTexture(GL_TEXTURE_2D, this->specTextureID);
        glUniform1i(glGetUniformLocation(shader.ID, "material.specTexture"), 3);
        glDrawArrays(GL_TRIANGLES, 0, vertex_array_size / 8);
        glBindVertexArray(0);
    }

private:
    static const int vertex_array_size = 288;
    const float cube_vertex[vertex_array_size] = {
            // ---position--		--texture--  --normal--
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,    0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	-1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	-1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	-1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	 0.0f,  1.0f,  0.0f
    };
};


class Floor{
private:
    unsigned int floorVAO;
    unsigned int textureID;
    unsigned int specTextureID;
    glm::mat4 modelMat{};
    const float floor_vertex[48] = {
              -2.0f, 0.f,  2.f, 0.f, 0.f, 0.f, 1.0f, 0.0f,
               2.f, 0.f,  2.f, 0.f, 100.f,0.f, 1.0f, 0.0f,
              -2.f, 0.f, -2.f, 100.f, 0.f,0.f, 1.0f, 0.0f,
               2.f,0.f, 2.f,0.f, 100.f,0.f, 1.0f, 0.0f,
             -2.f,0.f,-2.f, 100.f, 0.f,0.f, 1.0f, 0.0f,
              2.f,0.f,-2.f, 100.f, 100.f,0.f, 1.0f, 0.0f
    };

public:
    Floor(const std::string& textureURL, const std::string& specTextureURL){
        // bind texture
        int width, height, channelCount;
        unsigned char* textureContent = stbi_load(textureURL.c_str(), &width, &height, &channelCount, 0);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int RGBMode = GL_RGB;
        if(channelCount == 4)
            RGBMode = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, width, height, 0, RGBMode, GL_UNSIGNED_BYTE, textureContent);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureContent);  
        glBindTexture(GL_TEXTURE_2D, 0);

        textureContent = stbi_load(specTextureURL.c_str(), &width, & height, &channelCount, 0);
        glGenTextures(1, &this->specTextureID);
        glBindTexture(GL_TEXTURE_2D, this->specTextureID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if(channelCount == 4)
            RGBMode = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, width, height, 0, RGBMode, GL_UNSIGNED_BYTE, textureContent);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(textureContent);
        glBindTexture(GL_TEXTURE_2D, 0);

        // bind VAO
        glGenVertexArrays(1, &this->floorVAO);
        glBindVertexArray(this->floorVAO);
        // gen VBO
        unsigned int verticesID;
        glGenBuffers(1, &verticesID);
        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->floor_vertex), this->floor_vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glBindVertexArray(0);

        this->modelMat = glm::mat4(1.0f);
        modelMat = glm::scale(modelMat, glm::vec3(100.f, 100.f, 100.f));
    }

    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat){
        shader.use();

        // 输入uniform变量
        shader.uniform_mat4(projectionMat, "projectionMat");
        shader.uniform_mat4(viewMat, "viewMat");
        shader.uniform_mat4(this->modelMat, "modelMat");

        
        glBindVertexArray(this->floorVAO); 
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, this->textureID);
        glUniform1i(glGetUniformLocation(shader.ID, "material.objectTexture"), 2);
        glActiveTexture(GL_TEXTURE3); // 启用纹理1 作为镜面反射纹理
        glBindTexture(GL_TEXTURE_2D, this->specTextureID);
        glUniform1i(glGetUniformLocation(shader.ID, "material.specTexture"), 3);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};
