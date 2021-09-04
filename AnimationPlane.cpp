//----------------------------------------------------------------------------------------
/**
 * \file       AnimationPlane.cpp
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#include "pch.h"
#include "AnimationPlane.h"
#include "stb_image.h"




AnimationPlane::~AnimationPlane()
{
}

void AnimationPlane::Draw(Program * program, Camera * camera, float time)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	program->use();

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), geometry.position); glm::vec3(0.0f, 0.0f, 1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(geometry.size));
	glm::mat4 billboardRotationMatrix = glm::mat4(
		camera->View[0],
		camera->View[1],
		camera->View[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	modelMatrix = modelMatrix * billboardRotationMatrix;

	glm::mat4 PVMmatrix = camera->Projection * camera->View * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program->programId, "PVMmatrix"), 1, GL_FALSE, glm::value_ptr(PVMmatrix));
	CHECK_GL_ERROR();
	glUniformMatrix4fv(glGetUniformLocation(program->programId, "Vmatrix"), 1, GL_FALSE, glm::value_ptr(camera->View));
	CHECK_GL_ERROR();
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(program->programId, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform2iv(glGetUniformLocation(program->programId, "pattern"), 1, glm::value_ptr(geometry.diameter));
	glUniform1i(glGetUniformLocation(program->programId, "actualFrame"), geometry.actualFrame);
	glUniform1i(glGetUniformLocation(program->programId, "texSampler"), 0);
	glUniform1i(glGetUniformLocation(program->programId, "animationType"), geometry.animationType);
	glUniform1f(glGetUniformLocation(program->programId, "time"), geometry.curentTime);
	CHECK_GL_ERROR();

	//Draw ----
	glActiveTexture(GL_TEXTURE0 + 0); ///Offset texturovací jednotky (0-15)

	glBindVertexArray(geometry.VAO);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, geometry.texture);
	CHECK_GL_ERROR();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 20);
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	//-----
	CHECK_GL_ERROR();
	glUseProgram(0);
	glDisable(GL_BLEND);
	return;
}

bool AnimationPlane::initShaderProgram()
{
	const char * VS = "shaders/animationShader.vert";
	const char * FS = "shaders/animationShader.frag";
	program = new Program(VS, FS, "Animation Shader");
	program->deleteShaders();
	if (program) {
		std::cout << "[Animation] shaders initialized \n";
		return true;
	}
	std::cout << "[Animation] ERROR: shaders weren't initialized \n";
	return false;
}

bool AnimationPlane::initGeometry()
{
	std::cout << "AnimationPlane init\n";

	glGenVertexArrays(1, &(geometry.VAO)); //generovani vao
	glBindVertexArray(geometry.VAO); //bind vao
	CHECK_GL_ERROR();
	glGenBuffers(1, &(geometry.VBO)); //generovani vbo
	glBindBuffer(GL_ARRAY_BUFFER, geometry.VBO); //bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(animationVertices), animationVertices, GL_STATIC_DRAW); //naplneni vbo
	CHECK_GL_ERROR();

	//vertices coords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); //3 slozky - xyz, stride 3 + 2*size
	CHECK_GL_ERROR();
	//tex coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	CHECK_GL_ERROR();

	glGenTextures(1, &(geometry.texture));
	glBindTexture(GL_TEXTURE_2D, (geometry.texture));
	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //nastavení pro osu S (=x) souøadnic textury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // -||- pro osu T (=y)	
	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //minifying pro downscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //magnifying pro upscaling  
	CHECK_GL_ERROR();

	int width, height, channels;
	unsigned char * data = stbi_load(geometry.directory/*"data/textures/scifiBlock.png"*//*"data/Animation/fire12x6NumbersFlipped.png"*//*"data/Explosion/digits.png"*/, &width, &height, &channels, 0);
	CHECK_GL_ERROR();

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "[RENDER] Texture successfuly loaded\n";
	}
	else {
		std::cout << "[ERROR][RENDER] Failed to load actual texture\n";
	}
	stbi_image_free(data); //uvolnìní pamìti

	glBindVertexArray(0);

	return true;
}
