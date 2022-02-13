#version 460 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coords;

out vec2 texture_coords_out;

void main(){
	texture_coords_out = texture_coords;
	gl_Position = vec4(vertex_position, 1.0);
}
