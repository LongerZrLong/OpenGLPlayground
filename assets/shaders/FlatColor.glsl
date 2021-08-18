#type vertex
#version 330 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(a_Pos, 1.0f);
}


#type fragment
#version 330 core

out vec4 color;

void main() {
    color = vec4(0.8f, 0.2f, 0.3f, 1.0f);
}
