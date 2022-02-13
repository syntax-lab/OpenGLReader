#version 460 core
//Phong lighting model

in VS_OUT{
	vec3 fragment_position;
	vec3 normal;
	vec4 fragment_position_light_space;
} fs_in;

in float visibility;

out vec4 color;

uniform sampler2D shadow_map;

uniform vec3 light_position;
uniform vec3 view_position;
uniform vec3 sky_color;

float shadow_calculation(vec4 fragment_position_light_space){
	//NOTE: useless if we use orthogonal projection
	vec3 projection_coords = fragment_position_light_space.xyz / fragment_position_light_space.w;

	projection_coords = projection_coords * 0.5 + 0.5;

	float closest_depth = texture(shadow_map, projection_coords.xy).r;

	float current_depth = projection_coords.z;

	vec3 normal = normalize(fs_in.normal);
	vec3 light_direction = normalize(light_position - fs_in.fragment_position);
	
	//NOTE: other bias values (0.03, 0.01)
	float bias = max(0.002 * (1.0 - dot(normal, light_direction)), 0.0005);

	float shadow = 0.0;
	vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcf_depth = texture(shadow_map, projection_coords.xy + vec2(x, y) * texel_size).r;
			shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if(projection_coords.z > 1.0){
		shadow = 0.0;
	}

	return shadow;
}

void main(){
	//vec3 object_color = vec3(0.5, 0.5, 0.5);
	vec3 object_color = vec3(1.0, 0.99, 0.81);
	vec3 normal = normalize(fs_in.normal);
	vec3 light_color = vec3(1, 1, 1);

	vec3 ambient = 0.3 * object_color;

	vec3 light_direction = normalize(light_position - fs_in.fragment_position);
	float diff = max(dot(light_direction, normal), 0.0);
	vec3 diffuse = diff * light_color;

	vec3 view_direction = normalize(view_position - fs_in.fragment_position);
	vec3 reflect_direction = reflect(-light_direction, normal);
	float spec = 0.0;
	vec3 halfway_direction = normalize(light_direction + view_direction);
	spec = pow(max(dot(normal, halfway_direction), 0.0), 64.0);
	vec3 specular = spec * light_color;

	float shadow = shadow_calculation(fs_in.fragment_position_light_space);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * object_color;

	vec4 out_color = vec4(lighting, 1.0);

	color = mix(vec4(sky_color, 1.0), out_color, visibility);
}
