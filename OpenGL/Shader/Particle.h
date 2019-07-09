//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Particle.h
// Description	: A compute shader particle system
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//
#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "Object.h"

class CParticleSystem :
	public CObject
{
public:
	CParticleSystem(GLuint _shaders, glm::vec3 _position);
	~CParticleSystem();

	void DrawObject() override;

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CParticleSystem* CreateParticleSystem(GLuint _shaders, glm::vec3 _position);
private:
	// Gets the location of some variables for the shader for later edditiong
	void getUniformLocation();
	// Sets the initial values for all the particles
	void InitializeVectors();
	// Initializes all the vbos that get used for sending and retreving data
	void InitializeVBOs();

	GLuint gScaleLocation, gRotateLocation, gTranslateLocation, currentTimeLocation;
	GLuint m_shaders;
	GLuint computeShader;
	//Texture* pTexture;
	GLfloat currentTime;

	std::vector<glm::vec4> initialPosition;
	std::vector<glm::vec4> initialVelocity;
	GLuint posVbo, initPosVbo, velVbo, initVelVbo, particleVao;
	GLuint vpLocation, modelLocation;

	int m_iIndices;
};

#endif // __SPHERE_H__