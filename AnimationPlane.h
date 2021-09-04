//----------------------------------------------------------------------------------------
/**
 * \file       AnimationPlane.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "Camera.h"
#include "pgr.h"
#include "Program.h"
#include "data.h"
#include "spline.h"

class AnimationPlane
{
public:
	Program * program;
	/**
	Structure that represents geometry of given Light
	*/
	struct Geometry {
		glm::vec3 position;
		glm::vec3 direction;
		float size;
		GLuint VAO;
		GLuint VBO;
		GLuint texture;
		const char * directory;
		float curentTime;

		GLuint actualFrame;
		GLuint totalFrames;
		GLfloat frameDuration;
		GLfloat timeDelta;
		glm::ivec2 diameter;
		int animationType;
	}geometry;
	AnimationPlane(glm::vec3 pos, glm::vec3 dir, float size, const char * directory,GLuint totalFrames, glm::ivec2 diameter, GLint animationType, GLfloat frameDuration = 0.1f) {
		geometry.position = pos;
		geometry.direction = dir;
		geometry.size = size;
		geometry.directory = directory;
		geometry.totalFrames = totalFrames;
		geometry.actualFrame = 0;
		geometry.frameDuration = frameDuration;
		geometry.timeDelta = 0;
		geometry.diameter = diameter;
		geometry.animationType = animationType;

		initShaderProgram();
		initGeometry();
	}
	~AnimationPlane();
	void Draw(Program * program, Camera * camera, float time);
private:
	/*
		Initializes shader program for current animation
	*/
	bool initShaderProgram();
	/*
		Initializes geomtry data for current animation
	*/
	bool initGeometry();
};

