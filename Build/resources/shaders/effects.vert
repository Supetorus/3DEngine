#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texcoord;

out VS_OUT
{
	out vec3 fs_position;
	out vec3 fs_normal;
	out vec2 fs_texcoord;
} vs_out;

struct UV
{
	vec2 tiling;
	vec2 offset;
};


uniform UV uv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float strength;
uniform float radius;
uniform float time;

float random(float n) // 0 - 1
{
	return fract(sin(n) * 43758.5453123);
}

float noise(float p)
{
	float fl = floor(p);
	float fc = fract(p);
	return mix(random(fl), random(fl + 1.0), fc);
}

void main()
{
	mat4 model_view = view * model;
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vec3 tposition = position;

	// Random
//	tposition = tposition * random(time + tposition.x);
	
	// Sine Wave
//	tposition = tposition * (sin(time * 5 + tposition.y) + 2) * 0.2;

	// Noise
	//tposition = tposition * (noise(time + tposition.x * 3) + 1);

	// bloat
	vec3 origin = vec3(0, 0, 0.5);
	vec3 dir = tposition - origin;
	float l = length(dir);
	float a = (1.0 - (min(l, radius) / radius));
	float f = mix(0.0, strength, a);
	tposition = tposition + (normalize(dir) * f);

	vs_out.fs_normal = normalize(mat3(model_view) * normal);
	vs_out.fs_position = vec3(model_view * vec4(tposition, 1));
	vs_out.fs_texcoord = (texcoord * uv.tiling) + uv.offset;

	gl_Position = projection * view * model * vec4(tposition, 1.0);
}