//----------------------------------------------------------------------------------------
/**
 * \file       Model.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Mesh.h"

class Model
{
public:
	//Data
	std::vector<Mesh> meshes;
	std::string modelDirectory;
	std::vector<Texture> textures;
	std::vector<Texture> textures_loaded;
	std::string modelName;
	bool gammaCorrection;
	bool useTextures = true;
	struct Material {
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		float Shininess;
	};
	//Construct
	Model(std::string &modelPath, std::string name, bool gamma = false) : gammaCorrection(gamma) {
		std::cout << "[MODEL] Init called \n";
		modelName = name;
		loadModel(modelPath);
	}
	void DrawModel(Program * program);
	~Model();
private:
	void loadModel(std::string & modelPath);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial * material, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char * path, const std::string & directory);
	Mesh::Material loadMaterial(aiMaterial* mat);
};

