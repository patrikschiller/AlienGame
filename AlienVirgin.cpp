// AlienVirgin.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Mesh.h"
#include "Model.h"
#include "pch.h"				
#include "pgr.h"
#include "Shader.h"
#include "render.h"
#include "Camera.h"
#include "Program.h"

#include "CosmoSuit.h"
#include "Reflector.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AnimationPlane.h"


//#include "data.h"
//#include "shadersData.h"
#include "spline.h"

#include <iostream>
#include <time.h>
#include <list>
#include <math.h>
#include "irrKlang/include/irrKlang.h"
#pragma comment(lib, "irrKlang/lib/Win32-visualStudio/irrKlang.lib") // link with irrKlang.dll
extern SCommonShaderProgram shaderProgram;

typedef std::list<void *> GameObjectsList;
std::list<AlarmObject> alarmObjs;
AlarmObject * globalAlarm;
GameObjectsList::iterator camerasIterator;
///ViewerObject * viewer;

Camera * camera;
Model * model;
Program * modelProgram;

irrklang::ISoundEngine * soundEngine;

struct GameState {
	int windowWidth;    // set by reshape callback
	int windowHeight;   // set by reshape callback

	//bool freeCameraMode;        // false;
	/*<DEPRE>*///float cameraElevationAngle; // in degrees = initially 0.0f 
	/*<DEPRE>*///float cameraHorizontalAngle;

	bool keyMap[KEYS_COUNT];    // false
	bool threeDMode; //false
	float elapsedTime;
	float timeDelta;
	float ambientLighting;
	bool playAlarm;
} gameState;

struct GameObjects {
	GameObjectsList boxes;
	GameObjectsList squares;
	SunObject * sun;
	Reflector * reflector;

	GameObjectsList reflectors; //Reflector, for modelShader
	GameObjectsList pointlights; //PointLight, for modelShader
	GameObjectsList dirLights; //DirectionalLight, for modelShader

	GameObjectsList alarms;
	GameObjectsList nanosuits;
	GameObjectsList animations;
	GameObjectsList cameras;
} gameObjects;

void cleanUpObjects(void) {
	while (!gameObjects.boxes.empty()) {
		delete gameObjects.boxes.back();
		gameObjects.boxes.pop_back();
	}
	while (!gameObjects.squares.empty()) {
		delete gameObjects.squares.back();
		gameObjects.squares.pop_back();
	}
	///delete gameState.camera;
}

BoxObject * createBox(glm::vec3 pos) {
	BoxObject * boxObj = new BoxObject;

	//boxObj->direction = glm::vec3(0.0f, 0.0f, 0.0f); //Vector
	boxObj->direction = glm::vec3(
		/*(float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),*/1.0f,
		/*(float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),*/0.0f,
		0.0f
	);
	boxObj->direction = glm::normalize(boxObj->direction);
	boxObj->position = glm::vec3(pos.x, pos.y, pos.z); //Point
	boxObj->size = BOX_BASE_SIZE;
	
	return boxObj;
}
SquareObject * createSquare(glm::vec3 pos, float colorMod) {
	SquareObject * squareObj = new SquareObject;

	squareObj->direction = glm::vec3(
		0.0f,
		1.0f,
		0.0f
	);
	squareObj->direction = glm::normalize(squareObj->direction);
	squareObj->position = glm::vec3(pos.x, pos.y, pos.z); //Point
	squareObj->size = SQUARE_BASE_SIZE;
	squareObj->colorMod = colorMod;

	return squareObj;
}
SunObject * createSun(glm::vec3 pos, glm::vec3 dir, float intensity, float size) {
	SunObject * sun = new SunObject;
	sun->direction = glm::normalize(dir);
	sun->position = pos;
	sun->lightIntensity = intensity;
	sun->size = size;
	sun->movingDir = "left";
	return sun;
}

AlarmObject * createAlarm(glm::vec3 position, glm::vec3 initDir, glm::vec3 color, float size) {
	AlarmObject * alarm = new AlarmObject;
	alarm->color = color;
	alarm->direction = initDir;
	alarm->position = position;
	alarm->size = size;
	alarm->angle = 0.0f;

	alarm->lightAmbient = glm::vec3(0.2f, 0.0f, 0.0f);
	alarm->lightDiffuse = glm::vec3(0.9f, 0.0f, 0.0f);
	alarm->lightSpecular = glm::vec3(0.1f, 0.0f, 0.0f);

	alarm->lightConstantPower = 1.0f;
	alarm->lightLinearPower = 0.29f;
	alarm->lightQuadraticPower = 0.032f;

	alarm->lightCameraCutoff = 10.5f;
	alarm->lightCameraOuterCutoff = 15.5f;

	alarm->rotationSpeed = 1.0f;

	alarmObjs.push_back(*alarm);
	globalAlarm = alarm;
	return alarm;
}

void initializeFlatSurface(glm::vec3 initPosition, glm::vec3 direction) { //DEPRE===========================
	///std::cout << "initializeFlatSurface called";
	float initPositionX = initPosition.x - /*1.0f*/ SQUARE_BASE_SIZE * (float)SURFACE_WIDTH_RADIUS/2;
	float initPositionY = initPosition.y -/* 1.0f*/ SQUARE_BASE_SIZE * (float)SURFACE_HEIGHT_RADIUS/2;
	float totalSurface = 2 * SURFACE_HEIGHT_RADIUS * 2 * SURFACE_HEIGHT_RADIUS;
	float colorIncrement = (float)(1/(totalSurface))*1.5f;
	float colorMod = colorIncrement * totalSurface/4;
	for (int i = 0; i < 2*SURFACE_WIDTH_RADIUS; i++) {
		for (int j = 0; j < 2*SURFACE_HEIGHT_RADIUS; j++) {
			/*if ((i + j) % 2 == 0) {
				//colorMod = 0.95;
			}
			else {
				//colorMod = 0.05;
				colorMod -= colorIncrement;
			}*/
			int random = rand() % 2;
			if (random == 0) {
				colorMod -= colorIncrement;
			}
			else {
				colorMod += colorIncrement;
			}
			glm::vec3 position = glm::vec3(initPositionX + (float)(/*1.0f*/SQUARE_BASE_SIZE*2 * i/2), initPositionY + (float)(/*1.0f*/SQUARE_BASE_SIZE*2 * j/2), initPosition.z);
			SquareObject * newSquare = createSquare(position, colorMod);
			gameObjects.squares.push_back(newSquare);
			///std::cout << gameObjects.squares.size();
		}
	}
}
void rotateAlarm(float deltaTime, AlarmObject * alarm) {
	alarm->angle += 1.0f;
	/*alarm->direction = glm::vec3(
		0.0f,
		cos(glm::radians(alarm->angle)),
		sin(glm::radians(alarm->angle))
	);*/
	glm::mat4 dirRotate = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	alarm->direction = glm::vec3(glm::vec4(alarm->direction,0.0f) * dirRotate);
}

void restartModel() {
	camera->position = glm::vec3(1.0f, 2.0f, 0.6f);
}

