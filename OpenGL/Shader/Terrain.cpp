#include "Terrain.h"
#include "Camera.h"
#include "Perlin.h"
#include "ShaderHelper.h"
#include "ShadowMap.h"

int CTerrain::m_iNumOfCols = 10;
int CTerrain::m_iNumOfRows = 10;
float CTerrain::m_fSize = 100.0f;
float CTerrain::m_fMaxHeight = 35.0f;
Tensor<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3> CTerrain::tenImage = { {{}} };

CTerrain::CTerrain(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath) :
	m_iIndices(0),
	m_shaders(_shaders),
	m_shadowShader(_shadowShaders),
	m_strImagePath(_strImagePath),
	pTexture(nullptr),
	pGrassTexture(nullptr),
	m_iLevelOfDetail(32)
{
	//glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

	m_eModelType = TERRAIN;
	m_position = _position;
	m_rotation = _rotation;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	CreateVBOAttributes(_vecVertices);
	CreateEBO(_vecIndices);
	m_iIndices = _vecIndices.size();

	CreateAndBindTexture();

	//getUniformLocation(m_shaders);
}

CTerrain::~CTerrain()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}

	if (pGrassTexture != nullptr)
	{
		delete pGrassTexture;
		pGrassTexture = 0;
	}
}

void CTerrain::DrawObject()
{
	glUseProgram(m_shaders);
	getUniformLocation(m_shaders);

	float distanceToCamera;
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), m_rotation);
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);

	glm::mat4 mvp = CCamera::getProjectionMatrix() * CCamera::getViewMatrix() * Translate * Rotate * Scale;

	currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	currentTime = currentTime / 1000.0f;

	distanceToCamera = glm::abs(glm::length(CCamera::GetPosition() - m_position));

	glUniformMatrix4fv(gMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4f(gCameraPos, CCamera::GetPosition().x, CCamera::GetPosition().y, CCamera::GetPosition().z, 1.0f);
	glUniform1f(gCurrentTimeLocation, currentTime);
	glUniform1f(gMaxHeightLocation, m_fMaxHeight);
	glUniform1i(gLevelOfDetailLocation, m_iLevelOfDetail);

	pTexture->bind(GL_TEXTURE0);
	glUniform1i(gSampler, 0);
	pGrassTexture->bind(GL_TEXTURE1);
	glUniform1i(gGrassSampler, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, CShadowMap::depthMapTexture);
	glUniform1i(gShadowMapSampler, 2);

	glm::mat4 lightMvp = CCamera::getProjectionMatrix() * CShadowMap::lightViewMatrix * Translate * Rotate * Scale;
	glUniformMatrix4fv(gLightMVPLocation, 1, GL_FALSE, glm::value_ptr(lightMvp));

	glBindVertexArray(VAO);
	glDrawElements(GL_PATCHES, m_iIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CTerrain::ShadowPass()
{
	glUseProgram(m_shadowShader);
	getUniformLocation(m_shadowShader);

	float distanceToCamera;
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), m_rotation);
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);

	glm::mat4 mvp = CCamera::getProjectionMatrix() * CShadowMap::lightViewMatrix * Translate * Rotate * Scale;

	currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	currentTime = currentTime / 1000.0f;

	distanceToCamera = glm::abs(glm::length(CCamera::GetPosition() - m_position));

	glUniformMatrix4fv(gMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform4f(gCameraPos, CCamera::GetPosition().x, CCamera::GetPosition().y, CCamera::GetPosition().z, 1.0f);
	glUniform1f(gCurrentTimeLocation, currentTime);
	glUniform1f(gMaxHeightLocation, m_fMaxHeight);
	glUniform1i(gLevelOfDetailLocation, m_iLevelOfDetail);
	glUniformMatrix4fv(gLightMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));

	pTexture->bind(GL_TEXTURE0);
	glUniform1i(gSampler, 0);

	pGrassTexture->bind(GL_TEXTURE1);
	glUniform1i(gGrassSampler, 1);

	glBindVertexArray(VAO);
	glDrawElements(GL_PATCHES, m_iIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

CTerrain * CTerrain::CreateTerrain(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position)
{
	std::vector<VertexFormat> _vecVertices;
	std::vector<GLuint> vecIndices;
	std::string strImagePath;
	glm::vec3 rotation;

	CPerlin::PerlinNoise(tenImage, 4, 10.0f, 0.5f, 0, 0);
	_vecVertices = CreateVertices();
	vecIndices = CreateIndices();
	strImagePath = "Textures/Grass.png";
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);

	CTerrain* terrain = new CTerrain(_vecVertices, vecIndices, _shaders, _shadowShaders, _position, rotation, strImagePath);
	return terrain;
}

