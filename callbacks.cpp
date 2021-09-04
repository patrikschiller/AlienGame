#include "callbacks.h"
void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {

	switch (keyPressed) {
	case 27: // escape
		glutLeaveMainLoop();
		break;
	case 'r': // restart game
		restartGame();
		break;
	case ' ': // launch missile
		if (gameState.gameOver != true)
			gameState.keyMap[KEY_SPACE] = true;
		break;
	case 't': // teleport space ship
		if (gameState.gameOver != true)
			teleport();
		break;
	case 'c': // switch camera
		gameState.freeCameraMode = !gameState.freeCameraMode;
		if (gameState.freeCameraMode == true) {
			glutPassiveMotionFunc(passiveMouseMotionCallback);
			glutWarpPointer(gameState.windowWidth / 2, gameState.windowHeight / 2);
		}
		else {
			glutPassiveMotionFunc(NULL);
		}
		break;
	default:
		; // printf("Unrecognized key pressed\n");
	}
}