void restartGame(void) {
	cleanUpObjects();
	//initialize alarms
	for (int i = -5; i < -4; i++) {
		for (int j = 5; j < 6; j+=1) {
			for (int k = 15; k < 16; k++) {
				glm::vec3 pos = glm::vec3(0.5f*i, 0.5f*j, 0.5f*k - 0.1f);
				glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
				glm::vec3 color = glm::vec3(0.1f, 0.0f, 0.0f);
				AlarmObject * alarm = createAlarm(pos, dir, color, 0.2f);
				gameObjects.alarms.push_back(alarm);
			}
		}
	}
	AlarmObject * alarm = createAlarm(glm::vec3(27.8f, 2.6f, 3.12f), glm::vec3(0.002f, 0.00f, 1.0f), glm::vec3(0.1f, 0.0f, 0.0f), 0.2f);
	gameObjects.alarms.push_back(alarm);
	alarm = createAlarm(glm::vec3(39.8f, 3.6f, 2.32f), glm::vec3(0.002f, 0.00f, 1.0f), glm::vec3(0.1f, 0.0f, 0.0f), 0.2f);
	gameObjects.alarms.push_back(alarm);
	alarm = createAlarm(glm::vec3(54.5f, 2.4f, 1.12f), glm::vec3(0.002f, 0.00f, 1.0f), glm::vec3(0.1f, 0.0f, 0.0f), 0.2f);
	gameObjects.alarms.push_back(alarm);
	// initialize boxes
	/*for (int i = 0; i < 1; i++) {
		for (int j = -1; j < 0; j++) {
			for (int k = 7; k < 8; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}*/
	//initialize tunnel
	//LEFT
	for (int i = -7; i < -6; i++) {
		for (int j = 0; j < 15; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//RIGHT
	for (int i = -2; i < -1; i++) {
		for (int j = 0; j < 25; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//BOTTOM
	for (int i = -7; i < -1; i++) {
		for (int j = 0; j < 25; j++) {
			for (int k = 9; k < 10; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//TOP
	for (int i = -7; i < -1; i++) {
		for (int j = 0; j < 25; j++) {
			for (int k = 15; k < 16; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}

	//FAR
	for (int i = -14; i < -1; i++) {
		for (int j = 24; j < 25; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//NEAR
	/*for (int i = -14; i < -1; i++) {
		for (int j = -2; j < -1; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}*/
	/*//LEFT
	for (int i = -14; i < -13; i++) {
		for (int j = -5; j < 25; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//RIGHT
	for (int i = -9; i < -8; i++) {
		for (int j = -5; j < 15; j++) {
			for (int k = 10; k < 15; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//TOP
	for (int i = -14; i < -7; i++) {
		for (int j = -5; j < 25; j++) {
			for (int k = 15; k < 16; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}
	//BOTTOM
	for (int i = -14; i < -7; i++) {
		for (int j = -5; j < 25; j++) {
			for (int k = 9; k < 10; k++) {
				BoxObject* box = createBox(glm::vec3(0.5f*i, 0.5f*j, 0.5f*k));
				gameObjects.boxes.push_back(box);
			}
		}
	}*/
	//initialize sun
	gameObjects.sun = createSun(
		glm::vec3(-3.0f, 1.0f, 6.0f), //position, Height = z
		glm::vec3(-1.0f, -0.0f, -0.5f), //direction
		0.75f,
		0.15f
	);
	//initialize squares
	initializeFlatSurface(glm::vec3(0.1f, -0.1f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	camera->cameraElevationAngle = 0.0f;

	gameObjects.reflector = new Reflector(
		camera->position,
		camera->direction,
		glm::vec3(0.2f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		12.5f,
		20.5f,
		1.0f,
		0.09f,
		0.032f
	);
	//ALarms
	Reflector * newAlarm = new Reflector(

		glm::vec3(27.8f, 2.6f, 3.12f), //position
		glm::vec3(0.002f, 0.00f, 1.0f), //direction

		glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.1f, 0.2f),
		glm::vec3(0.2f, 0.0f, 0.0f),

		12.5f,
		20.5f,
		1.0f,
		0.09f,
		0.032f
	);
	gameObjects.reflectors.push_back(newAlarm);

	newAlarm = new Reflector(

		glm::vec3(39.8f, 3.6f, 2.32f),
		glm::vec3(0.002f, 0.00f, 1.0f),

		glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.1f, 0.2f),
		glm::vec3(0.2f, 0.0f, 0.0f),

		12.5f,
		20.5f,
		1.0f,
		0.09f,
		0.032f
	);
	gameObjects.reflectors.push_back(newAlarm);

	newAlarm = new Reflector(

		glm::vec3(39.8f, 3.6f, 2.32f),
		glm::vec3(0.002f, 0.00f, 1.0f),

		glm::vec3(0.2f, 0.0f, 0.0f),
		glm::vec3(0.7f, 0.1f, 0.2f),
		glm::vec3(0.2f, 0.0f, 0.0f),

		12.5f,
		20.5f,
		1.0f,
		0.09f,
		0.032f
	);
	gameObjects.reflectors.push_back(newAlarm);


	//Point lights
	PointLight * PL = new PointLight(
		gameObjects.sun->position,
		gameObjects.sun->direction,
		glm::vec3(0.2f, 0.5f, 0.5f),
		glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		1.0f,
		0.09f,
		0.032f
	);
	gameObjects.pointlights.push_back(PL);

	DirectionalLight * DL = new DirectionalLight(
		glm::vec3(1.0f, 0.75f, 0.75f),
		glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(0.1f, 0.2f, 0.2f),
		glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3(0.1f, 0.1f, 0.1f)
	);
	DL->initCycle(15.11f, 0.0f);
	gameObjects.dirLights.push_back(DL);

	AnimationPlane * AP = new AnimationPlane(
		glm::vec3(3.0f, 3.0f, 1.2f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.1f,
		"data/Animation/fire12x6.png",
		12 * 6,
		glm::ivec2(12, 6),
		1,
		0.05f
	);
	gameObjects.animations.push_back(AP);

	AP = new AnimationPlane(
		glm::vec3(4.0f, 1.0f, 1.2f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.8f,
		"data/Animation/boom8x8.png",
		64,
		glm::ivec2(8, 8),
		1,
		0.03f
	);
	gameObjects.animations.push_back(AP);

	AP = new AnimationPlane(
		glm::vec3(42.0f, 3.0f, 12.2f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		8.8f,
		"data/Animation/hole2.png",
		1,
		glm::ivec2(1, 1),
		2,
		0.1f
	);
	gameObjects.animations.push_back(AP);

	/// Alternative cameras
	//Cinematic camera
	Camera * newCamera = new Camera(
		glm::vec3(0.0f, 0.0f, 0.0f),
		camera_init.CAMERA_INIT_DIRECTION,
		glm::vec3(0.0f, 0.0f, 1.0f),
		"Driver Cam"
	);
	newCamera->staticCam = true;
	newCamera->cameraHorizontalAngle = camera_init.CAMERA_INIT_VIEWANGLE;
	newCamera->mouseDelta = camera_init.CAMERA_INIT_MOUSE_DELTA;
	newCamera->motionSpeed = camera_init.CAMERA_INIT_MOTION_SPEED;
	newCamera->turnHardLeft(VIEWER_ANGLE_DELTA);
	gameObjects.cameras.push_back(newCamera);

	//Cinematic camera
	newCamera = new Camera(
		glm::vec3(5.0f, 4.0f, 7.0f),
		camera_init.CAMERA_INIT_DIRECTION,
		glm::vec3(0.0f, 0.0f, 1.0f),
		"Cinematic Cam"
	);
	newCamera->staticCam = true;
	newCamera->cameraHorizontalAngle = camera_init.CAMERA_INIT_VIEWANGLE;
	newCamera->mouseDelta = camera_init.CAMERA_INIT_MOUSE_DELTA;
	newCamera->motionSpeed = camera_init.CAMERA_INIT_MOTION_SPEED;
	newCamera->turnHardLeft(VIEWER_ANGLE_DELTA);
	gameObjects.cameras.push_back(newCamera);

	//Static camera
	newCamera = new Camera(
		glm::vec3(5.0f, -8.0f, 8.0f),
		//camera_init.CAMERA_INIT_DIRECTION,
		glm::vec3(1.0f, 1.0f, -0.10f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		"Static Cam"
	);
	newCamera->staticCam = true;
	newCamera->cameraHorizontalAngle = camera_init.CAMERA_INIT_VIEWANGLE;
	newCamera->mouseDelta = camera_init.CAMERA_INIT_MOUSE_DELTA;
	newCamera->motionSpeed = camera_init.CAMERA_INIT_MOTION_SPEED;
	newCamera->turnHardLeft(VIEWER_ANGLE_DELTA);
	gameObjects.cameras.push_back(newCamera);
}

void drawWindowContents(void) {
	//defaultne 2D 
	//paralelni projekce
	int objectId = 0;

	glm::mat4 orthoProjectionMatrix = glm::ortho(
		-SCENE_WIDTH, SCENE_WIDTH,
		-SCENE_HEIGHT, SCENE_HEIGHT,
		-10.0f*SCENE_DEPTH, 10.0f*SCENE_DEPTH
	);
	// static viewpoint - pohled ze shora
	glm::mat4 orthoViewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	// priprava kamery + projekcni transformace ve 2D
	///glm::mat4 viewMatrix = orthoViewMatrix;
	///glm::mat4 projectionMatrix = orthoProjectionMatrix;
	camera->View = orthoViewMatrix;
	camera->Projection = orthoProjectionMatrix;

	if (gameState.threeDMode) { //zpracovani 3D sceny
		camera->updateCamera(gameState.windowWidth, gameState.windowHeight);
	}

	glUseProgram(shaderProgram.program);
	//glUniform1f(shaderProgram.timeLocation, gameState.elapsedTime);// uniform s aktualnim casem
	glUseProgram(0);






	//draw boxes
	for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
		CHECK_GL_ERROR();
		BoxObject* box = (BoxObject*)(*it);
		drawBox(box, camera->View, camera->Projection, gameObjects.sun, camera, alarmObjs, globalAlarm, gameState.playAlarm);
	}
	/*glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	int j = 0;
	int objects = gameObjects.boxes.size();
	for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
		CHECK_GL_ERROR();
		BoxObject* box = (BoxObject*)(*it);
		glStencilFunc(GL_ALWAYS, j + 1, -1);
		drawBox(box, camera->View, camera->Projection, gameObjects.sun, camera, alarmObjs, globalAlarm, gameState.playAlarm);
		j++;
	}
	glDisable(GL_STENCIL_TEST);*/






	//draw sun
	drawSun(gameObjects.sun, camera->View, camera->Projection);

	//draw alarms
	/*for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
		CHECK_GL_ERROR();
		AlarmObject* alarm = (AlarmObject*)(*it);
		drawAlarm(alarm, camera->View, camera->Projection, camera);
	}*/
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	///int j = 0;
	for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
		CHECK_GL_ERROR();
		glStencilFunc(GL_ALWAYS, objectId + 1, -1);
		AlarmObject* alarm = (AlarmObject*)(*it);
		drawAlarm(alarm, camera->View, camera->Projection, camera);
		objectId++;
	}
	glDisable(GL_STENCIL_TEST);

	///drawAssimpModel(model, camera);
	CHECK_GL_ERROR();
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for (GameObjectsList::iterator it = gameObjects.nanosuits.begin(); it != gameObjects.nanosuits.end(); ++it) {
		CHECK_GL_ERROR();
		CosmoSuit * cs = (CosmoSuit*)(*it);
		//drawAlarm(alarm, camera->View, camera->Projection, camera); 
		if (cs->model->modelName == "Button" || cs->model->modelName == "MarsExplorer" || cs->model->modelName == "Alien1") {
			cs->id = objectId;
			glStencilFunc(GL_ALWAYS, objectId + 1, -1);
			cs->Draw(cs->program, camera, gameObjects.reflector, gameObjects.pointlights, gameObjects.dirLights, gameObjects.reflectors, gameState.playAlarm);
			//std::cout << "Adding button with id " << objectId + 1;
			objectId++;
		}
		else {
			glStencilFunc(GL_ALWAYS, 0, -1);
			cs->Draw(cs->program, camera, gameObjects.reflector, gameObjects.pointlights, gameObjects.dirLights, gameObjects.reflectors, gameState.playAlarm);
		}

	}
	CHECK_GL_ERROR();
	glDisable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);

	//draw surface
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
///std::cout << gameObjects.squares.size();
	int i = 0;
	for (GameObjectsList::iterator it = gameObjects.squares.begin(); it != gameObjects.squares.end(); ++it) {
		CHECK_GL_ERROR();
		SquareObject* square = (SquareObject*)(*it);
		///std::cout << "["<< i <<"]Ctverece vykreslen\n";
		//drawSquare(square, viewMatrix, projectionMatrix, square->colorMod);
		drawSquare(square, camera->View, camera->Projection, square->colorMod);
		/*if(i%2 == 0){
			drawSquare(square, viewMatrix, projectionMatrix, square->colorMod);
		}
		else {
			drawSquare(square, viewMatrix, projectionMatrix, 0.95f);
		}
		i++;*/
	}
	glDisable(GL_CULL_FACE);
	CHECK_GL_ERROR();
	//Draw skybox - final draw
	drawSkybox(camera->View, camera->Projection);

	for (GameObjectsList::iterator it = gameObjects.animations.begin(); it != gameObjects.animations.end(); ++it) {
		CHECK_GL_ERROR();
		AnimationPlane * ap = (AnimationPlane*)(*it);
		ap->Draw(ap->program, camera, 1.0f);
	}
}
void displayCallback() {
	GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	glClear(mask);

	drawWindowContents();

	glutSwapBuffers();
}

void updateObjects(float elapsedTime) { //animace
	float timeDelta = 0.1f;
	if (gameObjects.sun->movingDir == "left") {
		if (gameObjects.sun->position.y > -10.0f) {
			gameObjects.sun->position.y -= timeDelta * 2.0f;
		}
		else {
			gameObjects.sun->movingDir = "right";
		}
	}
	if (gameObjects.sun->movingDir == "right") {
		if (gameObjects.sun->position.y < 10.0f) {
			gameObjects.sun->position.y += timeDelta * 2.0f;
		}
		else {
			gameObjects.sun->movingDir = "left";
		}
	}
	for (GameObjectsList::iterator it = gameObjects.pointlights.begin(); it != gameObjects.pointlights.end(); ++it) {
		CHECK_GL_ERROR();
		PointLight * PL = (PointLight*)(*it);
		PL->geometry.position = gameObjects.sun->position;
		PL->geometry.direction = gameObjects.sun->direction;
	}

	for (GameObjectsList::iterator it = gameObjects.animations.begin(); it != gameObjects.animations.end(); ++it) {
		CHECK_GL_ERROR();
		AnimationPlane * ap = (AnimationPlane*)(*it);
		float timeDelta = elapsedTime - ap->geometry.curentTime;

		ap->geometry.curentTime = elapsedTime;
		ap->geometry.timeDelta += timeDelta;
		if (ap->geometry.timeDelta >= ap->geometry.frameDuration) {
			ap->geometry.timeDelta = 0.0f;
			if (ap->geometry.actualFrame  < ap->geometry.totalFrames /*0*/) {
				ap->geometry.actualFrame++;
			}
			else {
				//ap->geometry.actualFrame = ap->geometry.totalFrames;
				ap->geometry.actualFrame = 0;
			}
		}
	}

	for (GameObjectsList::iterator it = gameObjects.nanosuits.begin(); it != gameObjects.nanosuits.end(); ++it) {
		CHECK_GL_ERROR();

		CosmoSuit * cs = (CosmoSuit*)(*it);
		if (cs->move.isMoving && cs->model->modelName == "NewTieFighter") {
			const size_t  curveSize = 16;
			glm::vec3 curveData[] = {
				 glm::vec3(1.0f, 0.0f, 0.9f),
				 glm::vec3(cos45d, sin45d, 0.8f),
				 glm::vec3(0.0f, 1.0f, 0.7f),
				 glm::vec3(-cos45d, sin45d, 0.6f),
				 glm::vec3(-1.0f, 0.0f, 0.5f),
				 glm::vec3(-0.8f, -0.6f, 0.4f),
				 glm::vec3(-0.25f, -0.75f, 0.35f),
				 glm::vec3(0.12f, -0.58f, 0.45f),//8
				 glm::vec3(0.25f, -0.25f, 0.6f),
				 glm::vec3(0.12f, 0.12f, 0.85f),//10
				 glm::vec3(-0.25f, 0.25f, 1.0f),
				 glm::vec3(-0.8f, -0.1f, 1.2f),
				 glm::vec3(-1.0f, -0.5f, 1.25f),
				 glm::vec3(-0.77f, -1.0f, 1.2f),
				 glm::vec3(-0.25f, -1.25f, 1.1f),
				 glm::vec3(0.68f, -.9f, 1.0f),
			};
			cs->move.currentTime = elapsedTime;
			float curveParamT = cs->move.speed * (cs->move.currentTime - cs->move.animationStart);
			cs->geometry.position = cs->geometry.initPosition+ cs->move.evaluateClosedCurve(curveData, curveSize, curveParamT) * 25.0f;
			//glm::vec3 out = cs->move.evaluateClosedCurve(/*cs->move.curveData,*/ curveData,/* cs->move.curveSize*/curveSize, curveParamT);
			///std::cout << "Position vector: [" << out.x << ", " << out.y << " ," << out.z << "] ";
			cs->geometry.direction = normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData,/* cs->move.curveSize*/curveSize, curveParamT));
			if (camera->name == "Driver Cam") {
				camera->position = cs->geometry.position;
				camera->direction = cs->geometry.direction;
			}
			//glm::vec3 out2 = normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData,/* cs->move.curveSize*/curveSize, curveParamT));
			///std::cout << "Direction vector: [" << out2.x << ", " << out2.y << " ," << out2.z << "] \n";
		}
		if (cs->move.isMoving && cs->model->modelName == "MarsExplorer") {
			const size_t  curveSize = 7;
			glm::vec3 curveData2[] = {
			//  glm::vec3(0.00, 0.0,  0.0),

			//  glm::vec3(-0.33,  0.35, 0.0),
			  glm::vec3(-0.5,  0.5, 0.0),
			  glm::vec3(-1.00,  0.0, 0.0),
			  glm::vec3(-0.66, -0.5, 0.0),
			  glm::vec3(-0.0, -0.6, 0.0),
			  glm::vec3(0.66, -0.5, 0.0),
			  glm::vec3(1.00,  0.0, 0.0),
			  glm::vec3(0.66, 0.5, 0.0),
			 // glm::vec3(0.00,  0.0, 0.0),

			 // glm::vec3(0.33,  0.35, 0.0),
			  /*glm::vec3(0.66,  0.35, 0.0),
			  glm::vec3(1.00,  0.0, 0.0),
			  glm::vec3(0.66, -0.35, 0.0),*/
			  //glm::vec3(0.33, -0.35, 0.0)
			};
			cs->move.currentTime = elapsedTime;
			float curveParamT = cs->move.speed * (cs->move.currentTime - cs->move.animationStart);
			cs->geometry.position = cs->geometry.initPosition + cs->move.evaluateClosedCurve(curveData2, curveSize, curveParamT) * 5.0f;
			//glm::vec3 out = cs->move.evaluateClosedCurve(/*cs->move.curveData,*/ curveData2,/* cs->move.curveSize*/curveSize, curveParamT);
			///std::cout << "Position vector: [" << out.x << ", " << out.y << " ," << out.z << "] ";
			cs->geometry.direction = -normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData2,/* cs->move.curveSize*/curveSize, curveParamT));
			//glm::vec3 out2 = normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData2,/* cs->move.curveSize*/curveSize, curveParamT));
			///std::cout << "Direction vector: [" << out2.x << ", " << out2.y << " ," << out2.z << "] \n";
		}
		if (cs->model->modelName == "Viewer" && camera->name == "Cinematic Cam") {
			const size_t  curveSize = 20;
			glm::vec3 curveData2[] = {
			  glm::vec3(-1.0, 0.0, 1.0),
			  glm::vec3(-0.8, -0.23, 1.2),
			  glm::vec3(-0.63, -0.3, 1.33),
			  glm::vec3(-0.3, -0.25, 1.4),
			  glm::vec3(-0.0, -0.17, 1.41),
			  glm::vec3(0.2, 0.06, 1.35),
			  glm::vec3(0.5, 0.07, 1.21),

			  glm::vec3(0.75, 0.13, 1.10),
			  glm::vec3(0.87, 0.25, 1.0),
			  glm::vec3(1.25, 0.0, 0.96),
			  glm::vec3(1.18, -0.25,0.92),

			  glm::vec3(1.0, -0.38, 0.87),
			  glm::vec3(0.8, -0.23, 0.86),
			  glm::vec3(0.6, -0.19, 0.86),
			  glm::vec3(0.32, -0.07,0.88),
			  glm::vec3(0.21, -0.05, 0.90),

			  glm::vec3(0.0, 0.13, 0.93),
			  glm::vec3(-0.25, 0.1, 0.95),
			  glm::vec3(-0.45, 0.18, 0.98),
			  glm::vec3(-0.9, 0.25, 0.99),
			};
			cs->move.currentTime = elapsedTime;
			float curveParamT = cs->move.speed/10 * (cs->move.currentTime - cs->move.animationStart);
			cs->geometry.position = cs->geometry.initPosition + cs->move.evaluateClosedCurve(curveData2, curveSize, curveParamT) * 20.0f;
			glm::vec3 tempDir = -normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData2,/* cs->move.curveSize*/curveSize, curveParamT));
			//cs->geometry.direction = -normalize(cs->move.evaluateClosedCurve_1stDerivative(/*cs->move.curveData,*/ curveData2,/* cs->move.curveSize*/curveSize, curveParamT));
			cs->geometry.direction = glm::vec3(tempDir.x, tempDir.y,/* cs->geometry.direction.z*/ 0.8);
			camera->position = cs->geometry.position;
			camera->direction = -cs->geometry.direction;
		}
	}


	for (GameObjectsList::iterator it = gameObjects.dirLights.begin(); it != gameObjects.dirLights.end(); ++it) {
		CHECK_GL_ERROR();
		DirectionalLight * dl = (DirectionalLight*)(*it);
		if (dl->geometry.doCycle) {
			float timeDelta = elapsedTime - dl->geometry.currentTime;
			dl->geometry.currentTime = elapsedTime;
			if (dl->geometry.degrees < 360) {
				dl->geometry.degrees = dl->geometry.degrees + (timeDelta * dl->geometry.speed);
				//std::cout << "New angle = " << dl->geometry.degrees;
			}
			else {
				dl->geometry.degrees = 0;
			}
			dl->geometry.position = glm::vec3(
				cos(glm::radians(dl->geometry.degrees)),
				sin(glm::radians(dl->geometry.degrees)),
				dl->geometry.position.z
			);
			//std::cout << "Chaning sun position, new angle offset: " <<(timeDelta * dl->geometry.speed )<< " \n";
		}
	}

///
	//
	//
	//
	//

}

void reshapeCallback(int newWidth, int newHeight) {

	gameState.windowWidth = newWidth;
	gameState.windowHeight = newHeight;

	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);
}
bool checkBoxCollisions(glm::vec3 newLoc) {
	for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
		CHECK_GL_ERROR();
		BoxObject* box = (BoxObject*)(*it);
		if (newLoc.x >= box->position.x- box->size - 0.15f && newLoc.x <= box->position.x + box->size/8 + 0.15f
			&& newLoc.y >= box->position.y - box->size - 0.15f && newLoc.y <= box->position.y + box->size/8 + 0.15f
			&& newLoc.z > box->position.z-0.26f && newLoc.z < box->position.z + box->size + 0.26f
			) {
			//std::cout << "new location = [" << newLoc.x << ", " << newLoc.y << ", " << newLoc.z << "] \n";
			//std::cout << "box location = [" << box->position.x << ", " << box->position.y << ", " << box->position.z << "] \n";
			return false;
		}
	}
	return true;
}
void timerCallback(int) {
	gameState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	camera->timeDelta = gameState.elapsedTime - camera->time;
	camera->time = gameState.elapsedTime;

	//Box Movement
	gameState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME); // milliseconds => seconds
	if (gameState.keyMap[KEY_RIGHT_ARROW] == true) {
		for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
			CHECK_GL_ERROR();
			BoxObject* box = (BoxObject*)(*it);
			box->position.x += 0.005f;
		}
		gameState.keyMap[KEY_RIGHT_ARROW] = false;
	}

	if (gameState.keyMap[KEY_LEFT_ARROW] == true) {
		for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
			CHECK_GL_ERROR();
			BoxObject* box = (BoxObject*)(*it);
			box->position.x -= 0.005f;
		}
		gameState.keyMap[KEY_LEFT_ARROW] = false;
	}

	if (gameState.keyMap[KEY_UP_ARROW] == true) {
		for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
			CHECK_GL_ERROR();
			BoxObject* box = (BoxObject*)(*it);
			box->position.y += 0.005f;
		}
		gameState.keyMap[KEY_UP_ARROW] = false;
	}

	if (gameState.keyMap[KEY_DOWN_ARROW] == true) {
		for (GameObjectsList::iterator it = gameObjects.boxes.begin(); it != gameObjects.boxes.end(); ++it) {
			CHECK_GL_ERROR();
			BoxObject* box = (BoxObject*)(*it);
			box->position.y -= 0.005f;
		}
		gameState.keyMap[KEY_DOWN_ARROW] = false;
	}
	if (gameState.playAlarm == true) {
		for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
			CHECK_GL_ERROR();
			AlarmObject* alarm = (AlarmObject*)(*it);
			rotateAlarm(gameState.elapsedTime - camera->time, alarm);
		}
		rotateAlarm(gameState.elapsedTime - camera->time, globalAlarm);
	};

	// 1st person movement
	glm::vec3 futureLocation= camera->position + camera->timeDelta * camera->motionSpeed * camera->movementDirection;
	//FORWARD
	if (gameState.keyMap[KEY_W] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerMoveForward(0.05f);
		if (glm::length(glm::vec2(futureLocation.x, futureLocation.y))<= GAME_RADIUS
			&& checkBoxCollisions(futureLocation)) {
			camera->moveForward(camera->timeDelta * camera->motionSpeed);
		}
		///gameState.keyMap[KEY_W] = false; ///dd
	}
	//BACKWARD
	if (gameState.keyMap[KEY_S] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerMoveBackward(0.05f);
		///camera->moveBackward(camera->timeDelta * camera->motionSpeed);
		if (glm::length(glm::vec2(futureLocation.x, futureLocation.y)) <= GAME_RADIUS
			&& checkBoxCollisions(camera->position - (camera->timeDelta * camera->motionSpeed * camera->movementDirection)))
		{
			camera->moveBackward(camera->timeDelta * camera->motionSpeed);
		}
		///gameState.keyMap[KEY_S] = false;
	}
	//LEFT
	if (gameState.keyMap[KEY_A] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerTurnLeft(VIEWER_ANGLE_DELTA);
		///camera->moveLeft(camera->timeDelta * camera->motionSpeed);
		if (glm::length(glm::vec2(futureLocation.x, futureLocation.y)) <= GAME_RADIUS
			&& checkBoxCollisions(camera->position - (camera->timeDelta * camera->motionSpeed * glm::normalize(glm::cross(camera->movementDirection, camera->upVector)))))
		{
			camera->moveLeft(camera->timeDelta * camera->motionSpeed);
		}
		///camera->turnHardLeft(VIEWER_ANGLE_DELTA);
		///gameState.keyMap[KEY_A] = false;
	}
	//RIGHT
	if (gameState.keyMap[KEY_D] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerTurnRight(VIEWER_ANGLE_DELTA);
		///camera->moveRight(camera->timeDelta * camera->motionSpeed);
		if (glm::length(glm::vec2(futureLocation.x, futureLocation.y)) <= GAME_RADIUS
			&& checkBoxCollisions(camera->position + (camera->timeDelta * camera->motionSpeed * glm::normalize(glm::cross(camera->movementDirection, camera->upVector)))))
		{
			camera->moveRight(camera->timeDelta * camera->motionSpeed);
		}
		///gameState.keyMap[KEY_D] = false;
	}
	//ROTATION LEFT
	if (gameState.keyMap[KEY_Q] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerTurnRight(VIEWER_ANGLE_DELTA);
		//std::cout << "Q pressed";
		camera->turnHardLeft(VIEWER_ANGLE_DELTA);
		///gameState.keyMap[KEY_Q] = false;
	}
	//ROTATION RIGHT
	if (gameState.keyMap[KEY_E] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerTurnRight(VIEWER_ANGLE_DELTA);
		camera->turnHardRight(VIEWER_ANGLE_DELTA);
		///gameState.keyMap[KEY_E] = false;
	}
	//UP
	if (gameState.keyMap[KEY_SPACE] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerMoveUp(0.05f);
		///std::cout << "time delta: " << camera->timeDelta * camera->motionSpeed<<"\n";
		if (camera->position.z < 60.0f && checkBoxCollisions(camera->position + camera->timeDelta * camera->motionSpeed * camera->movementDirection)) {
			camera->moveUp(camera->timeDelta * camera->motionSpeed);
		}
		///gameState.keyMap[KEY_SPACE] = false;
	}
	//DOWN
	if (gameState.keyMap[KEY_LEFT_SHIFT] == true && !camera->staticCam) {
		/*<DEPRE>*///viewerMoveDown(0.05f);
		if (camera->position.z > 0.63f && checkBoxCollisions(camera->position - camera->timeDelta * camera->motionSpeed * camera->movementDirection - glm::vec3(0.0f, 0.0f, 0.3f))) {
			camera->moveDown(camera->timeDelta * camera->motionSpeed);
		}
	}
	// update objects in the scene
	gameObjects.reflector->geometry.position = camera->position;
	gameObjects.reflector->geometry.direction = camera->direction;

	updateObjects(gameState.elapsedTime);
	/*
	UPDATE VSECH OBJEKTU V ZAVISLOSTI NA STAVU STRUKTUR STAVŮ

	*/
	glutTimerFunc(16, timerCallback, 0);
	glutPostRedisplay();
}
void passiveMouseMotionCallback(int mouseX, int mouseY) {

	if (mouseY != gameState.windowHeight / 2 || mouseX != gameState.windowWidth / 2) { //pohnuti z prostredka obrazovky

		float cameraElevationAngleDelta = 0.5f * -(mouseY - gameState.windowHeight / 2);
		float cameraHorizontalAngleDelta = 0.5f * -(mouseX - gameState.windowWidth / 2);
		//float dirLength = sqrt(pow(viewer->direction.x, 2) + pow(viewer->direction.y, 2) + pow(viewer->direction.z, 2));
		//viewer->mouseDelta.x = (mouseX - gameState.windowWidth / 2);
	    //float cameraElevationAngleDelta = dirLength * (mouseY - gameState.windowHeight / 2);
		
		if (fabs(camera->cameraElevationAngle + cameraElevationAngleDelta) < 89) {
			camera->cameraElevationAngle += cameraElevationAngleDelta;
		}
		else {
			cameraElevationAngleDelta = 0.0f;
		}

		if (fabs(camera->cameraHorizontalAngle + cameraHorizontalAngleDelta) < 360) {
			camera->cameraHorizontalAngle += cameraHorizontalAngleDelta;
		}
		else {
			camera->cameraHorizontalAngle = camera->cameraHorizontalAngle + cameraHorizontalAngleDelta - 360;
		}
		//camera->viewAngle += cameraHorizontalAngleDelta;
		camera->rotateByMouse(cameraHorizontalAngleDelta, cameraElevationAngleDelta);
		// set mouse pointer to the window center
		glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);

		glutPostRedisplay();
	}
}
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {

	switch (keyPressed) {
	case 27: // escape
		glutLeaveMainLoop();
		break;
	case 'r': // restart game
		//restartGame();
		restartModel();
		break;
	case 'w': // move forward
		gameState.keyMap[KEY_W] = true;
		break;
	case 'a': // move left
		gameState.keyMap[KEY_A] = true;
		break;
	case 's': // move backward
		gameState.keyMap[KEY_S] = true;
		break;
	case 'd': // move right
		gameState.keyMap[KEY_D] = true;
		break;
	case 'q': // look left
		gameState.keyMap[KEY_Q] = true;
		break;
	case 'e': // look right
		gameState.keyMap[KEY_E] = true;
		break;
	case 'l': // look right
		gameState.keyMap[KEY_L] = true;
		gameState.playAlarm = !gameState.playAlarm;
		if (gameState.playAlarm == true) {
			soundEngine->play2D("data/audio/space_alarm.mp3", GL_TRUE);
			for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
				CHECK_GL_ERROR();
				AlarmObject* alarm = (AlarmObject*)(*it);
				alarm->color.x = 0.9f;
			}
			globalAlarm->color.x = 0.9f;
		}
		else {
			soundEngine->stopAllSounds();
			for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
				CHECK_GL_ERROR();
				AlarmObject* alarm = (AlarmObject*)(*it);
				alarm->color.x = 0.1f;
			}
			globalAlarm->color.x = 0.1f;
		}
		break;
	case ' ':
		gameState.keyMap[KEY_SPACE] = true;
		break;
	case 'c': // switch camera

		//gameState.freeCameraMode = !gameState.freeCameraMode;
		/*gameState.threeDMode = !gameState.threeDMode;
		if (gameState.threeDMode == true) {
			glutPassiveMotionFunc(passiveMouseMotionCallback);
			glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);
		}
		else {
			glutPassiveMotionFunc(NULL);
		}
		break;*/
		if (camera == gameObjects.cameras.back()) {
			Camera *temp = (Camera*)(gameObjects.cameras.back());
			std::cout << "[END] Comparing " << camera->name << " with " << temp->name << "\n";
			camerasIterator = gameObjects.cameras.begin();
			camera = (Camera*)(*(camerasIterator));
		}
		else {
			Camera *temp = (Camera*)(gameObjects.cameras.back());
			std::cout << "[MIDDLE] Comparing " << camera->name << " with " << temp->name << "\n";
			camerasIterator = std::next(camerasIterator);
			camera = (Camera*)(*(camerasIterator));
		}
		std::cout << "[GameState] Using camera <" << camera->name << ">\n";
	default:
		; // printf("Unrecognized key pressed\n");
	}
}

