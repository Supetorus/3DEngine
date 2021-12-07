#version 430 core
in VS_OUT
{
	vec3 fs_position;
	vec3 fs_normal;
	vec2 fs_texcoord;
} fs_in;

out vec4 outColor;
	
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 tint;
layout (binding = 0) uniform sampler2D color_sample;
layout (binding = 1) uniform sampler2D normal_sample;

void main()
{
	// Ambient
	vec3 ambient = material.ambient * light.ambient;

	// Diffuse
	vec3 light_dir = normalize(vec3(light.position) - fs_in.fs_position);

	float intensity = max(dot(light_dir, fs_in.fs_normal), 0);

	vec3 diffuse = material.diffuse * light.diffuse * intensity;

	// Specular
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		vec3 view_dir = normalize(-vec3(fs_in.fs_position));
		vec3 reflection = reflect(-light_dir, fs_in.fs_normal);
		intensity = max(dot(view_dir, reflection), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * light.specular * intensity;
	}

	//fs_color = ambient + diffuse + specular;
	//outColor = texture(textureSampler, fs_texcoord) * vec4(fs_color, 1);
	outColor = vec4(ambient + diffuse, 1) * texture(color_sample, fs_in.fs_texcoord) + vec4(specular, 1);
}