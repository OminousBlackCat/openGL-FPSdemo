#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;


void main(){
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
}