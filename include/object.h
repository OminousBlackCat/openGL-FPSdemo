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
#include<unordered_map>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include "util.h"
#include "mesh.h"
#include "material.h"


class Object{
private:
    vector<Mesh> meshes;
    std::string current_path;
    std::string mtl_file_name;
    // 模型在世界坐标系中的位置
    glm::vec3 position;
    std::unordered_map<std::string, Material> mtl_map;

    void readObjFile(const std::string& obj_file_url = ""){
        if(obj_file_url.empty())
            return;
        std::fstream cur_obj_file;
        cur_obj_file.open(obj_file_url, ios::in);
        std::cout<<"Obj file: ["<<obj_file_url<<"] is loading..."<<endl;
        // 获得当前模型文件的路径 方便后续mtl文件和image文件的加载
        this->current_path = obj_file_url.substr(0, obj_file_url.find_last_of("/\\") + 1);
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
                if(util::getHeadElement(cur_line) == "mtllib"){
                    this->mtl_file_name = cur_line.substr(7);
                    continue;
                }
                // 如果:  "o MESH_NAME"
                //       "g MESH_NAME"
                //       "use mtl MTL_NAME"
                // usemtl 行, 把当前的mtl名称放入当前mesh中
                if(util::getHeadElement(cur_line) == "usemtl"){
                    // 判断mesh是否为空, 不为空就把之前的mesh推入数组
                    if(!cur_mesh.empty()){
                        meshes.push_back(cur_mesh);
                        // 清空当前mesh
                        cur_mesh = Mesh();
                    }
                    std::string mtl_name = cur_line.substr(7);
                    cur_mesh.setMtlName(mtl_name);
                    continue;
                }
                // 如果是"v c1 c2 c3"行, 放入vertices
                if(util::getHeadElement(cur_line) == "v"){
                    std::istringstream stm(cur_line.substr(2));
                    glm::vec3 v;
                    stm >> v.x;
                    stm >> v.y;
                    stm >> v.z;
                    cur_vertices.push_back(v);
                    continue;
                }
                // 如果是"vt c1 c2"行, 放入vT
                if(util::getHeadElement(cur_line) == "vt"){
                    std::istringstream stm(cur_line.substr(3));
                    glm::vec2 v;
                    stm >> v.x;
                    stm >> v.y;
                    cur_vTexture.push_back(v);
                    continue;
                }
                // vN同理
                if(util::getHeadElement(cur_line) == "vn"){
                    std::istringstream stm(cur_line.substr(3));
                    glm::vec3 v;
                    stm >> v.x;
                    stm >> v.y;
                    stm >> v.z;
                    cur_vNormal.push_back(v);
                    continue;
                }
                // 遇到 "f v1/vT1/vN1 v2/vT2/vN2 ..."则可以开始构建Mesh内的数据结构
                if(util::getHeadElement(cur_line) == "f"){
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
        }
    }

    void readMtlFile(const std::string& mtl_file_url = ""){
        std::fstream cur_mtl_file;
        cur_mtl_file.open(mtl_file_url, ios::in);
        std::cout<<"Mtl file: ["<<mtl_file_url<<"] is loading..."<<endl;
        std::string cur_root_path = this->current_path;
        // 获得当前模型文件的路径 方便后续mtl文件和image文件的加载
        if(cur_mtl_file.is_open()) {
            std::string cur_line;
            Material cur_mtl = Material();
            std::string mtl_name;
            std::string cur_map_kd;
            std::string cur_map_bump;
            std::string cur_map_ks;
            glm::vec3 cur_vec_ka = glm::vec3(.0f);
            glm::vec3 cur_vec_kd = glm::vec3(.0f);
            glm::vec3 cur_vec_ks = glm::vec3(.0f);
            glm::vec3 cur_vec_ke = glm::vec3(.0f);
            float cur_ns = .0f;
            float cur_ni = .0f;
            float cur_d = .0f;
            int cur_ill = 0;
            while(std::getline(cur_mtl_file, cur_line)){
                if(cur_line[0] == '#')
                    continue;
                if(util::getHeadElement(cur_line) == "newmtl"){
                    if(!cur_map_kd.empty() || !cur_map_bump.empty() || !cur_map_ks.empty()){
                        cur_mtl = Material(cur_root_path, cur_map_kd, cur_map_bump, cur_map_ks);
                        cur_mtl.setVecValue(cur_vec_ka, cur_vec_kd, cur_vec_ks, cur_vec_ke);
                        cur_mtl.setNoneVecValue(cur_ns, cur_ni, cur_d, cur_ill);
                        mtl_map.insert({mtl_name, cur_mtl});
                    }
                    mtl_name = util::getTailElement(cur_line);
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Ns"){
                    cur_ns = std::stof(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Ka"){
                    std::istringstream stm(cur_line.substr(3));
                    stm >> cur_vec_ka.x;
                    stm >> cur_vec_ka.y;
                    stm >> cur_vec_ka.z;
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Kd"){
                    std::istringstream stm(cur_line.substr(3));
                    stm >> cur_vec_kd.x;
                    stm >> cur_vec_kd.y;
                    stm >> cur_vec_kd.z;
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Ks"){
                    std::istringstream stm(cur_line.substr(3));
                    stm >> cur_vec_ks.x;
                    stm >> cur_vec_ks.y;
                    stm >> cur_vec_ks.z;
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Ke"){
                    std::istringstream stm(cur_line.substr(3));
                    stm >> cur_vec_ke.x;
                    stm >> cur_vec_ke.y;
                    stm >> cur_vec_ke.z;
                    continue;
                }
                if(util::getHeadElement(cur_line) == "Ni"){
                    cur_ni = std::stof(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "d "){
                    cur_d = std::stof(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "illum"){
                    cur_ill = std::stoi(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "map_Kd"){
                    cur_map_kd = util::alterBackslash(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "map_Bump" || util::getHeadElement(cur_line) == "map_bump"){
                    cur_map_bump = util::alterBackslash(util::getTailElement(cur_line));
                    continue;
                }
                if(util::getHeadElement(cur_line) == "map_Ks"){
                    cur_map_ks = util::alterBackslash(util::getTailElement(cur_line));
                    continue;
                }
            }
            if(!cur_map_kd.empty() || !cur_map_bump.empty() || !cur_map_ks.empty()){
                cur_mtl = Material(cur_root_path, cur_map_kd, cur_map_bump, cur_map_ks);
                cur_mtl.setVecValue(cur_vec_ka, cur_vec_kd, cur_vec_ks, cur_vec_ke);
                cur_mtl.setNoneVecValue(cur_ns, cur_ni, cur_d, cur_ill);
                mtl_map.insert({mtl_name, cur_mtl});
            }
        }
    }

public:
    explicit Object(const std::string& file_url = ""){
        readObjFile(file_url);
        readMtlFile(this->current_path + this->mtl_file_name);
        // 对mesh 成员进行初始化
        for(auto &m : meshes){
            cout<<"Processing mesh: ["<<m.mtlName<<"]..."<<endl;
            m.processVAO(this->mtl_map[m.mtlName]);
        }
    }

    void draw(Shader shader, glm::mat4 projectionMat, glm::mat4 viewMat){
        for(auto &m : this->meshes)
            m.draw(shader, projectionMat, viewMat, this->mtl_map[m.mtlName]);
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

    void rotate(glm::vec3 axis, float degree){
        for(auto &m: this->meshes)
            m.rotate(axis, degree);
    }

};