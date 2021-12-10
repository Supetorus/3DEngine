#version 430 core
in VS_OUT
{
	vec3 position;
	vec3 light_position;
	vec2 texcoord;
} fs_in;

out vec4 outColor;
	
struct Material
{
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
//	generate the normals from the normal map
	vec3 normal = texture(normal_sample, fs_in.texcoord).rgb;
//	 convert rgb (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize(normal * 2.0 - 1.0);

//	Ambient
	vec3 ambient = light.ambient;

//	 Diffuse
	vec3 light_dir = normalize(vec3(fs_in.light_position) - fs_in.position);
	float intensity = max(dot(light_dir, normal), 0);
	vec3 diffuse = material.diffuse * light.diffuse * intensity;

//	Specular
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		vec3 view_dir = normalize(-vec3(fs_in.position));
		vec3 reflection = reflect(-light_dir, normal);
		intensity = max(dot(view_dir, reflection), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * light.specular * intensity;
	}

	outColor = vec4(ambient + diffuse, 1) * texture(color_sample, fs_in.texcoord) + vec4(specular, 1);
}