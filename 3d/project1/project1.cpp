#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

//const float PIXEL_SIZE = 0.0075f;
const float PIXEL_SIZE = 0.5f;

float RandomFloat(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}

struct Model {
	GLuint vao;
	GLuint vbo;
	GLsizei vertex_count;

	void draw() {
		glBindVertexArray(this->vao);
		glDrawArrays(GL_TRIANGLES, 0, this->vertex_count);
	}

	void cleanup() {
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
	}

	static Model load(string filename) {
		Model model;

		vector<GLfloat> vertices;

		// Load geometry from file
		{
			fstream file(filename);
			if (!file) {
				cout << "could not find model file\n";
				abort();
			}

			vector<glm::vec3> positions;
			vector<glm::vec2> texcoords;
			vector<glm::vec3> normals;

			string line;

			while (getline(file, line)) {
				istringstream line_stream(line);

				string type;
				line_stream >> type;

				if (type == "v") {
					GLfloat x, y, z;
					line_stream >> x;
					line_stream >> y;
					line_stream >> z;

					positions.push_back(glm::vec3(x, y, z));
				}else if (type == "vn") {
					GLfloat x, y, z;
					line_stream >> x;
					line_stream >> y;
					line_stream >> z;

					normals.push_back(glm::vec3(x, y, z));
				}else if(type == "vt"){
					GLfloat u, v;
					line_stream >> u;
					line_stream >> v;
					
					texcoords.push_back(glm::vec2(u, v));
				}else if(type == "f"){
					string face;
				
					line_stream >> face;
					replace(face.begin(), face.end(), '/', ' ');
					istringstream face_stream0(face);
						
					size_t position_index0, texcoord_index0, normal_index0;
					face_stream0 >> position_index0;
					face_stream0 >> texcoord_index0;
					face_stream0 >> normal_index0;
						
					glm::vec3 position0 = positions.at(position_index0-1);
					glm::vec2 texcoord0 = texcoords.at(texcoord_index0-1);
					glm::vec3 normal0 = normals.at(normal_index0 - 1);
					//
					//
					//
					line_stream >> face;
					replace(face.begin(), face.end(), '/', ' ');
					istringstream face_stream1(face);

					size_t position_index1, texcoord_index1, normal_index1;
					face_stream1 >> position_index1;
					face_stream1 >> texcoord_index1;
					face_stream1 >> normal_index1;

					glm::vec3 position1 = positions.at(position_index1 - 1);
					glm::vec2 texcoord1 = texcoords.at(texcoord_index1 - 1);
					glm::vec3 normal1 = normals.at(normal_index1 - 1);

					//
					//
					//
					line_stream >> face;
					replace(face.begin(), face.end(), '/', ' ');
					istringstream face_stream2(face);

					size_t position_index2, texcoord_index2, normal_index2;
					face_stream2 >> position_index2;
					face_stream2 >> texcoord_index2;
					face_stream2 >> normal_index2;

					glm::vec3 position2 = positions.at(position_index2 - 1);
					glm::vec2 texcoord2 = texcoords.at(texcoord_index2 - 1);
					glm::vec3 normal2 = normals.at(normal_index2 - 1);

					// calculate tangent and bitangent
					glm::vec3 edge0 = position1 - position0;
					glm::vec3 edge1 = position2 - position0;
					glm::vec2 delta_texcoords0 = texcoord1 - texcoord0;
					glm::vec2 delta_texcoords1 = texcoord2 - texcoord0;

					float f = 1.0f / (delta_texcoords0.x * delta_texcoords1.y - delta_texcoords1.x * delta_texcoords0.y);

					glm::vec3 tangent;
					tangent.x = f * (delta_texcoords1.y * edge0.x - delta_texcoords0.y * edge1.x);
					tangent.y = f * (delta_texcoords1.y * edge0.y - delta_texcoords0.y * edge1.y);
					tangent.z = f * (delta_texcoords1.y * edge0.z - delta_texcoords0.y * edge1.z);
					tangent = glm::normalize(tangent);


					glm::vec3 bitangent;
					bitangent.x = f * (-delta_texcoords1.x * edge0.x - delta_texcoords0.x * edge1.x);
					bitangent.y = f * (-delta_texcoords1.x * edge0.y - delta_texcoords0.x * edge1.y);
					bitangent.z = f * (-delta_texcoords1.x * edge0.z - delta_texcoords0.x * edge1.z);
					bitangent = glm::normalize(bitangent);


					//cout << "work";
					vertices.push_back(position0.x);
					vertices.push_back(position0.y);
					vertices.push_back(position0.z);
					vertices.push_back(texcoord0.x);
					vertices.push_back(texcoord0.y);
					//cout << "work1";
					vertices.push_back(normal0.x);
					vertices.push_back(normal0.y);
					vertices.push_back(normal0.z);
					//cout << "work2";
					vertices.push_back(tangent.x);
					vertices.push_back(tangent.y);
					vertices.push_back(tangent.z);
					vertices.push_back(bitangent.x);
					vertices.push_back(bitangent.y);
					vertices.push_back(bitangent.z);

					//cout << "work";
					vertices.push_back(position1.x);
					vertices.push_back(position1.y);
					vertices.push_back(position1.z);
					vertices.push_back(texcoord1.x);
					vertices.push_back(texcoord1.y);
					//cout << "work1";
					vertices.push_back(normal1.x);
					vertices.push_back(normal1.y);
					vertices.push_back(normal1.z);
					//cout << "work2";
					vertices.push_back(tangent.x);
					vertices.push_back(tangent.y);
					vertices.push_back(tangent.z);
					vertices.push_back(bitangent.x);
					vertices.push_back(bitangent.y);
					vertices.push_back(bitangent.z);

					//cout << "work";
					vertices.push_back(position2.x);
					vertices.push_back(position2.y);
					vertices.push_back(position2.z);
					vertices.push_back(texcoord2.x);
					vertices.push_back(texcoord2.y);
					//cout << "work1";
					vertices.push_back(normal2.x);
					vertices.push_back(normal2.y);
					vertices.push_back(normal2.z);
					//cout << "work2";
					vertices.push_back(tangent.x);
					vertices.push_back(tangent.y);
					vertices.push_back(tangent.z);
					vertices.push_back(bitangent.x);
					vertices.push_back(bitangent.y);
					vertices.push_back(bitangent.z);
				}

			}
		}

		// Send the vertex data to the GPU
		{
			// Generate the data on the CPU
			//float size = PIXEL_SIZE;

			model.vertex_count = vertices.size();

			// Use OpenGL to store it on the GPU
			{
				// Create a Vertex Buffer Object on the GPU
				glGenBuffers(1, &model.vbo);
				// Tell OpenGL that we want to set this as the current buffer
				glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
				// Copy all our data to the current buffer!
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			}
		}

		// Tell the GPU how to interpret our existing vertex data
		{
			// Create a Vertex Array Object to hold the settings
			glGenVertexArrays(1, &model.vao);

			// Tell OpenGL that we want to set this as the current vertex array
			glBindVertexArray(model.vao);

			// Tell OpenGL the settings for the current 0th vertex array!
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

			// Enable the 0th vertex attrib array!
			glEnableVertexAttribArray(0);

			GLsizei stride = 14 * sizeof(GLfloat);

			// Tell OpenGL about the position component
			glVertexAttribPointer(
				0, // index
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)0 // first component
			);
			glEnableVertexAttribArray(0);

			// Tell OpenGL about the UV component
			glVertexAttribPointer(
				1, // index
				2, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)(3 * sizeof(GLfloat)) // first component
			);

