//----------------------------------------------------------------------------------------
/**
 * \file       Data.h
 * \author     Patrik Schiller
 * \date       2019/21/05
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#define SCREEN_WIDTH 1520//1280//720
#define SCREEN_HEIGHT 800//720//720
#define WINDOW_TITLE "Alien Game"

#define M_PI 3.14159f
#define BOX_BASE_SIZE 0.5f

#define SCENE_WIDTH  2.0f
#define SCENE_HEIGHT 2.0f
#define SCENE_DEPTH  2.0f

#define SURFACE_WIDTH_RADIUS 10
#define SURFACE_HEIGHT_RADIUS 10
#define SQUARE_BASE_SIZE 10.0f
#define GAME_RADIUS 60.0f

#define VIEWER_ANGLE_DELTA 3.0f

//Default Camera
const struct CAMERA_INIT {
	const glm::vec3 CAMERA_INIT_POSITION = glm::vec3(-2.5f, -3.0f, 6.0f);
	const glm::vec3 CAMERA_INIT_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
	const float CAMERA_INIT_VIEWANGLE = 90.0f; //degrees
	const glm::vec2 CAMERA_INIT_MOUSE_DELTA = glm::vec2(0.1f, 0.0f);
	const float CAMERA_ROTATION_SPEED = 0.5f;
	const float CAMERA_INIT_MOTION_SPEED = 3.10f;
	const float CAMERA_MAX_SPEED = 0.25f;
}camera_init;

//special keymap
enum {
	KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_UP_ARROW, KEY_DOWN_ARROW, //model rotation
	KEY_W, KEY_A, KEY_S, KEY_D, KEY_SPACE, KEY_LEFT_SHIFT, //1st person movement
	KEY_Q, KEY_E, KEY_L, //camera rotation in XZ coordinates by keyboard
	KEYS_COUNT 
};
//movement keymap
//enum movementKeys{KEY_W, KEY_A, KEY_S, KEY_D};

const float cos30d = (float)cos(M_PI / 6.0);
const float sin30d = (float)sin(M_PI / 6.0);

const float cos45d = (float)cos(M_PI / 4.0);
const float sin45d = (float)sin(M_PI / 4.0);

const float boxH = 1.0f;
//const float boxSize = 4.0f;
/*const float boxVertices[] = {
	//  x      y        z      r     g     b        nx      ny      nz
	///FRONT
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///RIGHT
	 1.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///BACK
	 1.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 0.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 0.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 0.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 1.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///LEFT
	 0.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 0.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 0.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///TOP
	 0.0f,  1.0f,  0.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  1.0f,  0.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///BOTTOM
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  0.0f,  1.0f,  0.1f, 0.1f, 0.6f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
};*/
const float boxVertices[] = { ///Souradnice pocita zeshora!!
	//  x      y        z      r     g     b        nx      ny      nz
	///FRONT V
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.5f,	0.0f, 0.0f, -1.0f, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  0.25f, 0.5f,	0.0f, 0.0f, -1.0f, // 1
	 1.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,	  0.25f, 0.25f,   0.0f, 0.0f, -1.0f, // 2

	 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,	  0.25f, 0.25f,   0.0f, 0.0f, -1.0f,  // 3
	 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 0.25f,   0.0f, 0.0f, -1.0f, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.5f,   0.0f, 0.0f, -1.0f, // 5
	 ///RIGHT
	 1.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.25f, 0.5f,	1.0f, 0.0f, 0.0f, // 0
	 1.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,	  0.25f, 0.75f,	1.0f, 0.0f, 0.0f, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  0.5f, 0.75f,   1.0f, 0.0f, 0.0f, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  0.5f, 0.75f,   1.0f, 0.0f, 0.0f, // 3
	 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,	  0.5f, 0.5f,   1.0f, 0.0f, 0.0f, // 4
	 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.25f, 0.5f,   1.0f, 0.0f, 0.0f, // 5
	 ///BACK V
	 1.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,	  0.25f, 0.75f,	0.0f, 0.0f, 1.0f, // 0
	 0.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,	  0.0f, 0.75f,	0.0f, 0.0f, 1.0f, // 1
	 0.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  0.0f, 1.0f,   0.0f, 0.0f, 1.0f, // 2

	 0.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  0.0f, 1.0f,   0.0f, 0.0f, 1.0f, // 3
	 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.25f, 1.0f,   0.0f, 0.0f, 1.0f, // 4
	 1.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,	  0.25f, 0.75f,   0.0f, 0.0f, 1.0f, // 5
	 ///LEFT
	 0.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,	  1.0f, 0.75f,	-1.0f, 0.0f, 0.0f, // 0
	 0.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.5f,	-1.0f, 0.0f, 0.0f, // 1
	 0.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,	  0.75f, 0.5f,  -1.0f, 0.0f, 0.0f, // 2

	 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,	  0.75f, 0.5f,   -1.0f, 0.0f, 0.0f, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.75f, 0.75f,   -1.0f, 0.0f, 0.0f, // 4
	 0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,	  1.0f, 0.75f,   -1.0f, 0.0f, 0.0f, // 5
	 ///TOP
	 0.0f,  1.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.5f, 0.5f,	0.0f, 1.0f, 0.0f, // 0
	 1.0f,  1.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  0.5f, 0.75f,	0.0f, 1.0f, 0.0f, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  0.75f, 0.75f,   0.0f, 1.0f, 0.0f, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  0.75f, 0.75f,   0.0f, 1.0f, 0.0f, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.75f, 0.5f,   0.0f, 1.0f, 0.0f, // 4
	 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.5f, 0.5f,   0.0f, 1.0f, 0.0f, // 5
	 ///BOTTOM
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.25f, 0.5f,	0.0f, -1.0f, 0.0f, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  0.0f, 0.5f,	0.0f, -1.0f, 0.0f, // 1
	 1.0f,  0.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  0.0f, 0.75f,   0.0f, -1.0f, 0.0f, // 2

	 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  0.0f, 0.75f,   0.0f, -1.0f, 0.0f, // 3
	 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.25f, 0.75f,   0.0f, -1.0f, 0.0f, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.25f, 0.5f,   0.0f, -1.0f, 0.0f, // 5
};
/*const float boxVertices[] = {
	//  x      y        z      r     g     b        nx      ny      nz
	///FRONT
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///RIGHT
	 1.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///BACK
	 1.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 0.0f,  0.0f,  1.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 0.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 0.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 1.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///LEFT
	 0.0f,  0.0f,  1.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 0.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 0.0f,  1.0f,  0.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///TOP
	 0.0f,  1.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  1.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  1.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  1.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
	 ///BOTTOM
	 0.0f,  0.0f,  0.0f,  0.6f, 0.1f, 0.1f,	  0.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	 1.0f,  0.0f,  0.0f,  0.1f, 0.6f, 0.1f,	  1.0f, 0.0f,	boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	 1.0f,  0.0f,  1.0f,  0.1f, 0.1f, 0.6f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	 1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,	  1.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 3
	 0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,	  0.0f, 1.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 4
	 0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,	  0.0f, 0.0f,   boxH*cos30d, -1.0f,  boxH*sin30d, // 5
};*/

