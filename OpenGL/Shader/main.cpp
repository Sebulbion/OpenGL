//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: main.cpp
// Description	: the main loop of the program. Where the code starts
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include <glew.h>
#include <freeglut.h>
#include <iostream>
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#include <vld.h>

#include "Utils.h"
#include "Game.h"

unsigned char keyState[255];
unsigned char mouseState[3];
CGame* game;

// Clears and draws on the backbuffer followed by swapping them
static void RenderSceneCB()
{
	game->RenderScene();
}

// Populates a array with the normal keys pressed
// Takes the pressed key and (for whatever reason) the mouse position
void keyboard(unsigned char key, int x, int y) 
{
	keyState[key] = BUTTON_DOWN;
}
// Populates a array with the normal keys released
// Takes the released key and (for whatever reason) the mouse position
void keyboard_up(unsigned char key, int x, int y) 
{
	keyState[key] = BUTTON_UP;
}
// Populates a array with the mouse buttons pressed
// Takes the pressed button and the mouse position
void mouse(int button, int button_state, int x, int y) 
{
#define state ( (button_state== GLUT_DOWN) ? BUTTON_DOWN : BUTTON_UP )
	switch (button) {
	case GLUT_LEFT_BUTTON:
		mouseState[MOUSE_LEFT] = state; 
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseState[MOUSE_MIDDLE] = state; 
		break;
	case GLUT_RIGHT_BUTTON:
		mouseState[MOUSE_RIGHT] = state; 
		break;
	}
}
// Moves the camera
// Takes the mouse position
void mousePassiveMove(int x, int y) 
{
	game->RotateCamera(x, y);
}

// The function that is called when the window is not changing
static void update()
{
	glutPostRedisplay();
	game->Update(keyState, mouseState);
}

// Initializes the functions that will be passively called
static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(update);
}

// The entry point of the program
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL");

	InitializeGlutCallbacks();

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Error: ," << glewGetErrorString(res);
		return 1;
	}

	game = CGame::GetInstance();
	game->Initialize();

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutMouseFunc(mouse);
	glutMotionFunc(mousePassiveMove);
	glutPassiveMotionFunc(mousePassiveMove);

	glutMainLoop();
	game->DestroyInstance();
	//_CrtDumpMemoryLeaks();
	return 0;
}