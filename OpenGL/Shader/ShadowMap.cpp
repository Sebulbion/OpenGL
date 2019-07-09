#include "ShadowMap.h"

GLuint CShadowMap::depthMapTexture = 0;
glm::mat4 CShadowMap::lightViewMatrix = {};
glm::vec3 CShadowMap::lightPosition = { -70.0f, 70.0f, 70.0f };

CShadowMap::CShadowMap()
{
	// depth texture 
	glGenTextures(1, &depthMapTexture); 
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_DEPTH_COMPONENT, 
		WINDOW_WIDTH * 3,
		WINDOW_HEIGHT * 3,
		0, 
		GL_DEPTH_COMPONENT, 
		GL_FLOAT, 
		NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		GL_DEPTH_ATTACHMENT, 
		GL_TEXTURE_2D, 
		depthMapTexture, 
		0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind buffer

	lightViewMatrix = glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


CShadowMap::~CShadowMap()
{
}

void CShadowMap::StartShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); 
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH * 3, WINDOW_HEIGHT * 3);
}

void CShadowMap::EndShadowMap()
{
	glFlush(); 
	glFinish();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
