#version 330 core
out vec3 color;
in vec3 uvw;

uniform samplerCube skybox;

void main() {
    color = texture(skybox, uvw).rgb;
}
