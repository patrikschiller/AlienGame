//----------------------------------------------------------------------------------------
/**
 * \file       Camera.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include "pgr.h"
#include <iostream>
class Camera
{
	/**
		Recalculates far center of camera
	*/
	void recalculateCenter();
	void updateView();
	void updateProjection(float winWidth, float winHeight);
public:
	glm::vec3 position;
	glm::vec3 initPosition;
	glm::vec3 direction; //=center
	glm::vec3 movementDirection;
	glm::vec3 upVector;
	glm::vec3 center;
	float time;
	float timeDelta;
	std::string name;
	bool staticCam = false;

	glm::mat4 Projection;
	glm::mat4 View;

	float cameraElevationAngle;
	float cameraHorizontalAngle;
	float motionSpeed;
	glm::vec2 mouseDelta;

	Camera();
	Camera(glm::vec3 pos, glm::vec3 direction, glm::vec3 up, std::string name) {
		this->position = pos;
		this->direction = direction;
		this->upVector = up;
		this->movementDirection = glm::vec3(up.x, 0.0f, up.z);
		this->name = name;
		this->initPosition = pos;
		std::cout << "Camera initiated\n";
		cameraElevationAngle = 0.0f;
		cameraHorizontalAngle = 0.0f;
	}
	virtual ~Camera(); //default, maybe inherited

	/**
		Moves camera forward by given distance
	*/
	void moveForward(float deltaMotion);
	/**
	Moves camera backward by given distance
*/
	void moveBackward(float deltaMotion);
	/**
	Moves camera left by given distance
*/
	void moveLeft(float deltaMotion);
	/**
	Moves camera right by given distance
*/
	void moveRight(float deltaMotion);
	/**
	Moves camera up by given distance
*/
	void moveUp(float deltaMotion);
	/**
	Moves camera down by given distance
*/
	void moveDown(float deltaMotion);

	/**
		Rotates camera direction by given anle
	*/
	void rotateByMouse(float deltaX, float deltaY);

	void updateCamera(float winWidth, float winHeight);

	void turnHardLeft(float deltaAngle);
	void turnHardRight(float deltaAngle);
};

