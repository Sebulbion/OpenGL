//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Pyramid.h
// Description	: this is the pyramid object with all relevant functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "Object.h"
#include "Model.h"
class CFloor :
	public CObject
{
public:
	CFloor(GLuint _shaders, glm::vec3 _position);
	~CFloor();

	void DrawObject() override;

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CFloor* CreateFloor(GLuint _shaders, glm::vec3 _position);
private:
	// Gets the location of some variables for the shader for later edditiong
	void getUniformLocation();

	GLuint gScaleLocation, gRotateLocation, gTranslateLocation;
	GLuint m_shaders;
	Texture* pTexture;
	Model m_model;

	int m_iIndices;
};

#endif // __FLOOR_H__