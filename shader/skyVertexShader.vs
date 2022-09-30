#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 projectionMat;
uniform mat4 viewMat;

void main(){
    gl_Position = projectionMat * viewMat * vec4(position, 1.0);
    TexCoords = position;
}
