//----------------------------------------------------------------------------------------
/**
 * \file       render.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
//Renderovaci funkce modelu
#pragma once
#include "data.h"
#include "Camera.h"
#include "Model.h"
#include <list>
//Struktura s geometrii objektu
 typedef struct MeshGeometry {
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	unsigned int  numTriangles;         // number of triangles in the mesh
	GLuint texture;

	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;

	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;

	float lightConstantPower;
	float lightLinearPower;
	float lightQuadraticPower;

	float lightCameraCutoff;
	float lightCameraOuterCutoff;

} MeshGeometry;

//Reprezentace obecneho objektu a jeho atributu
typedef struct Object {
	glm::vec3 position;
	glm::vec3 direction; /// vektor d
	float     size;
	GLint id;
} Object;

typedef struct BoxObject : public Object {
	float colorMod;
} BoxObject;

typedef struct SquareObject : public Object {
	float colorMod;
} SquareObject;
/*
typedef struct ViewerObject : public Object {
	float viewAngle; // in degrees = úhel alfa
	glm::vec2 mouseDelta;
} ViewerObject;*/

typedef struct SunObject : public Object {
	float lightIntensity; ///can be changed
	std::string movingDir;
} SunObject;

typedef struct AlarmObject : public Object {
	// + position + direction (=light direction)
	glm::vec3 color;
	//Light colors
	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;
	float angle;

	//Light distance params
	float lightConstantPower;
	float lightLinearPower;
	float lightQuadraticPower;

	//Reflector cutoff
	float lightCameraCutoff;
	float lightCameraOuterCutoff;

	char sound;
	float rotationSpeed;
};

//Reprezentace grafickeho programu
typedef struct _commonShaderProgram {
	// identifier for the program
	GLuint program;          // = 0; - id programu
	// vertex attributes locations
	GLint posLocation;       // = -1; index lokace
	GLint colorLocation;     // = -1; index barvy
	GLint colorModifierLocation;
	GLint PVMmatrixLocation;    // = -1;
	GLint texCoordLocation;

	GLint useTextureLocation; // = -1; 
	GLint texSamplerLocation; // = -1;
} SCommonShaderProgram;

void drawBox(BoxObject* box, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix,
	SunObject * sun, Camera * camera, std::list<AlarmObject> alarms, AlarmObject* globalAlarm, bool playAlarms);
void drawSquare(SquareObject * square, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix, float colorModifier);
void drawSun(SunObject * sun, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawSkybox(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix);
void drawAlarm(AlarmObject * alarm, const glm::mat4 & viewMatrix, const glm::mat4 & projectionMatrix, Camera * camera);

void drawAssimpModel(Model * model, Camera * camera, std::list<AlarmObject> alarms, bool playAlarms);

void initializeShaderPrograms();
void cleanupShaderPrograms();

void initializeModels();
void cleanupModels();