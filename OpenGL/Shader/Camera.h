//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Camera.h
// Description	: this is the camera object containing related functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glew.h>
#include <freeglut.h>
#include <assert.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class CCamera
{
public:
	CCamera();
	~CCamera();
	
	// Gets the location of variables in the shader for later editiong. Creates a CShader object that contains the shaders and the view type
	// Takes the shaders to send camera data to and what view type that data needs to be
	void GetUniformLocation(GLuint _shaders, bool _bIsOrthographic);
	// Sends camera data to all the shaders
	void SendDataToShaders();
	// Moves the camera forward
	// Takes what speed to move the camera by
	void MoveForward(float _fSpeed);
	// Moves the camera left
	// Takes what speed to move the camera by
	void MoveLeft(float _fSpeed);
	// Sets the cameras front vector
	// Takes the new front vector
	void SetCameraFront(glm::vec3 _front);
	// Sets the cameras position
	// Takes the new position
	void SetPosition(glm::vec3 _position);
	// Get function for the camera front vector
	// Returns the camera front vector in the form of glm::vec3
	static glm::vec3 GetFront();
	// Get function for position
	// Returns the position in the form of glm::vec3
	static glm::vec3 GetPosition();

	static glm::mat4 getProjectionMatrix();
	static glm::mat4 getViewMatrix();

private:
	std::vector<CShader> vecShaderFunctions;
	
	static glm::vec3 m_cameraPos;
	static glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;

	static glm::mat4 View;
	static glm::mat4 Perspective;
};

#endif //__CAMERA_H__