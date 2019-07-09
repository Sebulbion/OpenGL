//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Pyramid.cpp
// Description	: this is the floor object with all relevant functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Floor.h"


CFloor::CFloor(GLuint _shaders, glm::vec3 _position) :
	m_iIndices(0),
	m_shaders(_shaders),
	pTexture(nullptr),
	m_model("Models/Floor/floor.obj", _shaders)
{
	m_eModelType = FLOOR;
	m_position = _position;

	getUniformLocation();
}

CFloor::~CFloor()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}
}

void CFloor::DrawObject()
{
	glUseProgram(m_shaders);
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);
	glUniformMatrix4fv(gScaleLocation, 1, GL_FALSE, glm::value_ptr(Scale));
	glUniformMatrix4fv(gRotateLocation, 1, GL_FALSE, glm::value_ptr(Rotate));
	glUniformMatrix4fv(gTranslateLocation, 1, GL_FALSE, glm::value_ptr(Translate));

	m_model.Draw();
}

CFloor * CFloor::CreateFloor(GLuint _shaders, glm::vec3 _position)
{
	CFloor* floor = new CFloor(_shaders, _position);
	return floor;
}

void CFloor::getUniformLocation()
{
	gScaleLocation = glGetUniformLocation(m_shaders, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);

	gRotateLocation = glGetUniformLocation(m_shaders, "gRotate");
	assert(gRotateLocation != 0xFFFFFFFF);

	gTranslateLocation = glGetUniformLocation(m_shaders, "gTranslate");
	assert(gTranslateLocation != 0xFFFFFFFF);
}