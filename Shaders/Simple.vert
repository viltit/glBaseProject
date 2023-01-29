#version 400 core

layout (location = 0) in vec3 v_pos;    // vertex position
layout (location = 1) in vec4 v_color;  // vertex color
layout (location = 2) in mat4 T;        // instance position matrix
layout (location = 6) in vec4 color; 

out vec4 fragColor; 
uniform mat4 C;     // camera matrix
uniform mat4 M;     // model matrix

void main() 
{ 
    fragColor = color; 
    gl_Position = C * M * T * vec4(v_pos, 1.0); 
}