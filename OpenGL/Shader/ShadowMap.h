//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: ShadowMap.h
// Description	: The shadow map frame buffer object
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//
#ifndef __SHADOWMAP_H__
#define __SHADOWMAP_H__

#include <glew.h>
#include <freeglut.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"

class CShadowMap
{
public:
	// Initializer that creates the texture and frame buffer
	CShadowMap();
	~CShadowMap();

	// Begins drawing to the shadow maps depth buffer
	void StartShadowMap();
	// Ends drawing to the shadow maps depth buffer
	void EndShadowMap();
	static GLuint depthMapTexture;
	static glm::mat4 lightViewMatrix;
	static glm::vec3 lightPosition;

private:
	GLuint depthMapFBO;
};

#endif // __SHADOWMAP_H__