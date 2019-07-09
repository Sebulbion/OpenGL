//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Shader.h
// Description	: this is a wrapper that links each shader with their view type
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glew.h>
#include <freeglut.h>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ViewType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

class CShader
{
public:
	// The constructor
	// Takes the shaders and their view type
	CShader(GLuint _shader, ViewType _eViewType);
	~CShader();

	// Sends data to the shaders based on their view type
	void SendDataToShaders(glm::mat4 _view, glm::mat4 _ortho, glm::mat4 _perspective);

private:
	GLuint m_shader, m_viewLocation, m_orthoLocation, m_perspectiveLocation;
	ViewType m_eViewType;
};

#endif //__SHADER_H__