const float cubeTextureCoords[] = { ///Not Used
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

const float flatSurfaceElementVertices[] = {
	0.0f,  0.0f,  0.0f,  0.0f, 0.6f, 0.6f,  0.0f, 0.5f,  boxH*cos30d, -1.0f,  boxH*sin30d, // 0
	1.0f,  0.0f,  0.0f,  0.0f, 0.6f, 0.6f,  0.25f, 0.5f,  boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	1.0f,  0.0f,  1.0f,  0.0f, 0.6f, 0.6f,  0.25f, 0.25f,  boxH*cos30d, -1.0f,  boxH*sin30d, // 2

	1.0f,  0.0f,  1.0f,  0.0f, 0.6f, 0.6f,  0.25f, 0.25f,  boxH*cos30d, -1.0f,  boxH*sin30d, // 1
	0.0f,  0.0f,  1.0f,  0.0f, 0.6f, 0.6f,  0.0f, 0.25f,  boxH*cos30d, -1.0f,  boxH*sin30d, // 2
	0.0f,  0.0f,  0.0f,  0.0f, 0.6f, 0.6f,  0.0f, 0.5f,  boxH*cos30d, -1.0f,  boxH*sin30d // 3
};

const float sunHeight = 1.5f;
const float sunObjectVertices[] = {
	//Coords
	///Front
	0.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	0.5f,  sunHeight,  0.5f,
	///Right
	1.0f,  0.0f,  0.0f,
	1.0f, 0.0f, 1.0f,
	0.5f,  sunHeight,  0.5f,
	///Back
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.5f,  sunHeight,  0.5f,
	///Left
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
	0.5f,  sunHeight,  0.5f
};

const float alarmLightWidth = 1.0f;
const float alarmObjectVertices[] = { //stred ve vrcholu jehlanu
	//Coords
	///Front
	0.0f,  0.0f,  0.0f,
	alarmLightWidth, -0.5f,  -0.5f,
	alarmLightWidth, -0.5f,  0.5f,
	///Bottom
	0.0f,  0.0f,  0.0f,
	alarmLightWidth, 0.5f, -0.5f,
	alarmLightWidth,  -0.5, -0.5f,
	///Back
	0.0f, 0.0f, 0.0f,
	alarmLightWidth, 0.5f, 0.5f,
	alarmLightWidth,  0.5f, -0.5f,
	///Top
	0.0f, 0.0f, 0.0f,
	alarmLightWidth, -0.5f, 0.5f,
	alarmLightWidth,  0.5f,  0.5f
};
const float skyboxObjectVertices[] = {         
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
const float animationVertices[] = {
	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};
//ASYMP