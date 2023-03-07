//
// Created by seu-wxy on 2023/02.
//
#pragma once
#include<vector>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include "shader.h"

using namespace std;

// Mesh为渲染的最小单位, 使用单个
class Mesh{

private:
    // 绘制所需的顶点数据
    // 一个顶点为一组float, 包含:
    // vertices 顶点坐标(vec3)
    // vTexture 贴图坐标(vec2)
    // vNormal 法向量坐标(vec3)

    // 因此最后传入的绘制数据格式如下:
    // -vertices cor(vec3)-vTexture cor(vec2)-vNormal cor(vec3)
    vector<float> vBuffer;
    // VAO ID
    unsigned int _index_VAO;
    // texture ID
    unsigned int textureID;
    // spec texture ID (if have)
    unsigned int specTextureID;
    // model matrix
    glm::mat4 modelMat;
    // 平移矩阵, 用来获得最后的modelMat
    glm::mat4 translationMat;
    // 缩放矩阵, 用来获得最后的ModelMat
    glm::mat4 scaleMat;
    // 模型位置, 和object共享位置
    glm::vec3 position;

public:
    // 材质名称, 从obj模型文件中可以获取, 每个mesh对应一个材质贴图
    string mtlName;

    Mesh(){
        // 初始化成员变量
        this->position = glm::vec3(0.0f);
        this->translationMat = glm::mat4(1.0f);
        this->scaleMat = glm::mat4(1.0f);
        this->modelMat = glm::mat4(1.0f);
        this->textureID = -1;
        this->specTextureID = -1;
        vBuffer.clear();
    }

    // 向顶点数据内添加一项
    void addVertexData(vector<float>& temp_data){
        for(auto &f : temp_data)
            this->vBuffer.push_back(f);
    }

    // 设定mtl名称
    void setMtlName(string& temp_name){
        this->mtlName = temp_name;
    }

    // 查看mesh是否为空
    bool empty(){
        return vBuffer.empty();
    }

    // 创建VAO
    void processVAO(Material& material){
        if(material.kdContent.validate){
            // bind texture
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, this->textureID);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int RGBMode = GL_RGB;
            if(material.kdContent.nrChannels == 4)
                RGBMode = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, material.kdContent.width, material.kdContent.height, 0, RGBMode, GL_UNSIGNED_BYTE, material.kdContent.imageContent);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if(material.ksContent.validate){
            glGenTextures(1, &this->specTextureID);
            glBindTexture(GL_TEXTURE_2D, this->specTextureID);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int RGBMode = GL_RGB;
            if(material.ksContent.nrChannels == 4)
                RGBMode = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, RGBMode, material.ksContent.width, material.ksContent.height, 0, RGBMode, GL_UNSIGNED_BYTE, material.ksContent.imageContent);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // bind VAO
        glGenVertexArrays(1, &this->_index_VAO);
        glBindVertexArray(this->_index_VAO);
        // gen VBO
        unsigned int verticesID;
        glGenBuffers(1, &verticesID);
        glBindBuffer(GL_ARRAY_BUFFER, verticesID);
        glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(float), &vBuffer[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glBindVertexArray(0);
    }


    //  绘制mesh
    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat, Material& material){
        shader.use();

        // 输入uniform变量
        shader.uniform_mat4(projectionMat, "projectionMat");
        shader.uniform_mat4(viewMat, "viewMat");
        shader.uniform_mat4(this->modelMat, "modelMat");
        shader.uniform_material(material, "material");

        glBindVertexArray(this->_index_VAO);
        // 启用颜色纹理
        if(this->textureID != -1){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->textureID);
            glUniform1i(glGetUniformLocation(shader.ID, "material.colorTexture"), 0);
        }
        // 启用纹理1 作为镜面反射纹理
        if(this->specTextureID != -1){
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, this->specTextureID);
            glUniform1i(glGetUniformLocation(shader.ID, "material.specTexture"), 1);
        }
        glDrawArrays(GL_TRIANGLES, 0, vBuffer.size());
        glBindVertexArray(0);
    }

    void setPosition(glm::vec3 new_position){
        this->translationMat = glm::translate(translationMat, new_position - this->position);
        this->position = new_position;
        this->modelMat = this->translationMat * this->scaleMat;
    }

    void setPosition(float n_x, float n_y, float n_z){
        this->translationMat = glm::translate(translationMat, glm::vec3(n_x, n_y, n_z) - this->position);
        this->position = glm::vec3(n_x, n_y, n_z);
        this->modelMat = this->translationMat * this->scaleMat;
    }

    void scale(float rate){
        this->scaleMat = glm::scale(scaleMat, glm::vec3(rate, rate ,rate));
        this->modelMat = this->translationMat * this->scaleMat;
    }

    void translation(glm::vec3 translation_value){
        this->translationMat = glm::translate(translationMat, translation_value);
        this->position += translation_value;
        this->modelMat = this->translationMat * this->scaleMat;
    }

};

