#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

void main() {
    gl_Position = vec4(a_Pos, 1.0f);
}


#type fragment
#version 330 core

out vec4 color;

void main() {
    color = vec4(0.8f, 0.2f, 0.3f, 1.0f);
}
