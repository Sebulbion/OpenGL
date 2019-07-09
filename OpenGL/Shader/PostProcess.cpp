#include <iostream>
#include "PostProcess.h"
#include "Camera.h"



CPostProcess::CPostProcess(GLuint _shaders):
	shaders(_shaders),
	m_iMode(0)
{
	glUseProgram(shaders);
	SetupFrameBuffer();
	CreateQuad();
	GetUniformLocations();

	// Values for first frame
	inverseViewProjection = glm::inverse(CCamera::getProjectionMatrix() * CCamera::getViewMatrix());
	previousViewProjection = CCamera::getProjectionMatrix() * CCamera::getViewMatrix();
}


CPostProcess::~CPostProcess()
{
}

void CPostProcess::SetupFrameBuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//======================Color Buffer=========================
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGB,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0, 
		GL_RGB,
		GL_UNSIGNED_BYTE,
		NULL); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		renderTexture,
		0);

	//======================Depth Buffer=========================
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT24,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_BYTE,
		NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		depthTexture,
		0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void CPostProcess::GetUniformLocations()
{
	gSampler = glGetUniformLocation(shaders, "gSampler");
	assert(gSampler != 0xFFFFFFFF);

	gDepthSampler = glGetUniformLocation(shaders, "gDepthSampler");
	//assert(gDepthSampler != 0xFFFFFFFF);

	aModeLocation = glGetUniformLocation(shaders, "gMode");
	assert(aModeLocation != 0xFFFFFFFF);

	inverseViewProjectionLocation = glGetUniformLocation(shaders, "gInverseViewProjection");
	//assert(inverseViewProjectionLocation != 0xFFFFFFFF);

	previousViewProjectionLocation = glGetUniformLocation(shaders, "gPreviousViewProjection");
	//assert(previousViewProjectionLocation != 0xFFFFFFFF);
}

void CPostProcess::CreateQuad()
{
	VertexFormat verFrontTopLeft{ { -1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } };
	VertexFormat verFrontTopRight{ { 1.0f,  1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } };
	VertexFormat verFrontBottomLeft{ { -1.0f,  -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } };
	VertexFormat verFrontBottomRight{ { 1.0f, -1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } };
	
	
	std::vector<VertexFormat> vecVertices{
		verFrontTopRight,
		verFrontTopLeft,
		verFrontBottomLeft,
		verFrontBottomRight,};

	std::vector<GLuint> vecIndices{
		0, 2, 1, 
		0, 2, 3};

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vecVertices.size(), vecVertices.data(), GL_STATIC_DRAW);

	aPositionLocation = glGetAttribLocation(shaders, "a_position");
	glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(aPositionLocation);

	//aColorLocation = glGetAttribLocation(m_shaders, "a_Color");
	//glVertexAttribPointer(aColorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3));
	//glEnableVertexAttribArray(aColorLocation);

	aTextureLocation = glGetAttribLocation(shaders, "a_texCord");
	glVertexAttribPointer(aTextureLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6));
	glEnableVertexAttribArray(aTextureLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vecIndices.size(), vecIndices.data(), GL_STATIC_DRAW);
}

void CPostProcess::PreRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void CPostProcess::PostRender()
{
	inverseViewProjection = glm::inverse(CCamera::getProjectionMatrix() * CCamera::getViewMatrix());

	glUseProgram(shaders);
	glUniformMatrix4fv(inverseViewProjectionLocation, 1, GL_FALSE, &inverseViewProjection[0][0]);
	glUniformMatrix4fv(previousViewProjectionLocation, 1, GL_FALSE, &previousViewProjection[0][0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(gSampler, 0);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(gDepthSampler, 1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glUniform1i(aModeLocation, m_iMode);

	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);

	previousViewProjection = CCamera::getProjectionMatrix() * CCamera::getViewMatrix();
}

void CPostProcess::SwapMode()
{
	m_iMode = (m_iMode + 1) % 5;
}
