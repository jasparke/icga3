#version 330 core
in vec3 pos;

out vec3 uvw;

uniform mat4 VIEW;
uniform mat4 PROJ;

void main() {
    uvw = -pos;
    gl_Position = PROJ * VIEW * vec4(pos, 1.0);
}
