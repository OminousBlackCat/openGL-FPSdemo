#version 330 core

in vec2 texCoord;
in vec3 outNormal;
in vec3 fragPos;

out vec4 color;

void main(){
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}