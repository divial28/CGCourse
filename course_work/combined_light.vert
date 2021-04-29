#version 330 core

precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 frontLights;
layout (location = 4) in vec3 backLights;
layout (location = 5) in mat4 instanceMatrix;

out vec3 Pos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FrontLights;
out vec3 BackLights;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0); 
    TexCoords = aTexCoords;
    Normal = normal;
    FrontLights = frontLights;
    BackLights = backLights;
}