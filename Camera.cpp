#include "pch.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
	std::cout << "Camera destruct!\n";
}

void Camera::moveForward(float deltaMotion)
{
	position += deltaMotion * movementDirection;
}

void Camera::moveBackward(float deltaMotion)
{
	position -= deltaMotion * movementDirection;
}

void Camera::moveLeft(float deltaMotion)
{
	position -= glm::normalize(glm::cross(movementDirection, upVector)) * deltaMotion;
}

void Camera::moveRight(float deltaMotion)
{
	position += glm::normalize(glm::cross(movementDirection, upVector)) * deltaMotion;
}

void Camera::moveUp(float deltaMotion)
{
	position.z += deltaMotion;
}

void Camera::moveDown(float deltaMotion)
{
	position.z -= deltaMotion;
}

void Camera::recalculateCenter()
{
	center = glm::normalize(direction) + position;
}

void Camera::rotateByMouse(float deltaX, float deltaY)
{
	///std::cout << "Rotation by mouse " <<cameraHorizontalAngle<<"\n";
	glm::vec3 xAxis = glm::cross(direction, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 cameraRotaionX = glm::rotate(
		glm::mat4(1.0f),
		glm::radians(deltaY),
		xAxis
	);
	direction = cameraRotaionX * glm::vec4(direction, 0.0f);

	glm::mat4 cameraRotaionZ = glm::rotate(
		glm::mat4(1.0f), 
		glm::radians(deltaX),
		upVector
	);
	direction = cameraRotaionZ * glm::vec4(direction, 0.0f);
	movementDirection.x = direction.x;
	movementDirection.y = direction.y;
	movementDirection.z = 0.0f;
}

void Camera::updateView()
{
	View = glm::lookAt(
		position,
		center,
		upVector
	);
}

void Camera::updateProjection(float winWidth, float winHeight)
{
	Projection = glm::perspective(
		glm::radians(60.0f),
		winWidth / (float)winHeight,
		0.1f,
		150.0f //Render distance
	);
}

void Camera::updateCamera(float winWidth, float winHeight)
{
	recalculateCenter();
	updateView();
	updateProjection(winWidth, winHeight);
}

void Camera::turnHardLeft(float deltaAngle)
{
	std::cout << "turn left called\n";
	cameraHorizontalAngle += deltaAngle;
	direction = glm::vec3(
		cos(glm::radians(cameraHorizontalAngle)),
		sin(glm::radians(cameraHorizontalAngle)),
		0.0f
	);
	movementDirection.x = direction.x;
	movementDirection.y = direction.y;
	movementDirection.z = 0.0f;
}

void Camera::turnHardRight(float deltaAngle)
{
	std::cout << "turn right called\n";
	cameraHorizontalAngle -= deltaAngle;
	direction = glm::vec3(
		cos(glm::radians(cameraHorizontalAngle)),
		sin(glm::radians(cameraHorizontalAngle)),
		0.0f
	);
	movementDirection.x = direction.x;
	movementDirection.y = direction.y;
	movementDirection.z = 0.0f;
}
