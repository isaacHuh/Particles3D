#version 330 core


out vec4 FragColor;
in vec2 Texcoords;
uniform sampler2D tex;
uniform float fade;

void main() {
	FragColor = texture(tex, Texcoords) * fade;
}
