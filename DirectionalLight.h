//----------------------------------------------------------------------------------------
/**
 * \file       DirectionalLight.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "pgr.h"
#include "Program.h"
class DirectionalLight
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

		float degrees; // 0 - 360
		float currentTime;
		bool doCycle;
		float speed;

	}geometry;
	 DirectionalLight(
		glm::vec3 position,
		glm::vec3 direction,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular
	);
	 /**
		Sets uniforms fot given light in given shader program
		@param program - used shader program
		@param index - index of light (setting array of lights)
	 */
	bool setUniforms(Program * program, GLuint index);
	/*
		Inits moving cycle
		@param speed - motion speed
		@param actualTime - init time
	*/
	void initCycle(float speed, float actualTime);
	/**
		Stops moving cycle
		@param void
	*/
	void stopCycle();
	~DirectionalLight();
};