void CTerrain::getUniformLocation(GLuint shader)
{
	gMVPLocation = glGetUniformLocation(shader, "mvp");
	//assert(gMVPLocation != 0xFFFFFFFF);

	gLightMVPLocation = glGetUniformLocation(shader, "lightMvp");
	//assert(gLightMVPLocation != 0xFFFFFFFF);

	gCameraPos = glGetUniformLocation(shader, "cameraPos");
	//assert(gCameraPos != 0xFFFFFFFF);

	gCurrentTimeLocation = glGetUniformLocation(shader, "currentTime");
	//assert(gCurrentTimeLocation != 0xFFFFFFFF);

	//gScaleLocation = glGetUniformLocation(m_shaders, "gScale");
	//assert(gScaleLocation != 0xFFFFFFFF);
	//
	//gRotateLocation = glGetUniformLocation(m_shaders, "gRotate");
	//assert(gRotateLocation != 0xFFFFFFFF);
	//
	//gTranslateLocation = glGetUniformLocation(m_shaders, "gTranslate");
	//assert(gTranslateLocation != 0xFFFFFFFF);

	gMaxHeightLocation = glGetUniformLocation(shader, "gMaxHeight");
	//assert(gMaxHeightLocation != 0xFFFFFFFF);

	gSampler = glGetUniformLocation(shader, "gSampler");
	//assert(gSampler != 0xFFFFFFFF);

	gLevelOfDetailLocation = glGetUniformLocation(shader, "levelOfDetail");
	//assert(gLevelOfDetailLocation != 0xFFFFFFFF);

	gGrassSampler = glGetUniformLocation(shader, "gGrassSampler");
	//assert(gGrassSampler != 0xFFFFFFFF);

	gShadowMapSampler = glGetUniformLocation(shader, "shadowMap");
	//assert(gShadowMapSampler != 0xFFFFFFFF);

	//gCameraFrontLocation = glGetUniformLocation(m_shaders, "gCameraFront");
	//assert(gCameraFrontLocation != 0xFFFFFFFF);
}

void CTerrain::CreateAndBindTexture()
{
	aTextureLocation = GL_TEXTURE_2D;
	pTexture = new Texture(aTextureLocation, m_strImagePath);
	if (!pTexture->load(&tenImage[0][0][0], PERLIN_WIDTH, PERLIN_HEIGHT))
	{
		//std::cout << "Error Loading Texture";
	}

	aTextureLocation = GL_TEXTURE_2D;
	pGrassTexture = new Texture(aTextureLocation, m_strImagePath);
	if (!pGrassTexture->load())
	{
		//std::cout << "Error Loading Texture";
	}
	//glUniform1i(glGetUniformLocation(m_shaders, "Texture"), 0);
}

void CTerrain::CreateVBOAttributes(std::vector<VertexFormat> _vecVertices)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * _vecVertices.size(), _vecVertices.data(), GL_STATIC_DRAW);

	aPositionLocation = glGetAttribLocation(m_shaders, "a_position");
	glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(aPositionLocation);

	aColorLocation = glGetAttribLocation(m_shaders, "a_Color");
	glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3));
	glEnableVertexAttribArray(aColorLocation);

	aTextureLocation = glGetAttribLocation(m_shaders, "a_texCord");
	glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6));
	glEnableVertexAttribArray(aTextureLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CTerrain::CreateEBO(std::vector<GLuint> _vecIndices)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _vecIndices.size(), _vecIndices.data(), GL_STATIC_DRAW);
}

std::vector<VertexFormat> CTerrain::CreateVertices()
{
	std::vector<VertexFormat> vecVertices{};
	float fHalfSize = m_fSize / 2;

	for (int i = 0; i < m_iNumOfRows; i++)
	{
		float fZPercentage = static_cast<float>(i) / (m_iNumOfRows - 1);
		for (int j = 0; j < m_iNumOfCols; j++)
		{
			float fXPercentage = static_cast<float>(j) / (m_iNumOfCols - 1);
			//int iPerlinXvalue = static_cast<int>(fXPercentage * static_cast<float>(PERLIN_WIDTH - 1));
			//int iPerlinYvalue = static_cast<int>(fZPercentage * static_cast<float>(PERLIN_HEIGHT - 1));

			//vecVertices.push_back({ {
			//		(fXPercentage  * m_fSize) - fHalfSize,
			//		static_cast<float>(tenImage[iPerlinXvalue][iPerlinYvalue][0]) * m_fMaxHeight,
			//		(fZPercentage  * m_fSize) - fHalfSize },													// Position
			//		{1.0f,1.0f,1.0f},																			// Color
			//		{ fXPercentage, fZPercentage } });															// UV

			vecVertices.push_back({ {
					(fXPercentage  * m_fSize) - fHalfSize,
					0.0f,
					(fZPercentage  * m_fSize) - fHalfSize },													// Position
					{ 1.0f,1.0f,1.0f },																			// Color
					{ fXPercentage, fZPercentage } });															// UV
		}
	}

	return vecVertices;
}

std::vector<GLuint> CTerrain::CreateIndices()
{
	//std::vector<GLuint> vecIndices{
	//	0, 2, 1, 
	//	0, 2, 3};

	std::vector<GLuint> vecIndices{};

	for (int i = 0; i < m_iNumOfRows - 1; i++)
	{
		for (int j = 0; j < m_iNumOfCols - 1; j++)
		{
			int iColValue = j + (i * m_iNumOfRows);
			vecIndices.push_back(iColValue);
			vecIndices.push_back(iColValue + 1);
			vecIndices.push_back(j + ((i + 1) * m_iNumOfRows));

			vecIndices.push_back(iColValue + 1);
			vecIndices.push_back(j + ((i + 1) * m_iNumOfRows) + 1);
			vecIndices.push_back(j + ((i + 1) * m_iNumOfRows));
		}
	}

	return vecIndices;
}

void CTerrain::SetLevelOfDetail(int _bNewDetail)
{
	m_iLevelOfDetail = _bNewDetail;
}
