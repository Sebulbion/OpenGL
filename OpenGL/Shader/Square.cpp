//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Square.cpp
// Description	: this is the square object with all relevant functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Square.h"


CSquare::CSquare(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath) :
	shaders(_shaders),
	m_iIndices(0),
	m_shaders(_shaders),
	m_strImagePath(_strImagePath),
	m_fRotationSpeed(0.0f),
	m_fRotationCounter(0.0f),
	pTexture(nullptr)
{
	m_eModelType = SQUARE;
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

CSquare::~CSquare()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}
}

void CSquare::DrawObject()
{
	glUseProgram(m_shaders);
	m_fRotationCounter += 0.5f * m_fRotationSpeed;

	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_fRotationCounter), m_rotation);
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);
	currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	currentTime = currentTime / 1000.0f;

	glUniformMatrix4fv(gScaleLocation, 1, GL_FALSE, glm::value_ptr(Scale));
	glUniformMatrix4fv(gRotateLocation, 1, GL_FALSE, glm::value_ptr(Rotate));
	glUniformMatrix4fv(gTranslateLocation, 1, GL_FALSE, glm::value_ptr(Translate));
	glUniform1f(currentTimeLocation, currentTime);

	glUniform1i(gSampler, 0);
	pTexture->bind(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_iIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

CSquare * CSquare::CreateSquare(GLuint _shaders, glm::vec3 _position)
{
	std::vector<VertexFormat> _vecVertices;
	std::vector<GLuint> vecIndices;
	std::string strImagePath;
	glm::vec3 rotation;

	_vecVertices = CreateVertices();
	vecIndices = CreateIndices();
	strImagePath = "Textures/swirl.jpg";
	rotation = glm::vec3(0.0f, 0.0f, 1.0f);

	CSquare* square = new CSquare(_vecVertices, vecIndices, _shaders, _position, rotation, strImagePath);
	return square;
}

void CSquare::IncreaseRotationSpeed(float _fValue)
{
	m_fRotationSpeed += _fValue;
}

void CSquare::getUniformLocation()
{
	gScaleLocation = glGetUniformLocation(m_shaders, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);

	gRotateLocation = glGetUniformLocation(m_shaders, "gRotate");
	assert(gRotateLocation != 0xFFFFFFFF);

	gTranslateLocation = glGetUniformLocation(m_shaders, "gTranslate");
	assert(gTranslateLocation != 0xFFFFFFFF);

	gSampler = glGetUniformLocation(m_shaders, "gSampler");
	assert(gSampler != 0xFFFFFFFF);

	currentTimeLocation = glGetUniformLocation(m_shaders, "currentTime");
	assert(currentTimeLocation != 0xFFFFFFFF);
}

void CSquare::CreateAndBindTexture()
{
	aTextureLocation = GL_TEXTURE_2D;
	pTexture = new Texture(aTextureLocation, m_strImagePath);
	if (!pTexture->load())
	{
		//std::cout << "Error Loading Texture";
	}
	glUniform1i(glGetUniformLocation(m_shaders, "Texture"), 0);
}

void CSquare::CreateVBOAttributes(std::vector<VertexFormat> _vecVertices)
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

void CSquare::CreateEBO(std::vector<GLuint> _vecIndices)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _vecIndices.size(), _vecIndices.data(), GL_STATIC_DRAW);
}

std::vector<VertexFormat> CSquare::CreateVertices()
{
	VertexFormat verTopLeft{ { -1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verTopRight{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verBottomLeft{ { -1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verBottomRight{ { 1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	std::vector<VertexFormat> vecVertices{
		verTopLeft,
		verTopRight,
		verBottomLeft,
		verBottomRight };

	return vecVertices;
}

std::vector<GLuint> CSquare::CreateIndices()
{
	std::vector<GLuint> vecIndices{
		0,2,1,
		1,2,3 };
	return vecIndices;
}