			glEnableVertexAttribArray(1);
			
			glVertexAttribPointer(
				2, // index
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)(5 * sizeof(GLfloat)) // first component
			);

			glEnableVertexAttribArray(2);

			glVertexAttribPointer(
				3, // index
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)(8 * sizeof(GLfloat)) // first component
			);

			glEnableVertexAttribArray(3);

			glVertexAttribPointer(
				4, // index
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)(11 * sizeof(GLfloat)) // first component
			);

			glEnableVertexAttribArray(4);
		}

		return model;
	}
};

struct Camera {
	glm::mat4 camera_from_world = glm::mat4(1);
	float fov_degrees = 60.0f;
	float near = 0.01f;
	float far = 1000.0f;

	glm::mat4 view_from_camera(GLFWwindow* window) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0.0f, 0.0f, width, height);
		return glm::perspective(
			glm::radians(this->fov_degrees),
			(float)width / (float)height, // aspect ratio
			this->near, // near
			this->far // far
		);
	}
};

struct Transform {
	glm::vec3 position;
	glm::vec3 scale;
};

struct Particle {
	bool moving = true;
	glm::mat4 world_from_model;
	glm::vec3 velocity;
	Model model;
	Transform transform;

	Particle(Model model, glm::vec3 position, glm::vec3 scale, glm::vec3 velocity) {
		this->transform.position = position;
		this->transform.scale = scale;
		this->model = model;
		//this->world_from_model = glm::scale(this->world_from_model, scale);
		this->world_from_model = glm::translate(glm::mat4(1), position);
		this->velocity = velocity;
	}
};