void keyboardUpCallback(unsigned char keyPressed, int mouseX, int mouseY) {

	switch (keyPressed) {
	case 27: // escape
		glutLeaveMainLoop();
		break;
	case 'r': // restart game
		restartGame();
		break;
	case 'w': // move forward
		gameState.keyMap[KEY_W] = false;
		break;
	case 'a': // move left
		gameState.keyMap[KEY_A] = false;
		break;
	case 's': // move backward
		gameState.keyMap[KEY_S] = false;
		break;
	case 'd': // move right
		gameState.keyMap[KEY_D] = false;
		break;
	case 'q': // look left
		gameState.keyMap[KEY_Q] = false;
		break;
	case 'e': // look right
		gameState.keyMap[KEY_E] = false;
		break;
	case 'l': // look right
		gameState.keyMap[KEY_L] = false;
	case ' ':
		gameState.keyMap[KEY_SPACE] = false;
		break;
	}
}
//Callback pro speciálni klávesy
void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {

	/*if (gameState.gameOver == true)
		return;*/

	switch (specKeyPressed) {
	case GLUT_KEY_RIGHT:
		gameState.keyMap[KEY_RIGHT_ARROW] = true;
		break;
	case GLUT_KEY_LEFT:
		gameState.keyMap[KEY_LEFT_ARROW] = true;
		break;
	case GLUT_KEY_UP:
		gameState.keyMap[KEY_UP_ARROW] = true;
		break;
	case GLUT_KEY_DOWN:
		gameState.keyMap[KEY_DOWN_ARROW] = true;
		break;
	case GLUT_KEY_SHIFT_L:
		gameState.keyMap[KEY_LEFT_SHIFT] = true;
		break;
	default:
		; 
	}
}

