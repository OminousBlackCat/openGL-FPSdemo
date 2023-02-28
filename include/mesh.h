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
    vector<vector<float>> vBuffer;
    string mtlName;

public:
    // 一个Mesh对象包含一组:
    // vertices 顶点坐标
    // vTexture 贴图坐标
    // vNormal 法向量坐标
    // indices 绘制顺序坐标

    // 最后传入的绘制数据格式如下:
    // for each triangle:
    // -vertices cor(vec3)-vTexture cor(vec2)-vNormal cor(vec3)

    Mesh(){
        this->mtlName = "";
        vBuffer.clear();
    }

    void addVertexData(vector<float>& temp_data){
        this->vBuffer.push_back(temp_data);
    }

    void setMtlName(string& temp_name){
        this->mtlName = temp_name;
    }

};

