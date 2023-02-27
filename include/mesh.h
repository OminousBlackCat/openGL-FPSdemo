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
public:
    // 一个Mesh对象包含一组:
    // vertices 顶点坐标
    vector<glm::vec3> vertices;
    // vTexter 贴图坐标
    vector<glm::vec3> vTexter;
    // vNormal 法向量坐标
    vector<glm::vec3> vNormal;
    // indices 绘制顺序坐标
    vector<vector<int>> indices;

    


    


};

