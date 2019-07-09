//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Game.h
// Description	: this is the singelton main game scene
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __GAME_H__
#define __GAME_H__

#include <glew.h>
#include <freeglut.h>
#include <vector>

#include "ShaderHelper.h"
#include "TextureHelper.h"
#include "Object.h"
#include "Utils.h"
#include "Camera.h"
#include "Skybox.h"
#include "Square.h"
#include "PostProcess.h"
#include "ShadowMap.h"

class CGame
{
public:
	~CGame();
	// The singelton get function. Creates the singelton if it doesn't exist
	// Returns the singelton object
	static CGame* GetInstance();
	// The singelton destroy function. Destroys the singelton if it exists
	static void DestroyInstance();
	// Sends all the necessary data to all the shaders to let them draw the objects. Is called once per frame
	void RenderScene();
	// Updates the game scene. Is called once per frame
	// Takes two array pointers for the mouse and keyboard input
	void Update(unsigned char* _keyState, unsigned char* _mouseState);
	// Sets everything to their default values
	void Initialize();
	// Rotates the camera
	// Takes the position of the mouse and compares it to the position the previous call
	void RotateCamera(int _iX, int _iY);

private:
	CGame();

	// Calls all the models' creation functions
	void CreateModels();
	// Moves the camera based on the keyboard input
	// Takes a array pointer for the keyboard input
	void MoveCamera(unsigned char* _keyState);
	void ObjectOperations(unsigned char* _keyState);

	static CGame* m_gamInstance;
	std::vector<CObject*> m_vecObjects;
	std::vector<GLuint> m_vecShaders;
	std::vector<GLuint> m_vecShadowShaders;
	CCamera mainCamera;

	int m_iLastX;
	int m_iLastY;
	GLfloat yaw;
	GLfloat pitch;

	CPostProcess* postProcess;
	CShadowMap* shadowMap;
	GLuint postProcessingShader;

	bool m_bEPressed;
};

#endif //__GAME_H__

