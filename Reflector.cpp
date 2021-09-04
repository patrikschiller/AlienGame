//----------------------------------------------------------------------------------------
/**
 * \file       Reflector.cpp
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#include "pch.h"
#include "Reflector.h"


/*Reflector::Reflector()
{
}*/


Reflector::Reflector(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff, float constantPower, float linearPower, float quadraticPower)
{
	geometry.position = position;
	geometry.direction = direction;
	geometry.ambient = ambient;
	geometry.diffuse = diffuse;
	geometry.specular = specular;
	geometry.cutOff = cutOff;
	geometry.outerCutoff = outerCutOff;
	geometry.constantPower = constantPower;
	geometry.linearPower = linearPower;
	geometry.quadraticPower = quadraticPower;
}

bool Reflector::setUniforms(Program * program, GLuint index)
{
	//const std::string name = "reflectors["  + std::to_string(index) + "]";
	std::string number = std::to_string(index);
	glUniform3fv(glGetUniformLocation(program->programId, ("reflectors[" + number + "].position").c_str()), 1, glm::value_ptr(geometry.position));
	glUniform3fv(glGetUniformLocation(program->programId, ("reflectors[" + number + "].direction").c_str()), 1, glm::value_ptr(geometry.direction));
	glUniform3fv(glGetUniformLocation(program->programId, ("reflectors[" + number + "].ambient").c_str()), 1, glm::value_ptr(geometry.ambient));
	glUniform3fv(glGetUniformLocation(program->programId, ("reflectors[" + number + "].diffuse").c_str()), 1, glm::value_ptr(geometry.diffuse));
	glUniform3fv(glGetUniformLocation(program->programId, ("reflectors[" + number + "].specular").c_str()), 1, glm::value_ptr(geometry.specular));
	glUniform1f(glGetUniformLocation(program->programId, ("reflectors[" + number + "].cutOff").c_str()), geometry.cutOff);
	glUniform1f(glGetUniformLocation(program->programId, ("reflectors[" + number + "].outerCutoff").c_str()), geometry.outerCutoff);
	glUniform1f(glGetUniformLocation(program->programId, ("reflectors[" + number + "].constantPower").c_str()), geometry.constantPower);
	glUniform1f(glGetUniformLocation(program->programId, ("reflectors[" + number + "].linearPower").c_str()), geometry.linearPower);
	glUniform1f(glGetUniformLocation(program->programId, ("reflectors[" + number + "].quadraticPower").c_str()), geometry.quadraticPower);
	CHECK_GL_ERROR();
	return true;
}

bool Reflector::setAlarmUniforms(Program * program, GLuint index)
{
	//const std::string name = "reflectors["  + std::to_string(index) + "]";
	std::string number = std::to_string(index);
	glUniform3fv(glGetUniformLocation(program->programId, ("alarms[" + number + "].position").c_str()), 1, glm::value_ptr(geometry.position));
	glUniform3fv(glGetUniformLocation(program->programId, ("alarms[" + number + "].direction").c_str()), 1, glm::value_ptr(geometry.direction));
	glUniform3fv(glGetUniformLocation(program->programId, ("alarms[" + number + "].ambient").c_str()), 1, glm::value_ptr(geometry.ambient));
	glUniform3fv(glGetUniformLocation(program->programId, ("alarms[" + number + "].diffuse").c_str()), 1, glm::value_ptr(geometry.diffuse));
	glUniform3fv(glGetUniformLocation(program->programId, ("alarms[" + number + "].specular").c_str()), 1, glm::value_ptr(geometry.specular));
	glUniform1f(glGetUniformLocation(program->programId, ("alarms[" + number + "].cutOff").c_str()), geometry.cutOff);
	glUniform1f(glGetUniformLocation(program->programId, ("alarms[" + number + "].outerCutoff").c_str()), geometry.outerCutoff);
	glUniform1f(glGetUniformLocation(program->programId, ("alarms[" + number + "].constantPower").c_str()), geometry.constantPower);
	glUniform1f(glGetUniformLocation(program->programId, ("alarms[" + number + "].linearPower").c_str()), geometry.linearPower);
	glUniform1f(glGetUniformLocation(program->programId, ("alarms[" + number + "].quadraticPower").c_str()), geometry.quadraticPower);
	glUniform1i(glGetUniformLocation(program->programId, ("alarms[" + number + "].calculate").c_str()), true);
	CHECK_GL_ERROR();
	return true;
}

Reflector::~Reflector()
{
}
