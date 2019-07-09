#include "Particle.h"
#include "ShaderHelper.h"
#include "Camera.h"



CParticleSystem::CParticleSystem(GLuint _shaders, glm::vec3 _position) :
	m_iIndices(0),
	m_shaders(_shaders)
	//pTexture(nullptr),
{
	ShaderHelper::compileAndLinkShaders("Shaders/PAR_compute_shader.glsl", computeShader);
	InitializeVectors();
	InitializeVBOs();

	m_eModelType = PARTICLE_SYSTEM;
	m_position = _position;
	getUniformLocation();
}

CParticleSystem::~CParticleSystem()
{
	//if (pTexture != nullptr)
	//{
	//	delete pTexture;
	//	pTexture = 0;
	//}
}

void CParticleSystem::DrawObject()
{
	// Compute part
	glUseProgram(computeShader);
	currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
	currentTime = currentTime / 1000.0f;
	glUniform1f(currentTimeLocation, currentTime);

	glDispatchCompute(NUM_PARTICLES / 128, 1, 1);
	// Wait for completion
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Render part
	glUseProgram(m_shaders);

	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 Model = Translate * Rotate * Scale;
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Model));

	glm::mat4 vp = CCamera::getProjectionMatrix() * CCamera::getViewMatrix();
	glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(vp));

	// Bind position buffer
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	//glBindVertexArray(particleVao);

	// Render
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	// Unbind
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	//glBindVertexArray(0);
}

CParticleSystem * CParticleSystem::CreateParticleSystem(GLuint _shaders, glm::vec3 _position)
{
	CParticleSystem* particleSystem = new CParticleSystem(_shaders, _position);
	return particleSystem;
}

void CParticleSystem::getUniformLocation()
{
	vpLocation = glGetUniformLocation(m_shaders, "vp");

	modelLocation = glGetUniformLocation(m_shaders, "model");

	currentTimeLocation = glGetUniformLocation(computeShader, "currentTime");
	//assert(currentTimeLocation != 0xFFFFFFFF);
}

void CParticleSystem::InitializeVectors()
{
	initialPosition.resize(NUM_PARTICLES);
	initialVelocity.resize(NUM_PARTICLES);
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		float particleLife = 300.0f;

		initialPosition.at(i) = glm::vec4(
			((randomFloat() * 2.0f) - 1.0f) * 50.0f,
			((randomFloat() * 2.0f) - 1.0f) * 15.0f,
			((randomFloat() * 2.0f) - 1.0f) * 50.0f,
			particleLife);

		initialVelocity.at(i) = glm::vec4(
			0.0f,
			-0.05 + -randomFloat() * 0.2f,
			0.0f,
			particleLife);
	}
}

void CParticleSystem::InitializeVBOs()
{
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialPosition.size() * sizeof(glm::vec4), &initialPosition[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posVbo);

	glGenBuffers(1, &initPosVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initPosVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialPosition.size() * sizeof(glm::vec4), &initialPosition[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, initPosVbo);

	glGenBuffers(1, &velVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialVelocity.size() * sizeof(glm::vec4), &initialVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, velVbo);

	glGenBuffers(1, &initVelVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initVelVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialVelocity.size() * sizeof(glm::vec4), &initialVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, initVelVbo);

	// Unused VAO
	glGenVertexArrays(1, &particleVao);
	glBindVertexArray(particleVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
