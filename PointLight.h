//----------------------------------------------------------------------------------------
/**
 * \file       PointLight.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "pgr.h"
#include "Program.h"
class PointLight
{
public:
	/**
	Structure that represents geometry of given Light
	*/
	struct Geometry {
		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constantPower;
		float linearPower;
		float quadraticPower;

	}geometry;

	PointLight(
		glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float constantPower,
		float linearPower,
		float quadraticPower
	);
	/**
	   Sets uniforms fot given light in given shader program
	   @param program - used shader program
	   @param index - index of light (setting array of lights)
	*/
	bool setUniforms(Program * program, GLuint index);
	~PointLight();
};

