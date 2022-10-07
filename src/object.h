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

class Floor{
private:
    unsigned int floorVAO{};
    unsigned int textureID{};
    glm::mat4 modelMat;
    const float floor_vertex[30] = {
              -1.0f, 0.0f,  1.f, 0.f, 0.f,
               1.f, 0.f,  1.f, 0.f, 100.f,
              -1.f, 0.f, -1.f, 100.f, 0.f,
               1.f,0.f, 1.f,0.f, 100.f,
             -1.f,0.f,-1.f, 100.f, 0.f,
              1.f,0.f,-1.f, 100.f, 100.f
    };

public:
    Floor(const std::string& textureURL){
        // bind texture
        glGenTextures(1, &textureID);
        int width, height, channelCount;
        unsigned char* textureContent = stbi_load(textureURL.c_str(), &width, &height, &channelCount, 0);
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
        glTexImage2D(GL_TEXTURE_2D, this->textureID, RGBMode, width, height, 0, RGBMode, GL_UNSIGNED_BYTE, textureContent);
        stbi_image_free(textureContent);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(0, 0);

        // bind VAO
        glGenVertexArrays(1, &this->floorVAO);
        glBindVertexArray(this->floorVAO);
        // gen VBO
        unsigned int verticesID;
        glGenBuffers(1, &verticesID);
        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->floor_vertex), this->floor_vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glBindVertexArray(0);

        this->modelMat = glm::mat4(1.f);
    }

    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat){
        shader.use();

        // 将地板resize
        modelMat = glm::translate(modelMat, glm::vec3(0.f, -1.f, 0.f));

        // 输入uniform变量
        shader.uniform_mat4(projectionMat, "projectionMat");
        shader.uniform_mat4(viewMat, "viewMat");
        shader.uniform_mat4(this->modelMat, "modelMat");


        glBindVertexArray(this->floorVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader.ID, "floorTexture"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }


};
