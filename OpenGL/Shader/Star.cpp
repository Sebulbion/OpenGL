#include "Star.h"


CStar::CStar(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath) :
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

CStar::~CStar()
{
	if (pTexture != nullptr)
	{
		delete pTexture;
		pTexture = 0;
	}
}

void CStar::DrawObject()
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

CStar * CStar::CreateStar(GLuint _shaders, glm::vec3 _position)
{
	std::vector<VertexFormat> _vecVertices;
	std::vector<GLuint> vecIndices;
	std::string strImagePath;
	glm::vec3 rotation;

	_vecVertices = CreateVertices();
	vecIndices = CreateIndices();
	strImagePath = "Textures/blueRed.jpg";
	rotation = glm::vec3(0.0f, 1.0f, 0.0f);

	CStar* star = new CStar(_vecVertices, vecIndices, _shaders, _position, rotation, strImagePath);
	return star;
}

void CStar::IncreaseRotationSpeed(float _fValue)
{
	m_fRotationSpeed += _fValue;
}

void CStar::getUniformLocation()
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

void CStar::CreateAndBindTexture()
{
	//aTextureLocation = GL_TEXTURE_2D;
	//pTexture = new Texture(aTextureLocation, m_strImagePath);
	//if (!pTexture->load())
	//{
	//	//std::cout << "Error Loading Texture";
	//}
	//glUniform1i(glGetUniformLocation(m_shaders, "Texture"), 0);
}

void CStar::CreateVBOAttributes(std::vector<VertexFormat> _vecVertices)
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

	//aTextureLocation = glGetAttribLocation(shaders, "a_texCord");
	//glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6));
	//glEnableVertexAttribArray(aTextureLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CStar::CreateEBO(std::vector<GLuint> _vecIndices)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _vecIndices.size(), _vecIndices.data(), GL_STATIC_DRAW);
}

std::vector<VertexFormat> CStar::CreateVertices()
{
	VertexFormat p1{ { 0.0f,  0.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
	VertexFormat p2{ { -10.0f,  5.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
	VertexFormat p3{ { 3.0f,  7.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
	VertexFormat p4{ { 10.0f,  5.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
	VertexFormat p5{ { -7.0f,  -9.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };
	VertexFormat p6{ { -3.0f,  3.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } };


	std::vector<VertexFormat> vecVertices{
		p1,
		p2,
		p3,
		p4, 
		p5,
		p6 };

	return vecVertices;
}

std::vector<GLuint> CStar::CreateIndices()
{
	std::vector<GLuint> vecIndices{
		0, 1, 2, 3, 4, 5, 6 };
	return vecIndices;
}

