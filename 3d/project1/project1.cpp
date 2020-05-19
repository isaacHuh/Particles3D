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
	bool render = true;
	bool moving = true;
	glm::mat4 world_from_model;
	glm::vec3 velocity;
	Model model;
	Transform transform;
	string type = "sand";

	Particle(Model model, glm::vec3 position, glm::vec3 scale, glm::vec3 velocity, string type) {
		this->transform.position = position;
		this->transform.scale = scale;
		this->model = model;
		//this->world_from_model = glm::scale(this->world_from_model, scale);
		this->world_from_model = glm::translate(glm::mat4(1), position);
		this->velocity = velocity;
		this->type = type;
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
	window = glfwCreateWindow(640*2, 480*2, "Particles 3D", NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	return window;
}

GLuint compile_shader(const char *vertex_filename, const char *fragment_filename) {
	// Define shader sourcecode
	ifstream fragment_shader_file(fragment_filename);
	stringstream fragment_shader_stream;
	fragment_shader_stream << fragment_shader_file.rdbuf();
	string fragment_shader_string = fragment_shader_stream.str();
	const char* fragment_shader_c_string = fragment_shader_string.c_str();

	ifstream vertex_shader_file(vertex_filename);
	stringstream vertex_shader_stream;
	vertex_shader_stream << vertex_shader_file.rdbuf();
	string vertex_shader_string = vertex_shader_stream.str();
	const char* vertex_shader_c_string = vertex_shader_string.c_str();

	// Define some vars
	const int MAX_ERROR_LEN = 512;
	GLint success;
	GLchar infoLog[MAX_ERROR_LEN];

	// Compile the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_c_string, NULL);
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
	glShaderSource(fragment_shader, 1, &fragment_shader_c_string, NULL);
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


GLuint load_cubemap(GLenum active_texture, 
	const char* left,
	const char* front,
	const char* right,
	const char* back,
	const char* top,
	const char* bottom) {
	// OpenGL has multiple slots for textures, lets work with slot 0
	glActiveTexture(active_texture);

	// Create a new texture on the GPU
	GLuint tex;
	glGenTextures(1, &tex);

	// Bind our texture as a 2D texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

	
	// We’re also going to tell OpenGL to wrap the texture infinitely both horizontally and
	// vertically
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(right, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(left, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(top, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(bottom, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(front, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	{
		int width = 0, height = 0;
		GLubyte* pixels = stbi_load(back, &width, &height, NULL, 3);
		if (pixels == NULL || width == 0 || height == 0) {
			abort();
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);

	return tex;
}

Particle* object_at_point(vector<Particle>& particles, glm::vec3 pos) {
	//maybe use distance
	for (int i = 0; i < particles.size(); i++) {
		if (particles[i].type == "smoke") {
			continue;
		}

		if (particles[i].transform.position == pos) {
			return &particles[i];
		}
	}
	return NULL;
}

float point_distance(glm::vec3 a, glm::vec3 b) {
	return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0) + pow(a.z - b.z, 2.0));
}

void swap_positions(Particle &p1, Particle &p2) {
	glm::vec3 temp = p1.transform.position;
	p1.transform.position = p2.transform.position;
	p2.transform.position = temp;

	p1.render = true;
	p2.render = true;
	cout << "swapped\n";
}

int findPlace(vector<Particle>& particles, Particle& p) {
	for (int i = 0; i < particles.size(); i++) {
		if (&particles[i] == &p) {
			return i;
			break;
		}
	}
	return -1;
}

void remove_values(vector<Particle>& particles, Particle& particle1, Particle& particle2) {

	glm::vec3 spawnPos = particle1.transform.position;
	Model model = particle1.model;

	int p1 = 0, p2 = 0;
	
	vector<Particle>::iterator it;

	//get pos of particle1
	p1 = findPlace(particles, particle1);

	//get pos of particle2
	p2 = findPlace(particles, particle2);
	
	//delete &particles[p1];
	//delete &particles[p2];

	if (p1 > p2) {
		particles.erase(particles.begin() + p1);
		particles.erase(particles.begin() + p2);
	}
	else {
		particles.erase(particles.begin() + p2);
		particles.erase(particles.begin() + p1);
	}

	for (int i = 0; i < 3; i++) {
		particles.push_back(Particle(model,
			spawnPos,
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(RandomFloat(-2.0f, 2.0f), RandomFloat(0.5f,2.0f), RandomFloat(-2.0f, 2.0f)),
			"smoke"));
	}

	//p1.render = true;
	//p2.render = true;

	cout << "removed\n";
}

void print(vector<Particle> &vector)
{
	for (auto i : vector) {
		std::cout << & i << ' ';
	}

	std::cout << '\n';
}

int particles_move_collision(vector<Particle> &particles, vector<int> &movePos, vector<int> &checkPos) {
	//print(particles);
	//cout << "check: ";
	//print(particlesCheck);

	int bound = 6;
	int count = 0;
	for (int i : movePos) {
		//cout << "size: " << particles.size() << " i: " << i << endl;
		bool returning = false;
		//don't do anything with particles that are at the bottom or not rendering
		if (particles[i].type == "smoke") {
			particles[i].transform.position += particles[i].velocity;
			

			if (abs(particles[i].transform.position.x) >= 6) {
				particles[i].transform.position.x -= particles[i].velocity.x;
				particles[i].velocity.x = 0;
			}
				
			if (abs(particles[i].transform.position.z) >= 6) {
				particles[i].transform.position.z -= particles[i].velocity.z;
				particles[i].velocity.z = 0;
			}
			continue;
		}
		
		if (particles[i].transform.position.y <= -16.0f || !particles[i].render) {
			//cout << "cont\n";
			//particles[i].moving = false;
			continue;
		}
		count++;

		glm::vec3 newPos = particles[i].transform.position + particles[i].velocity;
		bool collide = false;


		for (int j : checkPos) {
			//cout << &(particles)+j <<" check: " << &(particlesCheck[j]);
			if (&particles[j] == &particles[i] || particles[j].type == "smoke") {
				//cout << "match";
				continue;
			}

			glm::vec3 a = newPos;
			glm::vec3 b = particles[j].transform.position;

			float dist = point_distance(a,b);
			//cout << dist << endl;
			if (dist < 2.0f) {
				collide = true;

				glm::vec3 newPosPlus = particles[i].transform.position;

				if (particles[i].type == "sand") {
					vector<glm::vec3> posAdd;
					posAdd.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(-2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, 2.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, -2.0f));

					//go down pyramid if sand and swap if collide with water
					for(int k = 0; k < posAdd.size(); k++){
						Particle* obj = object_at_point(particles, newPos + posAdd[k]);
						if (!obj) {
							newPosPlus = newPos + posAdd[k];
							break;
						}
						else if ((*obj).type == "water" || (*obj).type == "lava") {
							//swap positions if collide with water

							swap_positions(particles[i], *obj);
							newPosPlus = particles[i].transform.position;
							break;
						}
					}
				}
				else if(particles[i].type == "water" || particles[i].type == "lava"){
					//bool returning = false;
					//go down normally
					vector<glm::vec3> posAdd;
					posAdd.push_back(glm::vec3(-2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, 2.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, -2.0f));

					//go down pyramid 
					for (int k = 0; k < posAdd.size(); k++) {
						Particle* obj = object_at_point(particles, newPos + posAdd[k]);
						if (!obj) {
							newPosPlus = newPos + posAdd[k];
							break;
						}
						else {
							if ((particles[i].type == "water" && obj->type == "lava") || (particles[i].type == "lava" && obj->type == "water")) {
								remove_values(particles, particles[i], *obj);
								returning = true;
								break;
							}
						}
					}
					if (returning) { return 1; }

					if (newPosPlus == particles[i].transform.position || abs(newPosPlus.x) > bound || abs(newPosPlus.z) > bound) {
						//additional checks if water
						//check direction at random and spreads
						posAdd.clear();
						posAdd.push_back(glm::vec3(-2.0f, 2.0f, 0.0f));
						posAdd.push_back(glm::vec3(2.0f, 2.0f, 0.0f));
						posAdd.push_back(glm::vec3(0.0f, 2.0f, -2.0f));
						posAdd.push_back(glm::vec3(0.0f, 2.0f, 2.0f));

						int dir = (int)RandomFloat(0.0f, 4.0f);
						if (dir == 4) { dir--; }

						Particle* obj = object_at_point(particles, newPos + posAdd[dir]);
						if (!obj) {
							newPosPlus = newPos + posAdd[dir];
						}
						else {
							if ((particles[i].type == "water" && obj->type == "lava") || (particles[i].type == "lava" && obj->type == "water")) {
								remove_values(particles, particles[i], *obj);
								returning = true;
								//break;
							}
						}
					}
					if (returning) { return 1; }
				}

				if (abs(newPosPlus.x) <= bound && abs(newPosPlus.z) <= bound && newPosPlus != particles[i].transform.position) {
					Particle *bottomObj = object_at_point(particles, particles[i].transform.position + glm::vec3(0.0f,-2.0f,0.0f));
					if (bottomObj) { (*bottomObj).render = true; }

					particles[i].transform.position = newPosPlus;
					particles[i].render = true;
				}
				else {
					//if(newPosPlus != particles[i].transform.position)

					//determine whether to render
					//check all sides to see if any is exposed

					bool render = false;
					vector<glm::vec3> posAdd;
					posAdd.push_back(glm::vec3(-2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
					posAdd.push_back(glm::vec3(0.0f, -2.0f, 0.0f));
					posAdd.push_back(glm::vec3(0.0f, 2.0f, 0.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, -2.0f));
					posAdd.push_back(glm::vec3(0.0f, 0.0f, 2.0f));

					for (int k = 0; k < posAdd.size(); k++) {
						Particle* obj = object_at_point(particles, particles[i].transform.position + posAdd[k]);
						if (!obj) {
							render = true;
							break;
						}
					}

					particles[i].render = render;
				}
				break;
			}
		}
		//if (returning) { ; }

		if (!collide) {
			particles[i].transform.position = newPos;
		}

		//particles[i].moving = false;
		//particles[i].world_from_model = glm::translate(glm::mat4(1), particles[i].transform.position);
		//particles[i].world_from_model = glm::translate(particles[i].world_from_model, particles[i].velocity);
	}

	//cout << "p: " << particles.size() << " check: " << count << endl;
}

void particles_move(vector<Particle>& particles) {
	for (int i = 0; i < particles.size(); i++) {
		particles[i].transform.position += particles[i].velocity;
	}
}

void render_scene(GLuint& shader_program,
	GLuint& sky_shader_program,
	GLFWwindow* window, 
	Particle cubemap,
	vector<Particle>& particles, 
	Camera camera) {
	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_FALSE);

	{
		glUseProgram(sky_shader_program);

		GLint offset_location = glGetUniformLocation(sky_shader_program, "offset");
		GLint camera_from_world_location = glGetUniformLocation(sky_shader_program, "camera_from_world");

		GLint skybox_location = glGetUniformLocation(sky_shader_program, "skybox");
		glUniform1i(skybox_location, 0);

		glm::mat4 camera_from_world_no_translation = glm::mat4(glm::mat3(camera.camera_from_world));
		glUniformMatrix4fv(
			camera_from_world_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(camera_from_world_no_translation)
		);

		GLint view_from_camera_location = glGetUniformLocation(sky_shader_program, "view_from_camera");
		glUniformMatrix4fv(
			view_from_camera_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(camera.view_from_camera(window))
		);

		GLint world_from_model_location = glGetUniformLocation(sky_shader_program, "world_from_model");
		glUniformMatrix4fv(
			world_from_model_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(cubemap.world_from_model)
		);

		cubemap.model.draw();
	}

	glDepthMask(GL_TRUE);

	// Enable the shader here since we only have one
	glUseProgram(shader_program);

	GLint color_location = glGetUniformLocation(shader_program, "color");
	GLint shiny_location = glGetUniformLocation(shader_program, "shiny");
	GLint offset_location = glGetUniformLocation(shader_program, "offset");
	GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");

	glUniform4f(color_location, 1.0f, 1.0f, 1.0f, 1.0f);

	GLint diffuse_map_location = glGetUniformLocation(shader_program, "diffuse_map");
	glUniform1i(diffuse_map_location, 0);

	GLint specular_map_location = glGetUniformLocation(shader_program, "specular_map");
	glUniform1i(specular_map_location, 1);

	GLint normal_map_location = glGetUniformLocation(shader_program, "normal_map");
	glUniform1i(normal_map_location, 2);

	GLint skybox_location = glGetUniformLocation(shader_program, "skybox");
	glUniform1i(skybox_location, 0);

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
		if (particles[i].type == "smoke" && particles[i].transform.position[1] > 20.0f) {
			//delete &particles[i];
			particles.erase(particles.begin() + i);
			i--;
			continue;
		}

		if (!particles[i].render) {
			continue;
		}

		if (particles[i].type == "water") {
			glUniform1i(diffuse_map_location, 1);
			glUniform1i(specular_map_location, 2);
			glUniform1i(normal_map_location, 3);
			glUniform1i(shiny_location, 1);
		}
		else if (particles[i].type == "lava") {
			glUniform1i(diffuse_map_location, 7);
			glUniform1i(specular_map_location, 8);
			glUniform1i(normal_map_location, 9);
			glUniform1i(shiny_location, 0);
		}
		else if (particles[i].type == "smoke") {
			glUniform1i(diffuse_map_location, 10);
			glUniform1i(specular_map_location, 11);
			glUniform1i(normal_map_location, 12);
			glUniform1i(shiny_location, 0);
		}
		else{
			glUniform1i(diffuse_map_location, 4);
			glUniform1i(specular_map_location, 5);
			glUniform1i(normal_map_location, 6);
			glUniform1i(shiny_location, 0);
		}

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
}

void assign_bucket(vector<Particle> &particles, vector<vector<int>> &buckets) {
	for (int i = 0; i < 7; i++) {
		(buckets[i]).clear();
	}
	for (int i = 0; i < particles.size(); i++) {
		int bucketPos = (particles[i].transform.position.x + 6)/2;
		if (bucketPos > 6) {
			bucketPos = 0;
		}
		(buckets[bucketPos]).push_back(i);
	}
}

int main(void) {
	GLFWwindow* window = initialize_glfw();
	GLuint shader_program = compile_shader("shader.vert", "shader.frag");
	GLuint sky_shader_program = compile_shader("sky.vert", "sky.frag");
	GLuint postprocess_shader_program = compile_shader("post.vert", "post.frag");

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
	models.push_back(Model::load(string("normals/plane-normals.obj")));

	Particle cubemap = Particle(models[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), "cubemap");
	vector<vector<int>> buckets;
	for (int i = 0; i < 7; i++) {
		vector<int> v;
		buckets.push_back(v);
	}


	vector<Particle> particles;
	//particles.push_back(Particle(models[0], glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));
	/*
	particles.push_back(Particle(models[0], glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));

	particles.push_back(Particle(models[0], glm::vec3(3.0f, 2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));
	particles.push_back(Particle(models[0], glm::vec3(3.0f, -2.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));

	particles.push_back(Particle(models[0], glm::vec3(-3.0f, 4.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));
	particles.push_back(Particle(models[0], glm::vec3(-3.0f, 0.0f, -5.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -RandomFloat(0.005f, 0.05f), 0.0f)));
	*/

	vector<GLuint> textures;
	textures.push_back(load_textures(GL_TEXTURE1, "./Water/WaterColor.jpg"));
	textures.push_back(load_textures(GL_TEXTURE2, "./Water/WaterSpec.jpg"));
	textures.push_back(load_textures(GL_TEXTURE3, "./Water/WaterNormal.jpg"));

	textures.push_back(load_textures(GL_TEXTURE4, "./Rock/Rock2Color.jpg"));
	textures.push_back(load_textures(GL_TEXTURE5, "./Rock/Rock2Roughness.jpg"));
	textures.push_back(load_textures(GL_TEXTURE6, "./Rock/Rock2Normal.jpg"));

	textures.push_back(load_textures(GL_TEXTURE7, "./Lava2/LavaColor.jpg"));
	textures.push_back(load_textures(GL_TEXTURE8, "./Lava2/LavaRoughness.jpg"));
	textures.push_back(load_textures(GL_TEXTURE9, "./Lava2/LavaNormal.jpg"));

	textures.push_back(load_textures(GL_TEXTURE10, "./Snow/SnowColor.jpg"));
	textures.push_back(load_textures(GL_TEXTURE11, "./Snow/SnowRoughness.jpg"));
	textures.push_back(load_textures(GL_TEXTURE12, "./Snow/SnowNormal.jpg"));

	textures.push_back(load_cubemap(GL_TEXTURE0, 
		"./cloudy/bluecloud_rt.jpg",
		"./cloudy/bluecloud_ft.jpg", 
		"./cloudy/bluecloud_lf.jpg", 
		"./cloudy/bluecloud_bk.jpg", 
		"./cloudy/bluecloud_up.jpg", 
		"./cloudy/bluecloud_dn.jpg"));

	Camera camera;
	camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 10.0f, -25.0f));

	//glm::mat4 world_from_model = glm::mat4(1.0); // init to the identity matrix
	
	//Initialize framebuffers
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//initialize color buffer
	GLuint fbo_color;
	glGenTextures(1, &fbo_color);
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, fbo_color);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	
	glTexImage2D(GL_TEXTURE_2D, 
		0, 
		GL_RGB, 
		width, 
		height, 
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		fbo_color,
		0);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);


	//initialize depth buffer
	GLuint fbo_depth;
	glGenTextures(1, &fbo_depth);
	glActiveTexture(GL_TEXTURE14);
	glBindTexture(GL_TEXTURE_2D, fbo_depth);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH24_STENCIL8,
		width,
		height,
		0,
		GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8,
		nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		fbo_depth,
		0);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "framebuffer incomplete!\n";
		abort();
	}


	int count = 0;
	bool pressed = false;
	string type = "sand";
	float fade = 0.0f;
	bool running = true;

	while (!glfwWindowShouldClose(window)) {  

		if (running) {
			if (fade < 1.0f) {
				fade += 0.00025f;
			}
		}
		else {
			if (fade > 0.0f) {
				fade -= 0.00025f;
			}
			else {
				break;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_R)) {
			particles.clear();
			cout << "reset";
		}
		if (glfwGetKey(window, GLFW_KEY_1)) {
			type = "sand";
			cout << type << endl;
		}
		if (glfwGetKey(window, GLFW_KEY_2)) {
			type = "water";
			cout << type << endl;
		}
		if (glfwGetKey(window, GLFW_KEY_3)) {
			type = "lava";
			cout << type << endl;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			if (!pressed) {
				cout << "spawn" << endl;
				
				particles.push_back(Particle(models[0],
					//glm::vec3((int)RandomFloat(-2.0f, 2.0f)*2.0, 16.0f, (int)RandomFloat(-2.0f, 2.0f) * 2.0),
					glm::vec3(0.0f, 16.0f, 0.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec3(0.0f, -2.0f, 0.0f),
					type));
				

				/*
				particles.push_back(Particle(models[0],
					glm::vec3(0.0f, 0.0f, -5.0f),
					glm::vec3(1.0f, 1.0f, 1.0f),
					glm::vec3(RandomFloat(-0.005f, 0.005f), RandomFloat(0.001f, 0.01f), RandomFloat(-0.001f, 0.001f))));
				*/
				pressed = true;
			}
		}
		else 
		{
			pressed = false;
		}

		GLint light_location = glGetUniformLocation(shader_program, "lightPos");
		glUniform3f(light_location, lightPos[0], lightPos[1], lightPos[2]);

		//camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 0.0f, 0.0001f));
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			running = false;
		}

		if (glfwGetKey(window, GLFW_KEY_UP)) {
			camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, -0.01f, 0.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 0.01f, 0.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			camera.camera_from_world = glm::rotate(camera.camera_from_world, 0.001f, glm::vec3(0.0f, -1.0f, 0.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			camera.camera_from_world = glm::rotate(camera.camera_from_world, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		//world_from_model = glm::rotate(world_from_model, 0.001f, glm::vec3(0.0f, 1.0f, 0.0f));

		//move and check collision on particles
		
		count++;
		if (count % 25 == 0) {
			//double time = glfwGetTime();
			
			assign_bucket(particles, buckets);

			for (int i = 0; i < buckets.size(); i++) {
				vector<int> particlesCheck;

				//add bucket before
				
				if (i > 0) {
					for (int j = 0; j < buckets[i - 1].size(); j++) {
						particlesCheck.push_back(buckets[i - 1][j]);
					}
				}

				//add bucket
				for (int j = 0; j < buckets[i].size(); j++) {
					particlesCheck.push_back(buckets[i][j]);
				}

				//add bucket after
				if (i < 6) {
					for (int j = 0; j < buckets[i+1].size(); j++) {
						particlesCheck.push_back(buckets[i+1][j]);
					}
				}

				int move = particles_move_collision(particles,buckets[i],particlesCheck);
				if (move == 1) {
					assign_bucket(particles, buckets);
				}
				//cout << "moved\n";
			}
			/*
			assign_bucket(particles, buckets);
			print(particles);
			for (int i = 0; i < 7; i++) {
				cout << "bucket " << i << ": ";
				print(buckets[i]);
			}
			*/

			//at least this one works
			//particles_move_collision(particles, particles);
			
			count = 1;

			//double passed = glfwGetTime() - time;

			//cout << "part #: " << particles.size() << " | time: " << passed << endl;
		}


		/*
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_scene(shader_program, sky_shader_program, window, cubemap, particles, camera);
		*/
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		render_scene(shader_program, sky_shader_program, window, cubemap, particles, camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		{	
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(postprocess_shader_program);
			GLint tex_location = glGetUniformLocation(postprocess_shader_program, "tex");
			glUniform1i(tex_location, 13); // GL_TEXTURE4

			GLint fade_location = glGetUniformLocation(postprocess_shader_program, "fade");
			glUniform1f(fade_location, fade);

			models[2].draw();
		}
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteTextures(textures.size(), &(textures)[0]);
	glDeleteFramebuffers(1, &fbo);
	glDeleteProgram(shader_program);
	glDeleteProgram(sky_shader_program);
	for (Model model : models) {
		model.cleanup();
	}

	glfwTerminate();

	return 0;
}

