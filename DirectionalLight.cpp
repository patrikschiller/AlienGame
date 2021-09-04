//----------------------------------------------------------------------------------------
/**
 * \file       DirectionalLight.cpp
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#include "pch.h"
#include "DirectionalLight.h"

/*DirectionalLight::DirectionalLight()
{
}
*/

DirectionalLight::DirectionalLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	geometry.position = position;
	geometry.direction = direction;
	geometry.ambient = ambient;
	geometry.diffuse = diffuse;
	geometry.specular = specular;
}

bool DirectionalLight::setUniforms(Program * program, GLuint index)
{
	std::string number = std::to_string(index);
	//std::cout << "[DIRECTIONAL] Setting uniforms diffuse["<< geometry.diffuse.x << ", " << geometry.diffuse.y << ", " << geometry.diffuse.z << "] " << " \n";
	glUniform3fv(glGetUniformLocation(program->programId, ("dirLights[" + number + "].position").c_str()), 1, glm::value_ptr(geometry.position));
	glUniform3fv(glGetUniformLocation(program->programId, ("dirLights[" + number + "].direction").c_str()), 1, glm::value_ptr(geometry.direction));
	glUniform3fv(glGetUniformLocation(program->programId, ("dirLights[" + number + "].ambient").c_str()), 1, glm::value_ptr(geometry.ambient));
	glUniform3fv(glGetUniformLocation(program->programId, ("dirLights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(geometry.diffuse));
	glUniform3fv(glGetUniformLocation(program->programId, ("dirLights[" + number + "].specular").c_str()), 1, glm::value_ptr(geometry.specular));
	CHECK_GL_ERROR();
	return true;
}

void DirectionalLight::initCycle(float speed, float actualTime)
{
	geometry.doCycle = true;
	geometry.speed = speed;
	geometry.currentTime = actualTime;
	geometry.degrees = 0;
}

void DirectionalLight::stopCycle()
{
	geometry.doCycle = false;
}

DirectionalLight::~DirectionalLight()
{
}