GLFWwindow* initialize_glfw() {
	GLFWwindow* window;

	/* Initialize the library */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640*2, 480*2, "Hello World", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	return window;
}

GLuint compile_shader() {
	// Define shader sourcecode
	const char* vertex_shader_src =
		"#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"layout (location = 1) in vec2 texcoords;\n"
		"layout (location = 2) in vec3 normal;\n"
		"layout (location = 3) in vec3 tangent;\n"
		"layout (location = 4) in vec3 bitangent;\n"
		"out vec2 Texcoords;\n"
		"out vec3 Normal;\n"
		"out vec3 world_space_position;\n"
		"out vec3 world_space_camera_position;\n"
		"out vec3 diffuse;\n"
		"out mat3 TBN;"
		"uniform vec2 offset;\n"
		"uniform mat4 camera_from_world;"
		"uniform mat4 view_from_camera;"
		"uniform mat4 world_from_model;"
		"void main() {\n"
		"	Texcoords = texcoords;\n"
		"	Normal = mat3(transpose(inverse(world_from_model))) * normal;\n"
		"	gl_Position = view_from_camera * camera_from_world * world_from_model * vec4(pos, 1.0);\n"
		"	world_space_position = vec3(world_from_model * vec4(pos, 1.0));\n"
		"	mat4 world_from_camera = inverse(camera_from_world);\n"
		"	world_space_camera_position = vec3(world_from_camera * vec4(0.0, 0.0, 0.0, 1.0));\n"
		"	vec3 T = normalize(vec3(world_from_model * vec4(tangent,0.0f)));\n"
		"	vec3 B = normalize(vec3(world_from_model * vec4(bitangent,0.0f)));\n"
		"	vec3 N = normalize(vec3(world_from_model * vec4(normal,0.0f)));\n"
		"	TBN = mat3(T, B, N);\n"
		"}\n";

	const char* fragment_shader_src =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 Texcoords;\n"
		"in vec3 Normal;\n"
		"in vec3 world_space_position;\n"
		"in vec3 world_space_camera_position;\n"
		"in vec3 diffuse;\n"
		"in mat3 TBN;\n"
		"uniform vec4 color;\n"
		"uniform sampler2D diffuse_map;\n"
		"uniform sampler2D specular_map;\n"
		"uniform sampler2D normal_map;\n"
		"uniform int usingTex;\n"
		"uniform vec3 lightPos;\n"
		"void main() {\n"
		"	// Light settings etc\n"
		"	vec3 light_dir = lightPos;\n"
		"	vec3 light_color = 0.75 * vec3(1.0, 1.0, 1.0);\n"
		"	vec3 specular_color = 0.1 * vec3(1.0, 1.0, 1.0);\n"
		"	vec3 normal = vec3(texture(normal_map, Texcoords));\n"
		"	normal = normal * 2.0 - 1.0;\n"
		"	normal = TBN * normal;\n"
		"	normal = normalize(normal);\n"

		"	// Ambient lighting\n"
		"	vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);\n"
	
		"	// Diffuse lighting\n"
		"	float diffuse_intensity = max(dot(normal, light_dir), 0.0); \n"
		"	vec3 diffuse_map_color = vec3(texture(diffuse_map, Texcoords));\n"
		//"	diffuse_map_color += vec3(texture(specular_map, Texcoords));\n"
		"	vec3 diffuse = diffuse_map_color * diffuse_intensity * light_color;\n"
		"\n"
		"	// Specular lighting\n"
		"	vec3 view_direction = normalize(world_space_camera_position - world_space_position);\n"
		"	vec3 reflect_direction = reflect(-light_dir, normal);\n"
		"	float specular_intensity = pow(max(dot(view_direction, reflect_direction), 0.0), 1.5);\n"
		"	specular_intensity *= (1.0 - texture(specular_map, Texcoords).r);\n"
		"	vec3 specular = specular_intensity * specular_color;\n"
		"\n"
		"	// Final output\n"
		//"	FragColor = vec4(normal, 1.0); \n"
		//"	FragColor = vec4(texture(diffuse_map, Texcoords).xyz, 1.0);\n"
		"   FragColor = vec4(ambient + diffuse + specular, 1.0);\n"
		"}\n";

	// Define some vars
	const int MAX_ERROR_LEN = 512;
	GLint success;
	GLchar infoLog[MAX_ERROR_LEN];

	// Compile the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	// Check for errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, MAX_ERROR_LEN, NULL, infoLog);
		std::cout << "vertex shader compilation failed:\n" << infoLog << std::endl;
		abort();
	}

	// Compile the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);

	// Check for errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, MAX_ERROR_LEN, NULL, infoLog);
		std::cout << "fragment shader compilation failed:\n" << infoLog << std::endl;
		abort();
	}

	// Link the shaders
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Check for errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, MAX_ERROR_LEN, NULL, infoLog);
		cout << "shader linker failed:\n" << infoLog << endl;
		abort();
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Enable the shader here since we only have one
	glUseProgram(shader_program);

	return shader_program;
}



