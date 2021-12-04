#version 430 core
flat in vec3 fs_color;
in vec2 fs_texcoord;

out vec4 outColor;
	
uniform vec3 tint;
uniform sampler2D textureSampler;

void main()
{
	outColor = texture(textureSampler, fs_texcoord) * vec4(fs_color, 1);
}