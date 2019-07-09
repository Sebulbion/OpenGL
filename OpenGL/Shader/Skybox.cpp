//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Cube.cpp
// Description	: this is the cube object with all relevant functions
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Skybox.h"


CSkybox::CSkybox(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, glm::vec3 _position, std::vector<std::string> _strImagePaths):
	shaders(_shaders),
	m_iIndices(0),
	m_shaders(_shaders),
	m_strImagePaths(_strImagePaths),
	pTexture(nullptr)
{
	m_eModelType = SKYBOX;
	m_position = _position;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	CreateVBOAttributes(_vecVertices);
	CreateEBO(_vecIndices);
	m_iIndices = _vecIndices.size();

	CreateAndBindTexture();

	getUniformLocation();
}

CSkybox::~CSkybox()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}
}

CSkybox * CSkybox::CreateSkybox(GLuint _shaders, glm::vec3 _position)
{
	std::vector<VertexFormat> _vecVertices;
	std::vector<GLuint> vecIndices;
	std::vector<std::string> strImagePaths;

	_vecVertices = CreateVertices();
	vecIndices = CreateIndices();
	strImagePaths.push_back("Textures/skybox1/right.jpg");
	strImagePaths.push_back("Textures/skybox1/left.jpg");
	strImagePaths.push_back("Textures/skybox1/top.jpg");
	strImagePaths.push_back("Textures/skybox1/bottom.jpg");
	strImagePaths.push_back("Textures/skybox1/back.jpg");
	strImagePaths.push_back("Textures/skybox1/front.jpg");

	CSkybox* cube = new CSkybox(_vecVertices, vecIndices, _shaders, _position, strImagePaths);
	return cube;
}

void CSkybox::getUniformLocation()
{
	gScaleLocation = glGetUniformLocation(m_shaders, "gScale");
	assert(gScaleLocation != 0xFFFFFFFF);

	gRotateLocation = glGetUniformLocation(m_shaders, "gRotate");
	assert(gRotateLocation != 0xFFFFFFFF);

	gTranslateLocation = glGetUniformLocation(m_shaders, "gTranslate");
	assert(gTranslateLocation != 0xFFFFFFFF);

	gSampler = glGetUniformLocation(m_shaders, "cubeSampler");
	assert(gSampler != 0xFFFFFFFF);
}

void CSkybox::CreateAndBindTexture()
{
	aTextureLocation = GL_TEXTURE_CUBE_MAP;
	pTexture = new Texture(aTextureLocation, m_strImagePaths);
	if (!pTexture->loadCubemap())
	{
		//std::cout << "Error Loading Texture";
	}
	glUniform1i(glGetUniformLocation(m_shaders, "Texture"), 0);
}

void CSkybox::CreateVBOAttributes(std::vector<VertexFormat> _vecVertices)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * _vecVertices.size(), _vecVertices.data(), GL_STATIC_DRAW);

	aPositionLocation = glGetAttribLocation(shaders, "a_position");
	glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(aPositionLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CSkybox::CreateEBO(std::vector<GLuint> _vecIndices)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _vecIndices.size(), _vecIndices.data(), GL_STATIC_DRAW);
}

void CSkybox::DrawObject()
{
	//glDepthFunc(GL_GEQUAL);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);


	glUseProgram(m_shaders);

	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);

	glUniformMatrix4fv(gScaleLocation, 1, GL_FALSE, glm::value_ptr(Scale));
	glUniformMatrix4fv(gRotateLocation, 1, GL_FALSE, glm::value_ptr(Rotate));
	glUniformMatrix4fv(gTranslateLocation, 1, GL_FALSE, glm::value_ptr(Translate));

	glUniform1i(gSampler, 0);
	pTexture->bind(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_iIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDepthFunc(GL_LESS);

	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);

}

std::vector<VertexFormat> CSkybox::CreateVertices()
{
	VertexFormat verFrontTopLeft{ { -1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.0f } };
	VertexFormat verFrontTopRight{ { 1.0f,  1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verFrontBottomLeft{ { -1.0f, -1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verFrontBottomRight{ { 1.0f, -1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.5f } };

	VertexFormat verTopTopLeft{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 0.5f } };
	VertexFormat verTopTopRight{ { 1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verTopBottomLeft{ { -1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verTopBottomRight{ { 1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 1.0f } };

	VertexFormat verBackTopLeft{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.0f } };
	VertexFormat verBackTopRight{ { 1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	VertexFormat verBackBottomLeft{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verBackBottomRight{ { 1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.5f } };

	VertexFormat verBottomTopLeft{ { -1.0f,  -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 0.5f } };
	VertexFormat verBottomTopRight{ { 1.0f,  -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verBottomBottomLeft{ { -1.0f,  -1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verBottomBottomRight{ { 1.0f,  -1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.0f } };

	VertexFormat verLeftTopLeft{ { -1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verLeftTopRight{ { -1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.5f } };
	VertexFormat verLeftBottomLeft{ { -1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 1.0f } };
	VertexFormat verLeftBottomRight{ { -1.0f, -1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };

	VertexFormat verRightTopLeft{ { 1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 0.5f } };
	VertexFormat verRightTopRight{ { 1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 0.5f } };
	VertexFormat verRightBottomLeft{ { 1.0f, -1.0f, -1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.5f, 1.0f } };
	VertexFormat verRightBottomRight{ { 1.0f, -1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };


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

std::vector<GLuint> CSkybox::CreateIndices()
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
