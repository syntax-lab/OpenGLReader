#version 460 core

in float visibility;
in vec2 glyph_texture_coords;
in vec2 glyph_foreground_texture_coords;
out vec4 color;

uniform sampler2D glyph_texture;
uniform sampler2D glyph_foreground;
uniform vec3 glyph_color;
uniform vec3 sky_color;

void main(){
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(glyph_texture, glyph_texture_coords).r);
    //if(texture(glyph_texture, glyph_texture_coords).r == 1.0){
    //    sampled = vec4(1, 1, 1, 1);
    //}else{
    //    sampled = vec4(0, 0, 0, 0);
    //}
    vec4 out_color = sampled * vec4(glyph_color, 1.0);
    color = mix(vec4(sky_color, 1.0), out_color, visibility);
    //color = texture(glyph_foreground, glyph_foreground_texture_coords) * sampled;
}
