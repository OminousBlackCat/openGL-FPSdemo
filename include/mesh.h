//
// Created by seu-wxy on 2023/02.
//
#pragma once
#include<vector>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

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
    vector<vector<float>> vBuffer;
    // 材质名称, 从obj模型文件中可以获取, 每个mesh对应一个材质贴图
    string mtlName;

public:




    Mesh(){
        this->mtlName = "";
        vBuffer.clear();
    }

    // 向顶点数据内添加一项
    void addVertexData(vector<float>& temp_data){
        this->vBuffer.push_back(temp_data);
    }

    // 设定mtl名称
    void setMtlName(string& temp_name){
        this->mtlName = temp_name;
    }

};