void specialKeyboardUpCallback(int specKeyReleased, int mouseX, int mouseY) {

	/*if (gameState.gameOver == true)
		return;*/

	switch (specKeyReleased) {
	case GLUT_KEY_RIGHT:
		gameState.keyMap[KEY_RIGHT_ARROW] = false;
		break;
	case GLUT_KEY_LEFT:
		gameState.keyMap[KEY_LEFT_ARROW] = false;
		break;
	case GLUT_KEY_UP:
		gameState.keyMap[KEY_UP_ARROW] = false;
		break;
	case GLUT_KEY_DOWN:
		gameState.keyMap[KEY_DOWN_ARROW] = false;
		break;
	case GLUT_KEY_SHIFT_L:
		gameState.keyMap[KEY_LEFT_SHIFT] = false;
		break;
	default:
		; // printf("Unrecognized special key released\n");
	}
}
void mouseCallback(int buttonPressed, int buttonState, int mouseX, int mouseY) {

	// do picking only on mouse down
	if ((buttonPressed == GLUT_LEFT_BUTTON) && (buttonState == GLUT_DOWN)) {
		unsigned char boxID = 0;	

		glReadPixels(mouseX, mouseY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &boxID);
		if (boxID == 0) {
			// background was clicked
			std::cout << "Clicked on background [" << (int)boxID << "]\n";
		}
		else {
			std::cout << "Clicked on box [" << (int)boxID << "]\n";
			if ((int)boxID == 1 || (int)boxID == 6) {
				gameState.playAlarm = !gameState.playAlarm;
				if (gameState.playAlarm == true) {
					soundEngine->play2D("data/audio/space_alarm.mp3", GL_TRUE);
					for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
						CHECK_GL_ERROR();
						AlarmObject* alarm = (AlarmObject*)(*it);
						alarm->color.x = 0.9f;
					}
					globalAlarm->color.x = 0.9f;
				}
				else {
					soundEngine->stopAllSounds();
					for (GameObjectsList::iterator it = gameObjects.alarms.begin(); it != gameObjects.alarms.end(); ++it) {
						CHECK_GL_ERROR();
						AlarmObject* alarm = (AlarmObject*)(*it);
						alarm->color.x = 0.1f;
					}
					globalAlarm->color.x = 0.1f;
				}
			}
			else if ((int)boxID == 7) {
				for (GameObjectsList::iterator it = gameObjects.nanosuits.begin(); it != gameObjects.nanosuits.end(); ++it) {
					CHECK_GL_ERROR();
					CosmoSuit* cs = (CosmoSuit*)(*it);
					if (cs->model->modelName == "MarsExplorer") {
						if (cs->move.isMoving) {
							cs->move.isMoving = false;
						}
						else {
							cs->move.initMotion(0.2f, 0.001f * (float)glutGet(GLUT_ELAPSED_TIME));
						}
					}
				}
			}
			else if ((int)boxID == 5) {
				std::cout << "Alien \n";
				int randomSound = rand() % 3;
				switch (randomSound) {
				case 0:
					//soundEngine->stopAllSounds();
					soundEngine->play2D("data/audio/xeno1.mp3", GL_FALSE);
					break;
				case 1:
					//soundEngine->stopAllSounds();
					soundEngine->play2D("data/audio/xeno2.mp3", GL_FALSE);
					break;
				case 2:
					//soundEngine->stopAllSounds();
					soundEngine->play2D("data/audio/xeno3.mp3", GL_FALSE);
					break;
				}
			}
		}
	}
}

