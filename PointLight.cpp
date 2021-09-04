//----------------------------------------------------------------------------------------
/**
 * \file       PointLight.cpp
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#include "pch.h"
#include "PointLight.h"



PointLight::PointLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constantPower, float linearPower, float quadraticPower)
{
	geometry.position = position;
	geometry.direction = direction;
	geometry.ambient = ambient;
	geometry.diffuse = diffuse;
	geometry.specular = specular;

	geometry.constantPower = constantPower;
	geometry.linearPower = linearPower;
	geometry.quadraticPower = quadraticPower;
}

bool PointLight::setUniforms(Program * program, GLuint index)
{
	std::string number = std::to_string(index);
	glUniform3fv(glGetUniformLocation(program->programId, ("pointlights[" + number + "].position").c_str()), 1, glm::value_ptr(geometry.position));
	glUniform3fv(glGetUniformLocation(program->programId, ("pointlights[" + number + "].direction").c_str()), 1, glm::value_ptr(geometry.direction));
	glUniform3fv(glGetUniformLocation(program->programId, ("pointlights[" + number + "].ambient").c_str()), 1, glm::value_ptr(geometry.ambient));
	glUniform3fv(glGetUniformLocation(program->programId, ("pointlights[" + number + "].diffuse").c_str()), 1, glm::value_ptr(geometry.diffuse));
	glUniform3fv(glGetUniformLocation(program->programId, ("pointlights[" + number + "].specular").c_str()), 1, glm::value_ptr(geometry.specular));

	glUniform1f(glGetUniformLocation(program->programId, ("pointlights[" + number + "].constantPower").c_str()), geometry.constantPower);
	glUniform1f(glGetUniformLocation(program->programId, ("pointlights[" + number + "].linearPower").c_str()), geometry.linearPower);
	glUniform1f(glGetUniformLocation(program->programId, ("pointlights[" + number + "].quadraticPower").c_str()), geometry.quadraticPower);
	CHECK_GL_ERROR();
	return true;
}

PointLight::~PointLight()
{
}
