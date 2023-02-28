//
// Created by seu-wxy on 2023/2/28.
//

#ifndef OPENGL_FPSDEMO_OBJECT_H
#define OPENGL_FPSDEMO_OBJECT_H

#endif //OPENGL_FPSDEMO_OBJECT_H

#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include "mesh.h"


class Object{
private:
    vector<Mesh> meshes;
    string mtl_file_url;
    string current_url;


public:
    explicit Object(string file_url = ""){
        if(file_url.empty())
            return;
        std::fstream cur_obj_file;
        cur_obj_file.open(MODEL_DIR"/" + file_url, ios::in);
        std::cout<<"Obj file: ["<<MODEL_DIR"/" + file_url<<"] is loading..."<<endl;
        if(cur_obj_file.is_open()){
            string cur_line;
            // 一行一行开始读
            // 一个mesh的顶点坐标数组
            vector<glm::vec3> cur_vertices;
            // 一个mesh的UV数组
            vector<glm::vec2> cur_vTexture;
            // 一个mesh的法向量数组
            vector<glm::vec3> cur_vNormal;
            // 当前的mesh
            Mesh cur_mesh;
            while(std::getline(cur_obj_file, cur_line)){
                // 如果是注释, 直接跳过
                if(cur_line.substr(0, 1) == "#")
                    continue;
                // 如果是声明mtl文件位置行, 输入mtl_file_url
                if(cur_line.substr(0, 6) == "mtllib"){
                    mtl_file_url = cur_line.substr(7, string::npos);
                    continue;
                }
                // 如果是"o MESH_NAME"行, 开始新的Mesh
                if(cur_line.substr(0, 2) == "o "){
                    // 判断mesh是否为空, 不为空就把之前的mesh推入数组
                    if(!cur_vertices.empty()){
                        meshes.push_back(cur_mesh);
                        // 清空所有数组
                        cur_vertices.clear();
                        cur_vTexture.clear();
                        cur_vNormal.clear();
                        cur_mesh = Mesh();
                    }
                    continue;
                }
                // 如果是"v c1 c2 c3"行, 放入vertices
                if(cur_line.substr(0, 2) == "v "){
                    std::istringstream stm(cur_line.substr(2));
                    glm::vec3 v;
                    stm >> v.x;
                    stm >> v.y;
                    stm >> v.z;
                    cur_vertices.push_back(v);
                    continue;
                }
                // 如果是"vt c1 c2"行, 放入vT
                if(cur_line.substr(0, 2) == "vt"){
                    std::istringstream stm(cur_line.substr(3));
                    glm::vec2 v;
                    stm >> v.x;
                    stm >> v.y;
                    cur_vTexture.push_back(v);
                    continue;
                }
                // vN同理
                if(cur_line.substr(0, 2) == "vN"){
                    std::istringstream stm(cur_line.substr(3));
                    glm::vec3 v;
                    stm >> v.x;
                    stm >> v.y;
                    stm >> v.z;
                    cur_vNormal.push_back(v);
                    continue;
                }
                // 遇到 "f v1/vT1/vN1 v2/vT2/vN2 ..."则可以开始构建Mesh内的数据结构
                if(cur_line.substr(0, 2) == "f "){
                    std::stringstream ss(cur_line.substr(2));
                    string cur_set;
                    vector<string> tmp_strings;
                    // split by space
                    while(std::getline(ss, cur_set, ' ')){
                        // 将string输入到一个temp中, 判断有多少个顶点
                        tmp_strings.push_back(cur_set);
                    }
                    for(int i = 0;i<tmp_strings.size() - 2;i++){
                        vector<float> tmp_floats;
                        string v1 = tmp_strings[0];
                        string v2 = tmp_strings[i];
                        string v3 = tmp_strings[i + 1];
                    }
                }
            }
        }else
            cout << "OBJ file: [" << MODEL_DIR"/" + file_url << "] could not open...";
    }

};