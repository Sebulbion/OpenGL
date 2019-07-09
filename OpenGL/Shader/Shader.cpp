//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Shader.cpp
// Description	: this is a wrapper that links each shader with their view type
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Shader.h"


CShader::CShader(GLuint _shader, ViewType _eViewType):
	m_shader(_shader),
	m_eViewType(_eViewType)
{
	glUseProgram(_shader);
	m_viewLocation = glGetUniformLocation(_shader, "gView");
	assert(m_viewLocation != 0xFFFFFFFF);

	if (m_eViewType == ORTHOGRAPHIC)
	{
		m_orthoLocation = glGetUniformLocation(_shader, "gOrtho");
		assert(m_orthoLocation != 0xFFFFFFFF);
	}
	else if (m_eViewType == PERSPECTIVE)
	{
		m_perspectiveLocation = glGetUniformLocation(_shader, "gPerspective");
		assert(m_perspectiveLocation != 0xFFFFFFFF);
	}
}

CShader::~CShader()
{
}

void CShader::SendDataToShaders(glm::mat4 _view, glm::mat4 _ortho, glm::mat4 _perspective)
{
	glUseProgram(m_shader);
	glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, glm::value_ptr(_view));
	
	if (m_eViewType == ORTHOGRAPHIC)
	{
		glUniformMatrix4fv(m_orthoLocation, 1, GL_FALSE, glm::value_ptr(_ortho));
	}
	else if (m_eViewType == PERSPECTIVE)
	{
		glUniformMatrix4fv(m_perspectiveLocation, 1, GL_FALSE, glm::value_ptr(_perspective));
	}
}
