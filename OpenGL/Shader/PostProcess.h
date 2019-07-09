//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: PostProcess.h
// Description	: A handler for post processing
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __POSTPROCESS_H__
#define __POSTPROCESS_H__

#include <glew.h>
#include <freeglut.h>
#include <assert.h>
#include <vector>
#include "Utils.h"


class CPostProcess
{
public:
	CPostProcess(GLuint _shaders);
	~CPostProcess();

	void PreRender();
	void PostRender();
	// Swaps what type of shader is displayed by incrementing a intiger sent to the shader
	void SwapMode();
private:
	void SetupFrameBuffer();
	// Gets the location of all data to send to the shader
	void GetUniformLocations();
	// Generates a quad that covers the screen
	void CreateQuad();

	GLuint frameBuffer;
	GLuint renderTexture, depthTexture;
	GLuint rbo, depthRbo;

	GLuint shaders;
	GLuint gSampler, gDepthSampler;
	GLuint quadVAO, VBO, EBO;
	GLuint aPositionLocation, aTextureLocation, aModeLocation, inverseViewProjectionLocation, previousViewProjectionLocation;
	int m_iMode;
	glm::mat4 inverseViewProjection;
	glm::mat4 previousViewProjection;
};

#endif //__POSTPROCESS_H__