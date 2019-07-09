//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Cuboid.cpp
// Description	: this is the cuboid object with all relevant functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Cuboid.h"


CCuboid::CCuboid(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath) :
	shaders(_shaders),
	m_iIndices(0),
	m_shaders(_shaders),
	m_strImagePath(_strImagePath),
	m_fRotationSpeed(1.0f),
	m_fRotationCounter(0.0f),
	pTexture(nullptr)
{
	m_eModelType = CUBOID;
	m_position = _position;
	m_rotation = _rotation;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	CreateVBOAttributes(_vecVertices);
	CreateEBO(_vecIndices);
	m_iIndices = _vecIndices.size();

	CreateAndBindTexture();

	getUniformLocation();
}

CCuboid::~CCuboid()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}
}

void CCuboid::DrawObject()
{
	glUseProgram(m_shaders);
	m_fRotationCounter += 0.5f * m_fRotationSpeed;

	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), m_rotation);
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);

	glUniformMatrix4fv(gScaleLocation, 1, GL_FALSE, glm::value_ptr(Scale));
	glUniformMatrix4fv(gRotateLocation, 1, GL_FALSE, glm::value_ptr(Rotate));
	glUniformMatrix4fv(gTranslateLocation, 1, GL_FALSE, glm::value_ptr(Translate));

	glUniform1i(gSampler, 0);
	//pTexture->bind(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_POINTS, m_iIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

CCuboid * CCuboid::CreateCuboid(GLuint _shaders, glm::vec3 _position)
{
	std::vector<VertexFormat> _vecVertices;
	std::vector<GLuint> vecIndices;
	std::string strImagePath;
	glm::vec3 rotation;

	_vecVertices = CreateVertices();
	vecIndices = CreateIndices();
	strImagePath = "Textures/blueRed.jpg";
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);

	CCuboid* cuboid = new CCuboid(_vecVertices, vecIndices, _shaders, _position, rotation, strImagePath);
	return cuboid;
}

void CCuboid::IncreaseRotationSpeed(float _fValue)
{
	m_fRotationSpeed += _fValue;
}

void CCuboid::getUniformLocation()
{
	gScaleLocation = glGetUniformLocation(m_shaders, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);

	gRotateLocation = glGetUniformLocation(m_shaders, "gRotate");
	assert(gRotateLocation != 0xFFFFFFFF);

	gTranslateLocation = glGetUniformLocation(m_shaders, "gTranslate");
	assert(gTranslateLocation != 0xFFFFFFFF);

	//gSampler = glGetUniformLocation(m_shaders, "gSampler");
	//assert(gSampler != 0xFFFFFFFF);
}

void CCuboid::CreateAndBindTexture()
{
	//aTextureLocation = GL_TEXTURE_2D;
	//pTexture = new Texture(aTextureLocation, m_strImagePath);
	//if (!pTexture->load())
	//{
	//	//std::cout << "Error Loading Texture";
	//}
	//glUniform1i(glGetUniformLocation(m_shaders, "Texture"), 0);
}

void CCuboid::CreateVBOAttributes(std::vector<VertexFormat> _vecVertices)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * _vecVertices.size(), _vecVertices.data(), GL_STATIC_DRAW);

	aPositionLocation = glGetAttribLocation(shaders, "a_position");
	glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(aPositionLocation);

	aColorLocation = glGetAttribLocation(shaders, "a_Color");
	glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(aColorLocation);

	aTextureLocation = glGetAttribLocation(shaders, "a_texCord");
	glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6));
	glEnableVertexAttribArray(aTextureLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CCuboid::CreateEBO(std::vector<GLuint> _vecIndices)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _vecIndices.size(), _vecIndices.data(), GL_STATIC_DRAW);
}

std::vector<VertexFormat> CCuboid::CreateVertices()
{
	VertexFormat verFrontTopLeft{ { -0.5f,  1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verFrontTopRight{ { 1.5f,  1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verFrontBottomLeft{ { -1.5,  -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verFrontBottomRight{ { 0.5f, -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verTopTopLeft{ { -0.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verTopTopRight{ { 1.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verTopBottomLeft{ { -0.5f,  1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verTopBottomRight{ { 1.5f,  1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verBackTopLeft{ { -0.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verBackTopRight{ { 1.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verBackBottomLeft{ { -1.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verBackBottomRight{ { 0.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verBottomTopLeft{ { -1.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verBottomTopRight{ { 0.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verBottomBottomLeft{ { -1.5f, -1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verBottomBottomRight{ { 0.5f, -1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verLeftTopLeft{ { -0.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verLeftTopRight{ { -0.5f,  1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verLeftBottomLeft{ { -1.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verLeftBottomRight{ { -1.5f, -1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verRightTopLeft{ { 1.5f,  1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verRightTopRight{ { 1.5f,  1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verRightBottomLeft{ { 0.5f, -1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verRightBottomRight{ { 0.5f, -1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };


	std::vector<VertexFormat> vecVertices{
		verFrontTopLeft,
		verFrontTopRight,
		verFrontBottomLeft,
		verFrontBottomRight,

		verTopTopLeft,
		verTopTopRight,
		verTopBottomLeft,
		verTopBottomRight,

		verBackTopLeft,
		verBackTopRight,
		verBackBottomLeft,
		verBackBottomRight,

		verBottomTopLeft,
		verBottomTopRight,
		verBottomBottomLeft,
		verBottomBottomRight,

		verLeftTopLeft,
		verLeftTopRight,
		verLeftBottomLeft,
		verLeftBottomRight,

		verRightTopLeft,
		verRightTopRight,
		verRightBottomLeft,
		verRightBottomRight };

	return vecVertices;
}

std::vector<GLuint> CCuboid::CreateIndices()
{
	std::vector<GLuint> vecIndices{
		0,2,1,
		1,2,3,

		4,6,5,
		5,6,7,

		8,10,9,
		9,10,11,

		12,14,13,
		13,14,15,

		16,18,17,
		17,18,19,

		20,22,21,
		21,22,23 };
	return vecIndices;
}
