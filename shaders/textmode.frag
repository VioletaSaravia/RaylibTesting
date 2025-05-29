#version 460 core

out vec4 FragColor;

in vec2 vUV;

uniform sampler2D tex0;

void main() {
    FragColor = texture(tex0, vUV);
}
