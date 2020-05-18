#version 330 core
out vec4 FragColor;
in vec2 Texcoords;
in vec3 Normal;
in vec3 world_space_position;
in vec3 world_space_camera_position;
in vec3 diffuse;
in mat3 TBN;
uniform vec4 color;
uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D normal_map;
uniform samplerCube skybox;
uniform int shiny;
uniform vec3 lightPos;
void main() {
	vec3 diffuse_map_color = vec3(texture(diffuse_map, Texcoords));

	// Light settings etc
	vec3 ambient = diffuse_map_color * 2.0 * vec3(0.18, 0.2, 0.18);

	vec3 light_dir = lightPos;
	vec3 light_color = vec3(1.0) - ambient * 0.5;

	vec3 specular_color = 0.4 * vec3(1.0, 1.0, 1.0);
	vec3 normal = vec3(texture(normal_map, Texcoords));
	normal = normal * 2.0 - 1.0;
	normal = TBN * normal;
	normal = normalize(normal);

	// Diffuse lighting
	float diffuse_intensity = max(dot(normal, light_dir), 0.1); 
	vec3 diffuse = diffuse_map_color * diffuse_intensity * light_color;

	// Specular lighting
	vec3 view_direction = normalize(world_space_camera_position - world_space_position);
	vec3 reflect_direction = reflect(-light_dir, normal);
	float specular_intensity = pow(max(dot(view_direction, reflect_direction), 0.0), 0.5);
	specular_intensity *= (1.0 - texture(specular_map, Texcoords).r);
	vec3 specular = specular_intensity * specular_color;

	// Final output
	if(shiny == 1){
		//Reflection
		/*
		vec3 I = normalize(world_space_position - world_space_camera_position);
		vec3 R = reflect(I, normal);
		vec3 reflection = texture(skybox, R).xyz;
		FragColor = vec4(ambient + diffuse + (specular * reflection), 1.0);
		*/

		vec3 I = normalize(world_space_position - world_space_camera_position);
		vec3 Refract = refract(I, normal, 1.0/1.52);
		vec3 Reflect = reflect(I, normal);
		vec3 reflection = texture(skybox, Reflect).xyz;
		vec3 refraction = texture(skybox, Refract).xyz;

		FragColor = vec4(mix(reflection,refraction,0.5), 1.0 );
	}else{
		FragColor = vec4(ambient + diffuse + specular, 1.0);
   }
}