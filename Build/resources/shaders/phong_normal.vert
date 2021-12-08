#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texcoord;
layout(location = 2)in vec3 tangent;

out VS_OUT
{
	out vec3 fs_position;
	out vec3 fs_normal;
	out vec2 fs_texcoord;
	out mat3 fs_tbn;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 model_view = view * model;
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vs_out.fs_normal = normalize(mat3(model_view) * normal);
	vs_out.fs_position = vec3(model_view * vec4(position, 1));
	vs_out.fs_texcoord = texcoord;

	vec3 N = normalize(normal_matrix * normal);
	vec3 T = normalize(normal_matrix * tangent);
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N, T));
	vs_out.fs_tbn = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(position, 1.0);
}