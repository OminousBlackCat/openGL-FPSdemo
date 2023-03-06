//
// Created by seu-wxy on 2023/3/6.
//

#ifndef OPENGL_FPSDEMO_MATERIAL_H
#define OPENGL_FPSDEMO_MATERIAL_H

#endif //OPENGL_FPSDEMO_MATERIAL_H

#include <glm/glm.hpp>
#include <string>
#include <vector>


class Material{
private:
    bool isValidate = false;
    // model根目录所在文件夹路径
    std::string root_path;
    // 颜色贴图文件(color texture map)(必须)
    std::string map_kd_fname;
    // 凹凸贴图文件
    std::string map_bump_fname;
    // 镜面反射贴图文件
    std::string map_ks_fname;
    // 镜面反射高光
    float ns_value;
    // 光密度(optical density)
    float ni_value;
    // 溶解(dissolve)值
    float d_value;
    // 指定光照模型 with
    // value 0: 简易光照模型
    // value 1/2: 冯氏光照模型
    int illum;
    // ka 环境光颜色
    glm::vec3 ka_value;
    // kd 漫反射光颜色
    glm::vec3 kd_value;
    // ks 镜面反射光颜色
    glm::vec3 ks_value;
    // ke Emissive(?)
    glm::vec3 ke_value;

public:
    // 构造函数, initialize member variable
    Material(const std::string& r_path = "", const std::string& map_kd_n = "", const std::string& map_b_n = "", const std::string& map_ks_n = ""){
        if(!r_path.empty())
            this->isValidate = true;
        this->root_path = r_path;
        this->map_kd_fname = map_kd_n;
        this->map_bump_fname = map_b_n;
        this->map_ks_fname = map_ks_n;
        this->ns_value = .0f;
        this->ni_value = .0f;
        this->d_value = .0f;
        this-> illum = 0;
        this->ka_value = glm::vec3(.0f);
        this->kd_value = glm::vec3(.0f);
        this->ks_value = glm::vec3(.0f);
        this->ke_value = glm::vec3(.0f);
    }

    bool ifValidate() const{
        return this->isValidate;
    }

    void setNoneVecValue(const float ns_v, const float ni_v, const float d_v, const int _illum){
        this->ns_value = ns_v;
        this->ni_value = ni_v;
        this->d_value = d_v;
        this->illum = _illum;
    }

    void setVecValue(const glm::vec3 ka_v, const glm::vec3 kd_v, const glm::vec3 ks_v, const glm::vec3 ke_v){
        this->ka_value = ka_v;
        this->kd_value = kd_v;
        this->ks_value = ks_v;
        this->ke_value = ke_v;
    }

    void processMTL(){
        if(!ifValidate())
            return;
        if()

    }



};