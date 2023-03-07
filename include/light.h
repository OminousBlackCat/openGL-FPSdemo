//
// Created by seu-wxy on 2023/3/7.
//
#pragma once
#ifndef OPENGL_FPSDEMO_LIGHT_H
#define OPENGL_FPSDEMO_LIGHT_H

#endif //OPENGL_FPSDEMO_LIGHT_H

#include "glm/glm.hpp"



struct DirLight {
    DirLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &direction)
            : ambient(ambient), diffuse(diffuse), specular(specular), direction(direction) {}

    glm::vec3 ambient = glm::vec3(.0f);
    glm::vec3 diffuse = glm::vec3(.0f);
    glm::vec3 specular = glm::vec3(.0f);
    glm::vec3 direction = glm::vec3(.0f);
};


struct PointLight{
    PointLight(const glm::vec3 &position, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
               float attenuationConstant, float attenuationLinear, float attenuationQuadratic) :
               position(position),ambient(ambient),diffuse(diffuse),specular(specular),
               attenuation_constant(attenuationConstant),attenuation_linear(attenuationLinear),
               attenuation_quadratic(attenuationQuadratic) {}

    glm::vec3 position = glm::vec3(.0f);
    glm::vec3 ambient = glm::vec3(.0f);
    glm::vec3 diffuse = glm::vec3(.0f);
    glm::vec3 specular = glm::vec3(.0f);
    float attenuation_constant = .0f;  //衰减常数项
    float attenuation_linear = .0f;    //衰减线性项
    float attenuation_quadratic = .0f; //衰减二次项
};

struct FlashLight{

    FlashLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, float cutOff,
               float outerCutOff, float attenuationConstant, float attenuationLinear, float attenuationQuadratic)
            : ambient(ambient), diffuse(diffuse), specular(specular), cutOff(cutOff), outerCutOff(outerCutOff),
              attenuation_constant(attenuationConstant), attenuation_linear(attenuationLinear),
              attenuation_quadratic(attenuationQuadratic) {}
    bool flashSwitch = true;
    glm::vec3 position = glm::vec3(.0f);
    glm::vec3 ambient = glm::vec3(.0f);
    glm::vec3 diffuse = glm::vec3(.0f);
    glm::vec3 specular = glm::vec3(.0f);
    glm::vec3 direction = glm::vec3(.0f);
    float cutOff = .0f; //手电筒半切角的余弦值
    float outerCutOff = .0f; //外圆锥余弦值
    float attenuation_constant = .0f;  //衰减常数项
    float attenuation_linear = .0f;    //衰减线性项
    float attenuation_quadratic = .0f; //衰减二次项
};

