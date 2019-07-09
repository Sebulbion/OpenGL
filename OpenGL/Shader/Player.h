//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Player.h
// Description	: A wrapper to make the animated model work with the framework
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Object.h"
#include "AnimatedModel.h"

class CPlayer :
	public CObject
{
public:
	CPlayer(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	~CPlayer();

	void DrawObject() override;
	// Renders the object for the shadow map
	void ShadowPass();

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CPlayer* CreatePlayer(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	// Moves the player in the localSpace direction sent in (ie. rotated with the player)
	// Takes two movement vectors
	void Move(float _fDeltaX, float _fDeltaZ);
	// Rotates the player in the Y axis by the ammount given in degrees
	// Takes the rotation in degrees
	void Rotate(float _fDeltaRotation);
	// Starts a jump which sets variables
	// Takes the player movement when called
	void Jump(glm::vec2 _lastMovement);
	// A getter function for jumping
	// Returns if the player is in a jump
	bool GetIsJumping();
	// Updates the player. Moves the player forward in the jump cycle
	void Update();

	ssAnimatedModel m_model;
private:
	// Lerps the player to his possition on the terrain
	void CalculateTerrainPos();

	GLuint gScaleLocation, gRotateLocation, gTranslateLocation, currentTimeLocation;
	GLuint m_shaders, m_shadowShaders;
	//Texture* pTexture;
	GLfloat currentTime;
	bool m_bJumping;
	float m_percentage;
	glm::vec3 preJumpPosition;
	glm::vec2 m_lastMovement;

	int m_iIndices;
};

#endif // __SPHERE_H__