#version 330 core


out vec4 FragColor;

in vec2 Texcoords;

uniform sampler2D tex;
uniform float fade;

void main() {
	FragColor = vec4(texture(tex, Texcoords).rgb, 1.0) * fade;
}
