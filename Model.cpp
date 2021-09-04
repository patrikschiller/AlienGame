#include "pch.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Mesh.h"
#include "Model.h"


/*Model::Model()
{
		int width, height, channels;
	unsigned char * data = stbi_load("data/textures/grassBlock.png", &width, &height, &channels, 0);
}*/


void Model::DrawModel(Program * program)
{
	unsigned int counter = 0;
	for (unsigned int i = 0; i < meshes.size(); i++) {
		if (counter >= 3200) {
			return;
		}
		///std::cout << "[MODEL] Drawing mesh " << i << "\n"; WORKS
		/*std::cout << "[MESH]<"<<i<<"> Indices:" <<
		meshes[i].indices.size()
		<< " Vertices: " <<
		meshes[i].vertices.size()
		<< " Textures: " <<
		meshes[i].textures.size()
		<< "\n";*/
		meshes[i].DrawMesh(program);
		counter++;
	}
}

Model::~Model()
{
}

void Model::loadModel(std::string & modelPath)
{
	std::cout << "[MODEL][" << this->modelName << "] Load called \n";
	Assimp::Importer modelImporter;
	const aiScene * scene = modelImporter.ReadFile(modelPath, aiProcess_Triangulate); //Primitive triangulation, reversing Normals
	if (!scene || scene->mFlags &AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "[ERROR][ASSIMP] - " << modelImporter.GetErrorString() << "\n";
		return;
	}
	std::cout << "[INFO][ASSIMP] - Model sucessfully loaded\n";
	std::cout << "[INFO][ASSIMP] - Meshes: " << scene->mNumMeshes << "\n";
	modelDirectory = modelPath.substr(0, modelPath.find_last_of('/'));
	//Process tree
	CHECK_GL_ERROR();
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	///std::cout << "[MODEL] Process node called \n";
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		CHECK_GL_ERROR();
		Mesh m = processMesh(mesh, scene);
		//std::cout << "Error type: " << glGetError() << "\n";
		CHECK_GL_ERROR();

		if (&m == nullptr) { std::cout << "[ERROR][MODEL][" << this->modelName << "] Process mesh failed \n"; }
		meshes.push_back(m);
		///std::cout << "[MODEL] Mesh pushed" << meshes.front().indices.size()<<"\n";
		CHECK_GL_ERROR();
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
		CHECK_GL_ERROR();
	}
	CHECK_GL_ERROR();
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	///std::cout << "[MODEL] Process mesh called \n";
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//Position coords
		glm::vec3 temp; //temp vector for safe data transfer
		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;
		vertex.position = temp;

		//normal coords
		if (mesh->mNormals) {
			temp.x = mesh->mNormals[i].x;
			temp.y = mesh->mNormals[i].y;
			temp.z = mesh->mNormals[i].z;
			vertex.normal = temp;
		}
		vertices.push_back(vertex);

		//texture coords (if any)
		if (mesh->mTextureCoords[0]) {// if exists
			glm::vec2 coord;
			coord.x = mesh->mTextureCoords[0][i].x;
			coord.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		CHECK_GL_ERROR();

	}
	CHECK_GL_ERROR();
	bool useTextures = false;
	if (mesh->mTextureCoords[0]) {
		//std::cout << "[MODEL] Textures included \n";
		//useTextures = true;
		this->useTextures = true;
	}
	else {
		//std::cout << "[MODEL] No textures \n";
		//useTextures = false;
		this->useTextures = false;
	}

	//setting idnices for EBO
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
		CHECK_GL_ERROR();
	}
	CHECK_GL_ERROR();
	//setting materials
	Mesh::Material  m;
	if (mesh->mMaterialIndex >= 0 && this->useTextures) {
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		CHECK_GL_ERROR();
		//std::cout << "[MODEL] Textures included \n";
	}
	else {
		m = loadMaterial(scene->mMaterials[mesh->mMaterialIndex]);
	}
	CHECK_GL_ERROR();
	//return Mesh(vertices, indices, textures);
	return Mesh(vertices, indices, textures, &m, /*useTextures*/this->useTextures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * material, aiTextureType type, std::string typeName)
{
	///std::cout << "[MODEL] Load Material Textures called \n";
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString string;
		material->GetTexture(type, i, &string);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), string.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(string.C_Str(), this->modelDirectory);
			texture.type = typeName;
			texture.path = string.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
		//Texture texture;
		//texture.id = TextureFromFile(string.C_Str(), modelDirectory);
		//return textures;
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char * path, const std::string & directory)
{
	///std::cout << "[MODEL] Texture from file called \n";
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//unsigned char *daa = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	//unsigned char * data2 = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (data) {
		std::cout << "[MODEL]["<< this->modelName <<"][TEXTURE] texture sucessfully loaded\n";
	}
	else {
		std::cout << "[ERROR][MODEL][" << this->modelName << "][TEXTURE] texture WASNT loaded\n";
		this->useTextures = false;
	}
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format /*GL_RGB*/, width, height, 0, /*GL_RGB*/format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		//SOIL_free_image_data(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
		//SOIL_free_image_data(data);
	}
	CHECK_GL_ERROR();
	return textureID;
}

Mesh::Material Model::loadMaterial(aiMaterial * mat)
{
	Mesh::Material material;
	aiColor3D color(0.f, 0.f, 0.f);
	float shininess;

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.Diffuse = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.Ambient = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.Specular = glm::vec3(color.r, color.b, color.g);

	mat->Get(AI_MATKEY_SHININESS, shininess);
	material.Shininess = shininess;

	return material;
}

