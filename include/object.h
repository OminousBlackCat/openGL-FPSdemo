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
    std::string current_url;
    // 模型在世界坐标系中的位置
    glm::vec3 position;

    void readObjFile(const std::string& obj_file_url = ""){
        if(obj_file_url.empty())
            return;
        std::fstream cur_obj_file;
        cur_obj_file.open(obj_file_url, ios::in);
        std::cout<<"Obj file: ["<<MODEL_DIR"/" + obj_file_url<<"] is loading..."<<endl;
        if(cur_obj_file.is_open()){
            std::string cur_line;
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

                    continue;
                }
                // 如果是"o MESH_NAME"行, 开始新的Mesh
                if(cur_line.substr(0, 2) == "o "){
                    // 判断mesh是否为空, 不为空就把之前的mesh推入数组
                    if(!cur_vertices.empty()){
                        meshes.push_back(cur_mesh);
                        // 清空当前mesh
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
                if(cur_line.substr(0, 2) == "vn"){
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
                    std::string cur_set;
                    vector<std::string> tmp_strings;
                    // split by space
                    while(std::getline(ss, cur_set, ' ')){
                        // 将string输入到一个temp中, 判断有多少个顶点
                        tmp_strings.push_back(cur_set);
                    }
                    // 如果一个f 包含多个顶点, 则按照以下算法拆成三角形:
                    // 算法 -----
                    //      f contains n vertices
                    //      for i from 1 to n - 2:
                    //          add triangle with vertices[1, i, i + 1]
                    for(int i = 0;i<tmp_strings.size() - 2;i++){
                        vector<float> tmp_floats;
                        vector<int> tmp_index;
                        // 获得一行f, 并去除'/' 输出所有序号
                        std::string vets = tmp_strings[0] + "/";
                        vets += tmp_strings[i + 1] + "/";
                        vets += tmp_strings[i + 2];
                        // 注意此处获得的序号是从1开始计数的
                        ss = std::stringstream (vets);
                        while(std::getline(ss, cur_set, '/')){
                            tmp_index.push_back(std::stoi(cur_set) - 1);
                        }
                        // tmp_index 中顺序为 [ v1, vT1, vN1, v2, vT2, vN2, ..., vn, vTn, vNn]
                        for(int _i = 0; _i < 3; ++_i){
                            // 获得一个顶点的完整数据 加入到tmp_floats中
                            int _t_index = _i * 3;
                            // v
                            tmp_floats.push_back(cur_vertices[tmp_index[_t_index]].x);
                            tmp_floats.push_back(cur_vertices[tmp_index[_t_index]].y);
                            tmp_floats.push_back(cur_vertices[tmp_index[_t_index]].z);
                            // vT
                            tmp_floats.push_back(cur_vTexture[tmp_index[_t_index + 1]].x);
                            tmp_floats.push_back(cur_vTexture[tmp_index[_t_index + 1]].y);
                            // vN
                            tmp_floats.push_back(cur_vNormal[tmp_index[_t_index + 2]].x);
                            tmp_floats.push_back(cur_vNormal[tmp_index[_t_index + 2]].y);
                            tmp_floats.push_back(cur_vNormal[tmp_index[_t_index + 2]].z);
                            // 可以保证mesh中添加的顺序都是正确的, 顺序的三个点为三角形三个顶点
                            cur_mesh.addVertexData(tmp_floats);
                            tmp_floats.clear();
                        }
                    }
                }
            }
            // 别忘了读完把最后的mesh也存进去
            this->meshes.push_back(cur_mesh);
        }else
            cout << "OBJ file: [" << MODEL_DIR"/" + obj_file_url << "] could not open...";
        // 初始化object的成员变量
        this->position = glm::vec3(0.0f);
        // 读取成功后 对mesh进行初始化
        for(auto &m : this->meshes){
            if(m.empty()){
                cout << "One of the obj mesh is empty!!! please check obj file format !"<<endl;
                break;
            }
            m.processVAO();
        }
    }

    void readMtlFile(const std::string& mtl_file_url = ""){

    }

public:
    explicit Object(const std::string& file_url = ""){

    }

    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat){
        for(auto &m : this->meshes)
            m.draw(shader, projectionMat, viewMat);
    }

    void setPosition(glm::vec3 new_position){
        this->position = new_position;
        for(auto &m: this->meshes){
            m.setPosition(new_position);
        }
    }

    void setPosition(float n_x, float n_y, float n_z){
        this->position = glm::vec3(n_x, n_y, n_z);
        for(auto &m: this->meshes){
            m.setPosition(n_x, n_y, n_z);
        }
    }

    void scale(float rate){
        for(auto &m: this->meshes)
            m.scale(rate);
    }

    void translation(glm::vec3 translation_value){
        for(auto &m: this->meshes){
            m.translation(translation_value);
        }
    }

};