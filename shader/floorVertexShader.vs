#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoor;

out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;


void main(){
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
    texCoords = texCoor;
}