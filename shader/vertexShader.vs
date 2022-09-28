#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoor;
layout(location = 2) in vec3 normal;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

out vec2 texCoord;
out vec3 outNormal;
out vec3 fragPos;

void main(){
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0f);
    texCoord = texCoor;
    outNormal = normal;
    fragPos = vec3(modelMat * vec4(position, 1.0f));
}