GLuint load_textures(GLenum active_texture, const char *filename) {
	// OpenGL has multiple slots for textures, lets work with slot 0
	glActiveTexture(active_texture);

	// Create a new texture on the GPU
	GLuint tex;
	glGenTextures(1, &tex);

	// Bind our texture as a 2D texture
	glBindTexture(GL_TEXTURE_2D, tex);

	// Tell OpenGL how to scale the texture. We’re going to use “nearest” for now which pixelates it:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// We’re also going to tell OpenGL to repeat the texture infinitely both horizontally and
	// vertically
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/*
	GLsizei width = 2;
	GLsizei height = 2;
	float pixels[] = {
	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f, // r, g, b,   r, g, b
	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f, // r, g, b,   r, g, b
	};
	*/
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		cout << nrChannels << endl;
		if (nrChannels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);

	return tex;
}

void render_scene(GLuint& shader_program, GLFWwindow* window, vector<Particle> particles, Camera camera) {
	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint color_location = glGetUniformLocation(shader_program, "color");
	GLint offset_location = glGetUniformLocation(shader_program, "offset");
	GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");

	glUniform4f(color_location, 1.0f, 1.0f, 1.0f, 1.0f);

	GLint diffuse_map_location = glGetUniformLocation(shader_program, "diffuse_map");
	glUniform1i(diffuse_map_location, 0);

	GLint specular_map_location = glGetUniformLocation(shader_program, "specular_map");
	glUniform1i(specular_map_location, 1);

	GLint normal_map_location = glGetUniformLocation(shader_program, "normal_map");
	glUniform1i(normal_map_location, 2);

	glUniformMatrix4fv(
		world_to_camera_location,
		1, // count
		GL_FALSE, // transpose
		glm::value_ptr(camera.camera_from_world)
	);

	GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
	glUniformMatrix4fv(
		view_from_camera_location,
		1, // count
		GL_FALSE, // transpose
		glm::value_ptr(camera.view_from_camera(window))
	);

	for (int i = 0; i < particles.size(); i++) {

		particles[i].world_from_model = glm::translate(glm::mat4(1), particles[i].transform.position);

		GLint world_from_model_location = glGetUniformLocation(shader_program, "world_from_model");
		glUniformMatrix4fv(
			world_from_model_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(particles[i].world_from_model)
		);
	
		particles[i].model.draw();
	}

	/*
	glUniformMatrix4fv(
		world_to_camera_location,
		1, // count
		GL_FALSE, // transpose
		glm::value_ptr(camera_from_world)
	);
	*/
	// Display the results on screen
	glfwSwapBuffers(window);
}

