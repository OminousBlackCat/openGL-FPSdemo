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
    // 材质名称, 从obj模型文件中可以获取, 每个mesh对应一个材质贴图
    string mtlName;
    // VAO ID
    unsigned int _index_VAO;
    // texture ID
    //unsigned int textureID;
    // spec texture ID (if have)
    unsigned int specTextureID;
    // model matrix
    glm::mat4 modelMat{};
    // 模型位置
    glm::vec3 position;

public:
    Mesh(){
        this->mtlName = "";
        position = glm::vec3(0.0f);
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
    void processVAO(){
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

        this->modelMat = glm::mat4(1.0f);
        modelMat = glm::scale(modelMat, glm::vec3(0.01f, 0.01f, 0.01f));
    }

    //  绘制mesh
    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat){
        shader.use();

        // 输入uniform变量
        shader.uniform_mat4(projectionMat, "projectionMat");
        shader.uniform_mat4(viewMat, "viewMat");
        shader.uniform_mat4(this->modelMat, "modelMat");


        glBindVertexArray(this->_index_VAO);
        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, this->textureID);
        //glUniform1i(glGetUniformLocation(shader.ID, "material.objectTexture"), 2);
        //glActiveTexture(GL_TEXTURE3); // 启用纹理1 作为镜面反射纹理
        //glBindTexture(GL_TEXTURE_2D, this->specTextureID);
        //glUniform1i(glGetUniformLocation(shader.ID, "material.specTexture"), 3);
        glDrawArrays(GL_TRIANGLES, 0, vBuffer.size());
        glBindVertexArray(0);
    }

    void translation(glm::vec3 translation_value){
        modelMat = glm::translate(modelMat, translation_value);
        this->position += translation_value;
    }

};

