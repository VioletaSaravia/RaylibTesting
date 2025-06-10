#version 460 core

out vec4 FragColor;

in vec2 vUV;
in vec4 vColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

void main() {
    FragColor = texture(texture0, vUV) /* * vColor */ * colDiffuse;
}