//Inicializační metoda, zavolána pouze jednou
void initializeApplication() {
	std::cout << "Init called\n";
	// initialize random seed
	srand((unsigned int)time(NULL));

	// initialize OpenGL
	glClearColor(0.40f, 0.15f, 0.15f, 0.0f);
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);

	std::string modelPath = "data/crysis/source/scene.fbx";
	std::string modelPath2 = "data/FortVertices1.obj";
	std::string modelPath3 = "data/cottage_obj.obj";
	std::string modelPath4 = "data/nanosuit/nanosuit.obj";
	std::string modelPath5 = "data/dwarf/dwarf.x";
	CHECK_GL_ERROR();
	model = new Model(modelPath4, "NanoSuit00");
	CHECK_GL_ERROR();
	//const char * VS = "shaders/modelShader.vert";
	//const char * FS = "shaders/modelShader.frag";
	//modelProgram = new Program(VS, FS, "MODEL");

	/// --- render.cpp
		// initialize shaders
	initializeShaderPrograms();

	modelPath = "data/nanosuit/nanosuit.obj";
	Model * newModel = new Model(modelPath, "NanoSuit1");
	CosmoSuit * cs = new CosmoSuit(newModel, glm::vec3(-2.6f, 6.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 1.0f));
	cs->geometry.size = 0.12f;
	gameObjects.nanosuits.push_back(cs);

	modelPath = "data/nanosuit/nanosuit.obj";
	newModel = new Model(modelPath, "NanoSuit1");
	cs = new CosmoSuit(newModel, glm::vec3(27.6f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 1.0f));
	cs->geometry.size = 0.09f;
	gameObjects.nanosuits.push_back(cs);

	/*modelPath = "data/cottage_obj.obj";
	newModel = new Model(modelPath);
	//cs = new CosmoSuit(newModel, glm::vec3(1.0f, 3.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) /*glm::vec3(0.2f, 0.2f, 1.0f)*///);
	//gameObjects.nanosuits.push_back(cs);*/

	modelPath = "data/alien/alien.obj";
	newModel = new Model(modelPath, "Alien1");
	cs = new CosmoSuit(newModel, glm::vec3(-2.6f, 8.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f) /*glm::vec3(0.2f, 0.2f, 1.0f)*/);
	cs->geometry.size = 0.8f;
	gameObjects.nanosuits.push_back(cs);
	/*
	modelPath = "data/dwarf/dwarf.x";
	newModel = new Model(modelPath, "Dwarf1");
	cs = new CosmoSuit(newModel, glm::vec3(-2.6f, 4.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
	cs->geometry.size = 0.02;
	cs->geometry.color = glm::vec3(0.62f, 0.62f, 0.61f);
	gameObjects.nanosuits.push_back(cs);*/

	modelPath = "data/FortVertices1.obj";
	newModel = new Model(modelPath, "Fort");
	cs = new CosmoSuit(newModel, glm::vec3(-30.6f, -10.0f, -0.11f), glm::vec3(0.0f, 1.0f, 0.0f) /*glm::vec3(0.2f, 0.2f, 1.0f)*/);
	cs->geometry.size = 0.04;
	cs->geometry.color = glm::vec3(0.99f, 0.0f, 0.0f);
	gameObjects.nanosuits.push_back(cs);

	/*modelPath = "data/Viper/Viper-mk-IV-fighter.obj";
	newModel = new Model(modelPath, "Viper");
	cs = new CosmoSuit(newModel, glm::vec3(4.0f, -5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.0f) );
	cs->geometry.size = 0.8;
	cs->geometry.color = glm::vec3(1.0f, 0.1f, 0.1f);
	gameObjects.nanosuits.push_back(cs);*/

	modelPath = "data/FirstOrderFighter/NewTieFighter.obj";
	newModel = new Model(modelPath, "NewTieFighter");
	cs = new CosmoSuit(newModel, glm::vec3(4.0f, -5.0f, -3.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	cs->geometry.size = 0.8;
	cs->geometry.color = glm::vec3(0.72f, 0.7f, 0.65f);
	cs->geometry.initPosition = cs->geometry.position;
	cs->move.initMotion(0.2f, 0.0f);
	gameObjects.nanosuits.push_back(cs);

	modelPath = "data/Box/Sci-gi Box_low.obj";
	newModel = new Model(modelPath, "Box");
	cs = new CosmoSuit(newModel, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	cs->geometry.size = 0.01;
	cs->geometry.color = glm::vec3(0.8f, 0.6f, 0.6f);
	gameObjects.nanosuits.push_back(cs);

	modelPath = "data/Button/emergency_button.obj";
	newModel = new Model(modelPath, "Button");
	cs = new CosmoSuit(newModel, glm::vec3(-1.5f, 1.0f, 6.0f), glm::vec3(0.002f, 0.00f, 1.0f));
	cs->geometry.size = 0.02;
	cs->geometry.color = glm::vec3(1.0f, 0.1f, 0.1f);
	gameObjects.nanosuits.push_back(cs);

	modelPath = "data/MarsExplorer/Mars_Explorer.obj";
	newModel = new Model(modelPath, "MarsExplorer");
	cs = new CosmoSuit(newModel, glm::vec3(19.6f, 2.9f, -0.2f), glm::vec3(1.0f, 0.00f, 0.0f));
	cs->geometry.size = 0.01;
	cs->geometry.color = glm::vec3(1.0f, 0.1f, 0.1f);
	cs->geometry.initPosition = cs->geometry.position;
	cs->move.initMotion(0.6f, 0.0f);
	gameObjects.nanosuits.push_back(cs);
	
	modelPath = "data/Button/emergency_button.obj";
	newModel = new Model(modelPath, "Viewer");
	cs = new CosmoSuit(newModel, glm::vec3(-1.5f, -2.0f, 4.0f), glm::vec3(0.002f, 0.00f, 1.0f));
	cs->geometry.size = 0.00;
	cs->geometry.color = glm::vec3(1.0f, 0.1f, 0.1f);
	cs->move.initMotion(0.6f, 0.0f);
	gameObjects.nanosuits.push_back(cs);

	modelPath = "data/Landscape/rock.obj";
	newModel = new Model(modelPath, "Terrain");
	cs = new CosmoSuit(newModel, glm::vec3(-28.5f, -28.0f, -0.8f), glm::vec3(1.0f, 0.0f, 0.0f));
	cs->geometry.size = 0.08;
	cs->geometry.color = glm::vec3(0.75f, 0.75f, 0.0f);
	gameObjects.nanosuits.push_back(cs);
	// create geometry for all hardcoded models used
	initializeModels();
	///---------------	
	//<<Setup camera>>
	Camera * newCamera = new Camera(
		camera_init.CAMERA_INIT_POSITION,
		camera_init.CAMERA_INIT_DIRECTION,
		glm::vec3(0.0f, 0.0f, 1.0f),
		"Free Cam"
	);
	newCamera->cameraHorizontalAngle = camera_init.CAMERA_INIT_VIEWANGLE;
	newCamera->mouseDelta = camera_init.CAMERA_INIT_MOUSE_DELTA;
	newCamera->motionSpeed = camera_init.CAMERA_INIT_MOTION_SPEED;
	newCamera->turnHardLeft(VIEWER_ANGLE_DELTA);
	gameObjects.cameras.push_back(newCamera);

	camera = newCamera;
	camerasIterator = gameObjects.cameras.begin();

	restartGame();

	gameState.threeDMode = !gameState.threeDMode;
	if (gameState.threeDMode == true) {
		glutPassiveMotionFunc(passiveMouseMotionCallback);
		glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);
	}
	else {
		glutPassiveMotionFunc(NULL);
	}
	soundEngine = irrklang::createIrrKlangDevice();

	std::cout << "Init ended\n";
}

void finalizeApplication(void) {

	cleanUpObjects();

	// delete buffers 
	cleanupModels();

	// delete shaders
	cleanupShaderPrograms();
	glutSetCursor(GLUT_CURSOR_INHERIT);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv); //GLUT inicializace

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); //RGB, dva buffery,+ buffer hloubky,+ buffer sablony

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(WINDOW_TITLE);

	glutSetCursor(GLUT_CURSOR_NONE);
	//CALLBACKY
	glutDisplayFunc(displayCallback); //vykresleni
	glutReshapeFunc(reshapeCallback); //zmena velikosti

	//Callbacky inputu 
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	//glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialKeyboardCallback);     // key pressed
	glutSpecialUpFunc(specialKeyboardUpCallback); // key released

	glutMouseFunc(mouseCallback); //mys

	glutTimerFunc(33, timerCallback, 0);
	//inicializace pgr frameworku
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	initializeApplication();

	glutCloseFunc(finalizeApplication);

	glutMainLoop();

	return 0;
    std::cout << "Hello World!\n"; 
}