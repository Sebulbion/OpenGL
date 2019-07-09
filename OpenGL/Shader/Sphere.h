//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Sphere.h
// Description	: A sphere
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Object.h"
#include "Model.h"

class CSphere :
	public CObject
{
public:
	CSphere(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	~CSphere();

	void DrawObject() override;
	void ShadowPass();

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CSphere* CreateSphere(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	void Move(float _fDeltaX, float _fDeltaZ);
private:
	// Gets the location of some variables for the shader for later edditiong
	void getUniformLocation();
	void CalculateTerrainPos();

	GLuint modelLocation, gRotateLocation, gTranslateLocation, currentTimeLocation, vpLocation, lightPosLocation, lightVpLocation;
	GLuint m_shaders;
	GLuint m_shadowShaders;

	//Texture* pTexture;
	Model m_model;
	GLfloat currentTime;

	int m_iIndices;
};

#endif // __SPHERE_H__