void cleanup(GLFWwindow* window, GLuint shader_program,vector<Model> models, vector<GLuint> *textures) {
	glDeleteTextures(textures->size(), &(*textures)[0]);

	glDeleteProgram(shader_program);
	for (Model model : models) {
		model.cleanup();
	}

	glfwTerminate();
}

int main(void) {
	GLFWwindow* window = initialize_glfw();
	GLuint shader_program = compile_shader();

	//float light_x = 1.0;
	vector<float> lightPos;
	lightPos.push_back(1.0);
	lightPos.push_back(1.0);
	lightPos.push_back(1.0);

	int usingTex = 0;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<Model> models;
	models.push_back(Model::load(string("normals/cube-normals.obj")));
	models.push_back(Model::load(string("normals/monkey-normals.obj")));
	
	vector<Particle> particles;
	particles.push_back(Particle(models[0], glm::vec3(0.0f, 2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.02f), 0.0f)));
	particles.push_back(Particle(models[0], glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.02f), 0.0f)));
	particles.push_back(Particle(models[0], glm::vec3(3.0f, 2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.02f), 0.0f)));
	particles.push_back(Particle(models[0], glm::vec3(3.0f, -2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.02f), 0.0f)));

	vector<GLuint> textures;
	textures.push_back(load_textures(GL_TEXTURE0, "./Rock/RockColor.jpg"));
	textures.push_back(load_textures(GL_TEXTURE1, "./Rock/RockRoughness.jpg"));
	textures.push_back(load_textures(GL_TEXTURE2, "./Rock/RockNormal.jpg"));

	Camera camera;
	camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 0.0f, -5.0f));
	//glm::mat4 world_from_model = glm::mat4(1.0); // init to the identity matrix

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_D)) {
			if (lightPos[0] < 1.0) {
				lightPos[0] += 0.005;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			if (lightPos[0] > -1.0) {
				lightPos[0] -= 0.005;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_W)) {
			if (lightPos[1] < 1.0) {
				lightPos[1] += 0.005;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			if (lightPos[1] > -1.0) {
				lightPos[1] -= 0.005;
			}
		}

		GLint light_location = glGetUniformLocation(shader_program, "lightPos");
		glUniform3f(light_location, lightPos[0], lightPos[1], lightPos[2]);

		//camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 0.0f, 0.0001f));
		//camera.camera_from_world = glm::rotate(camera.camera_from_world, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
		//world_from_model = glm::rotate(world_from_model, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));

		for (int i = 0; i < particles.size(); i++) {
			if (!particles[i].moving) {
				continue;
			}
			if (particles[i].transform.position.y <= -4.0f) {
				particles[i].moving = false;
				continue;
			}

			glm::vec3 newPos = particles[i].transform.position + particles[i].velocity;
			bool collide = false;
			
			for (int j = 0; j < particles.size(); j++) {
				if (j == i) {
					continue;
				}

				glm::vec3 a = particles[i].transform.position;
				glm::vec3 b = particles[j].transform.position;

				float dist = sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0) + pow(a.z - b.z, 2.0));
				cout << dist << endl;
				if (dist < 2.0f) {
					collide = true;
					break;
				}
			}

			if (!collide) {
				particles[i].transform.position = newPos;
			}
			else {
				particles[i].moving = false;
			}
			//particles[i].world_from_model = glm::translate(glm::mat4(1), particles[i].transform.position);
			//particles[i].world_from_model = glm::translate(particles[i].world_from_model, particles[i].velocity);
		}

		render_scene(shader_program, window, particles, camera);
		glfwPollEvents();
	}

	cleanup(window, shader_program, models, &textures);
	return 0;
}

