#version 460 core

layout (location = 0) in vec2 glyph_position_vertex;
layout (location = 1) in vec2 glyph_texture_coords_vertex;
layout (location = 2) in vec2 glyph_foreground_coords_vertex;

out vec2 glyph_texture_coords;
out vec2 glyph_foreground_texture_coords;
out float visibility;

const float fog_density = 0.005;
const float fog_gradient = 1.5;

uniform mat4 view;
uniform mat4 model;
uniform mat4 mvp;
uniform vec3 sky_color;

void main(){
    glyph_texture_coords = glyph_texture_coords_vertex;
    glyph_foreground_texture_coords = glyph_foreground_coords_vertex;
    float distance = length(vec3(view * model * vec4(glyph_position_vertex, 0.0, 1.0)));
	visibility = clamp(exp(-pow(distance * fog_density, fog_gradient)), 0.0, 1.0);
    gl_Position = mvp * vec4(glyph_position_vertex, 0.0, 1.0);
}
