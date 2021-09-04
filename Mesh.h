//----------------------------------------------------------------------------------------
/**
 * \file       Mesh.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include "Program.h"
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	//glm::vec3 tangent;
	//glm::vec2 bitangent;
};
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};
class Mesh {
public:
	struct Material {
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		float Shininess;
	}material;
	bool useTextures;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;
	//Construct
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material * material, bool useTexture) {
		this->indices = indices;
		this->vertices = vertices;
		this->textures = textures;

		this->material = *material;
		this->useTextures = useTexture;
		initMeshData();
	}
	//draw contents
	void DrawMesh(Program * program) {
		unsigned int diffuseIdx = 1;
		unsigned int specularIdx = 1;

		unsigned int normalIdx = 1;
		unsigned int heightIdx = 1;

		if (useTextures) {
			//Texture Binding
			for (unsigned int i = 0; i < textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i); //activates texture with ID 0 + offset
				//representing texture types as "type_name[i]" -> E.G. texture_diffuse2
				std::string name = textures[i].type;
				std::string index;
				if (name == "texture_diffuse") {
					index = std::to_string(diffuseIdx++); //postIncrement
				}
				else if (name == "texture_specular") {
					index = std::to_string(specularIdx++);
				}
				else if (name == "texture_normal") {
					index = std::to_string(normalIdx++);
				}
				else if (name == "texture_height") {
					index = std::to_string(heightIdx++);
				}
				glUniform1i(glGetUniformLocation(program->programId, (name + index).c_str()), i); //connecting sampler to the correct TMU
				glUniform1i(glGetUniformLocation(program->programId, "useTextures"), useTextures);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
			}
			//glActiveTexture(GL_TEXTURE0);
		}
		else {
			glUniform1i(glGetUniformLocation(program->programId, "useTextures"), useTextures);
			glUniform3fv(glGetUniformLocation(program->programId, "material.ambient"), 1, glm::value_ptr(material.Ambient));
			glUniform3fv(glGetUniformLocation(program->programId, "material.diffuse"), 1, glm::value_ptr(material.Diffuse));
			glUniform3fv(glGetUniformLocation(program->programId, "material.specular"), 1, glm::value_ptr(material.Specular));
		}


		glBindVertexArray(VAO);
		//std::cout << "Drawing mesh\n";
		glDrawElements(GL_TRIANGLES, indices.size()/*vertices.size()*/, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, vertices.size(), 20);
		glBindVertexArray(0); //unbind

		glActiveTexture(GL_TEXTURE0);
	}
private: 
	unsigned int VBO, EBO;
	void initMeshData() {/*
		//VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//VBO
		glGenBuffers(1, &VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); //_COPY - GPU->GPU
		//EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		*/
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		//glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); //_COPY - GPU->GPU

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);	
		//				   |IDX||SIZE| |TYPE|             |STRIDE|		 |OFFSET|
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), /*(void*)*/0); //Pointer to data structure for positions

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); 
			//offsetof(array {=strucure}, attribut) - finds position of given attribut in given Structure represented as array
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		/*glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));*/

		glBindVertexArray(0); //unBind
	}
};