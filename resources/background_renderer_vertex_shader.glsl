#version 460 core
layout(location = 0) in vec3 position_vertex;
layout(location = 1) in vec3 normal_vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
uniform mat4 light_space_matrix;
uniform vec3 view_position;

out VS_OUT{
	vec3 fragment_position;
	vec3 normal;
	vec4 fragment_position_light_space;
} vs_out;

out float visibility;

const float fog_density = 0.005;
const float fog_gradient = 1.5;

void main(){
	vs_out.fragment_position = vec3(model * vec4(position_vertex, 1.0));
	vs_out.normal = transpose(inverse(mat3(model))) * normal_vertex; //TODO(SyntaX): should be done on CPU side
	vs_out.fragment_position_light_space = light_space_matrix * vec4(vs_out.fragment_position, 1.0);
	float distance = length(vec3(view * model * vec4(position_vertex, 1.0)));
	visibility = clamp(exp(-pow(distance * fog_density, fog_gradient)), 0.0, 1.0);
	gl_Position = mvp * vec4(position_vertex, 1.0);
}
