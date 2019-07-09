//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Model.h
// Description	: this is a factory class that creates several types of models
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __MODEL_H__
#define __MODEL_H__

#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <vector>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"
#include "TextureHelper.h"

class CObject
{
public:
	virtual ~CObject();

	// Creates a model based on the input
	// Takes what type of model to create, what shaders the object shuld use and its position in world space
	// Returns the created model
	static CObject* CreateObject(ModelType _modelType, GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	// Sends the necessary data to the shaders for them to draw the model
	virtual void DrawObject() = 0;
	ModelType GetModelType();
	// A get function for the models position
	// Returns the position in the form of a glm::vec3
	glm::vec3 GetPosition();

protected:
	CObject();

	glm::vec3 m_position;
	glm::vec3 m_rotation;

	ModelType m_eModelType;
};

#endif //__MODEL_H__