#include "Sphere.h"
#include "Terrain.h"
#include"Camera.h"
#include "ShadowMap.h"



CSphere::CSphere(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position) :
	m_iIndices(0),
	m_shaders(_shaders),
	m_shadowShaders(_shadowShaders),
	//pTexture(nullptr),
	m_model("Models/Sphere/Sphere.obj", _shaders)
{
	m_eModelType = SPHERE;
	m_position = _position;

	getUniformLocation();
}

CSphere::~CSphere()
{
	//if (pTexture != nullptr)
	//{
	//	delete pTexture;
	//	pTexture = 0;
	//}
}

void CSphere::DrawObject()
{
	glUseProgram(m_shaders);
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 Model = Translate * Rotate * Scale;

	glm::mat4 vp = CCamera::getProjectionMatrix() * CCamera::getViewMatrix();
	glm::mat4 lightVp = CCamera::getProjectionMatrix() * CShadowMap::lightViewMatrix;

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Model));
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));
	glUniformMatrix4fv(lightVpLocation, 1, GL_FALSE, glm::value_ptr(lightVp));
	glUniform3f(lightPosLocation, CShadowMap::lightPosition.x, CShadowMap::lightPosition.y, CShadowMap::lightPosition.z);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, CShadowMap::depthMapTexture);
	glUniform1i(glGetUniformLocation(m_shaders, "shadowMap"), 1);

	m_model.program = m_shaders;
	m_model.Draw();
}

void CSphere::ShadowPass()
{
	CalculateTerrainPos();
	glUseProgram(m_shadowShaders);
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 Model = Translate * Rotate * Scale;

	glm::mat4 lightVp = CCamera::getProjectionMatrix() * CShadowMap::lightViewMatrix;

	glUniformMatrix4fv(glGetUniformLocation(m_shadowShaders, "model"), 1, GL_FALSE, glm::value_ptr(Model));
	glUniformMatrix4fv(glGetUniformLocation(m_shadowShaders, "vp"), 1, GL_FALSE, glm::value_ptr(lightVp));

	m_model.program = m_shadowShaders;
	m_model.Draw();
}

CSphere * CSphere::CreateSphere(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position)
{
	CSphere* sphere = new CSphere(_shaders, _shadowShaders, _position);
	return sphere;
}

void CSphere::Move(float _fDeltaX, float _fDeltaZ)
{
	m_position.x += _fDeltaX;
	m_position.z += _fDeltaZ;
}

void CSphere::getUniformLocation()
{
	modelLocation = glGetUniformLocation(m_shaders, "model");
	//assert(gScaleLocation != 0xFFFFFFFF);

	vpLocation = glGetUniformLocation(m_shaders, "vp");
	//assert(vpLocation != 0xFFFFFFFF);

	lightPosLocation = glGetUniformLocation(m_shaders, "lightPos");
	//assert(vpLocation != 0xFFFFFFFF);

	lightVpLocation = glGetUniformLocation(m_shaders, "lightVp");
}

void CSphere::CalculateTerrainPos()
{
	float fHalfSize = CTerrain::m_fSize / 2.0f;
	float fXPercentage = (m_position.x + fHalfSize) / CTerrain::m_fSize;
	float fZPercentage = (m_position.z + fHalfSize) / CTerrain::m_fSize;

	if (fXPercentage <= 1.0f && fXPercentage >= 0.0f && fZPercentage <= 1.0f && fZPercentage >= 0.0f)
	{
		int iPerlinXvalue = static_cast<int>(fXPercentage * static_cast<float>(PERLIN_WIDTH - 1));
		int iPerlinYvalue = static_cast<int>(fZPercentage * static_cast<float>(PERLIN_HEIGHT - 1));

		m_position.y = Lerp(m_position.y,(static_cast<float>(CTerrain::tenImage[iPerlinXvalue][iPerlinYvalue][0]) * CTerrain::m_fMaxHeight) + 2.0f, 0.5f);
		//m_position.y += 2.0f;
	}
}
