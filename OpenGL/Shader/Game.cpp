//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Game.cpp
// Description	: this is the singelton main game scene
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Game.h"
#include "Sphere.h"
#include "Player.h"
#include "Terrain.h"

CGame* CGame::m_gamInstance = 0;

CGame::CGame():
	yaw(0.0f),
	pitch(-20.0f),
	m_bEPressed(false)
{
}


CGame::~CGame()
{
	while (m_vecObjects.size() != 0)
	{
		CObject* tempModel = m_vecObjects.back();
		m_vecObjects.pop_back();
		delete tempModel;
		tempModel = 0;
	}
	if (postProcess != nullptr)
	{
		delete postProcess;
		postProcess = 0;
	}

	if (shadowMap != nullptr)
	{
		delete shadowMap;
		shadowMap = 0;
	}
}

CGame * CGame::GetInstance()
{
	if (m_gamInstance == nullptr)
	{
		m_gamInstance = new CGame();
	}
	return m_gamInstance;
}

void CGame::DestroyInstance()
{
	if (m_gamInstance != nullptr)
	{
		delete m_gamInstance;
	}
}

void CGame::RenderScene()
{
	mainCamera.SendDataToShaders();

	shadowMap->StartShadowMap();
	static_cast<CTerrain*>(m_vecObjects.at(1))->ShadowPass();
	static_cast<CPlayer*>(m_vecObjects.at(2))->ShadowPass();
	static_cast<CSphere*>(m_vecObjects.at(4))->ShadowPass();
	shadowMap->EndShadowMap();

	postProcess->PreRender();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	for (size_t i = 0; i < m_vecObjects.size(); ++i)
	{
		m_vecObjects.at(i)->DrawObject();
	}
	postProcess->PostRender();
	glutSwapBuffers();
}

void CGame::Update(unsigned char* _keyState, unsigned char* _mouseState)
{
	if (_keyState[27] == BUTTON_DOWN)
	{
		glutLeaveMainLoop();
		DestroyInstance();
	}
	else
	{
		MoveCamera(_keyState);
		ObjectOperations(_keyState);

		static_cast<CPlayer*>(m_vecObjects.at(2))->Update();
	}
}

void CGame::Initialize()
{
	std::srand(GLUT_ELAPSED_TIME);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glutSetCursor(GLUT_CURSOR_NONE);
	//glutFullScreen();
	GLuint cubeShaders;
	ShaderHelper::compileAndLinkShaders("Shaders/cube_vertex_shader.glsl", "Shaders/cube_fragment_shader.glsl", cubeShaders);
	GLuint perspectiveShaders;
	ShaderHelper::compileAndLinkShaders("Shaders/model_vertex_shader.glsl", "Shaders/model_fragment_shader.glsl", perspectiveShaders);
	GLuint basicShader;
	ShaderHelper::compileAndLinkShaders("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl", basicShader);
	GLuint perlinShader;
	ShaderHelper::compileAndLinkShaders("Shaders/perlin_vertex_shader.glsl", "Shaders/perlin_fragment_shader.glsl", perlinShader);
	GLuint geometryShader;
	ShaderHelper::compileAndLinkShaders("Shaders/GO_vertex_shader.glsl", "Shaders/GO_fragment_shader.glsl","Shaders/GO_geometry_shader.glsl", geometryShader);
	GLuint tessellationShader;
	ShaderHelper::compileAndLinkShaders("Shaders/TES_vertex_shader.glsl", "Shaders/TES_fragment_shader.glsl", "Shaders/TES_geometry_shader.glsl",
										"Shaders/TES_tessellation_Control_shader.glsl", "Shaders/TES_tessellation_evaluation_shader.glsl", tessellationShader);
	ShaderHelper::compileAndLinkShaders("Shaders/PP_vertex_shader.glsl", "Shaders/PP_fragment_shader.glsl", postProcessingShader);

	GLuint terrainShadowShader;
	ShaderHelper::compileAndLinkShaders("Shaders/TES_vertex_shader.glsl", "Shaders/SH_terrain_fragment_shader.glsl", "Shaders/TES_geometry_shader.glsl",
		"Shaders/TES_tessellation_Control_shader.glsl", "Shaders/TES_tessellation_evaluation_shader.glsl", terrainShadowShader);

	GLuint particleShader;
	ShaderHelper::compileAndLinkShaders("Shaders/PAR_vertex_shader.glsl", "Shaders/PAR_fragment_shader.glsl", particleShader);

	GLuint animationShader;
	ShaderHelper::compileAndLinkShaders("Shaders/ANI_vertex_shader.glsl", "Shaders/ANI_fragment_shader.glsl", animationShader);
	GLuint animationShadowShader;
	ShaderHelper::compileAndLinkShaders("Shaders/SH_ANI_vertex_shader.glsl", "Shaders/SH_fragment_shader.glsl", animationShadowShader);
	GLuint sphereShadowShader;
	ShaderHelper::compileAndLinkShaders("Shaders/SH_SPH_vertex_shader.glsl", "Shaders/SH_fragment_shader.glsl", sphereShadowShader);
	GLuint sphereShader;
	ShaderHelper::compileAndLinkShaders("Shaders/SPH_vertex_shader.glsl", "Shaders/SPH_fragment_shader.glsl", sphereShader);

	// Sends all the shaders to the camera for sending matrix data later
	mainCamera.GetUniformLocation(cubeShaders, false);
	m_vecShaders.push_back(cubeShaders);
	mainCamera.GetUniformLocation(perspectiveShaders, false);
	m_vecShaders.push_back(perspectiveShaders);
	mainCamera.GetUniformLocation(basicShader, false);
	m_vecShaders.push_back(basicShader);
	mainCamera.GetUniformLocation(perlinShader, false);
	m_vecShaders.push_back(perlinShader);
	mainCamera.GetUniformLocation(geometryShader, false);
	m_vecShaders.push_back(geometryShader);
	//mainCamera.GetUniformLocation(tessellationShader, false);
	m_vecShaders.push_back(tessellationShader);
	m_vecShaders.push_back(particleShader);
	m_vecShaders.push_back(animationShader);
	m_vecShaders.push_back(sphereShader);

	m_vecShadowShaders.push_back(terrainShadowShader);
	m_vecShadowShaders.push_back(animationShadowShader);
	m_vecShadowShaders.push_back(sphereShadowShader);

	CreateModels();

	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	m_iLastX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	m_iLastY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	shadowMap = new CShadowMap();
	postProcess = new CPostProcess(postProcessingShader);
}

void CGame::RotateCamera(int _iX, int _iY)
{
	float xoffset = static_cast<float>(_iX - m_iLastX);
	float yoffset = static_cast<float>(_iY - m_iLastY);
	
	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw -= xoffset;
	pitch -= yoffset;
	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = -cos(glm::radians(pitch))*sin(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	mainCamera.SetCameraFront(glm::normalize(front));
	
	// Sets moves the mouse to the center of the screen
	if (_iX >(glutGet(GLUT_WINDOW_WIDTH) / 2) + 50 || _iX<(glutGet(GLUT_WINDOW_WIDTH) / 2) - 50 || _iY >(glutGet(GLUT_WINDOW_HEIGHT) / 2) + 50 || _iY < (glutGet(GLUT_WINDOW_HEIGHT) / 2) - 50)
	{
		_iX = glutGet(GLUT_WINDOW_WIDTH) / 2;
		_iY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	}
	m_iLastX = _iX;
	m_iLastY = _iY;
}

void CGame::CreateModels()
{
	// Skybox should render first
	m_vecObjects.push_back(CObject::CreateObject(SKYBOX, m_vecShaders.at(0), 0, mainCamera.GetPosition()));
	m_vecObjects.push_back(CObject::CreateObject(TERRAIN, m_vecShaders.at(5), m_vecShadowShaders.at(0), glm::vec3(0.0f, 0.0f, 0.0f)));
	m_vecObjects.push_back(CObject::CreateObject(PLAYER, m_vecShaders.at(7), m_vecShadowShaders.at(1), glm::vec3(0.0f, 0.0f, 0.0f)));
	//m_vecObjects.push_back(CObject::CreateObject(FLOOR, m_vecShaders.at(1), 0, glm::vec3(0.0f, 0.0f, 0.0f)));
	m_vecObjects.push_back(CObject::CreateObject(PARTICLE_SYSTEM, m_vecShaders.at(6), 0, glm::vec3(0.0f, 90.0f, 0.0f)));
	m_vecObjects.push_back(CObject::CreateObject(SPHERE, m_vecShaders.at(8), m_vecShadowShaders.at(2), glm::vec3(-4.0f, 10.0f, 0.0f)));
	//m_vecObjects.push_back(CObject::CreateObject(CUBOID, m_vecShaders.at(4), glm::vec3(10.0f, 6.0f, -5.0f)));
}

void CGame::MoveCamera(unsigned char * _keyState)
{
	if (_keyState[(unsigned char)'w'] == BUTTON_DOWN || _keyState[(unsigned char)'W'] == BUTTON_DOWN)
	{
		mainCamera.MoveForward(0.5f);
	}
	if (_keyState[(unsigned char)'s'] == BUTTON_DOWN || _keyState[(unsigned char)'S'] == BUTTON_DOWN)
	{
		mainCamera.MoveForward(-0.5f);
	}
	if (_keyState[(unsigned char)'a'] == BUTTON_DOWN || _keyState[(unsigned char)'A'] == BUTTON_DOWN)
	{
		mainCamera.MoveLeft(0.5f);
	}
	if (_keyState[(unsigned char)'d'] == BUTTON_DOWN || _keyState[(unsigned char)'D'] == BUTTON_DOWN)
	{
		mainCamera.MoveLeft(-0.5f);
	}
}

void CGame::ObjectOperations(unsigned char * _keyState)
{
	glm::vec2 cameraFront = { mainCamera.GetFront().x, mainCamera.GetFront().z };
	cameraFront = glm::normalize(cameraFront);
	cameraFront = cameraFront / 4.0f;
	glm::vec2 movementValue = { 0.0f,0.0f};

	if (static_cast<CPlayer*>(m_vecObjects.at(2))->GetIsJumping() == false) {
		if (_keyState[(unsigned char)'u'] == BUTTON_DOWN || _keyState[(unsigned char)'U'] == BUTTON_DOWN)
		{
			movementValue = { 0.0f, 0.1f };
			static_cast<CPlayer*>(m_vecObjects.at(2))->Move(0.0f, 0.1f);
			ssAnimatedModel& animatedModel = static_cast<CPlayer*>(m_vecObjects.at(2))->m_model;
			if (animatedModel.bMoving == false) {
				animatedModel.bMoving = true;
				animatedModel.setCurrentAnimation(31, 50); // run animation 
			}
		}
		else if (_keyState[(unsigned char)'j'] == BUTTON_DOWN || _keyState[(unsigned char)'J'] == BUTTON_DOWN)
		{
			movementValue = { 0.0f, -0.1f };
			static_cast<CPlayer*>(m_vecObjects.at(2))->Move(0.0f, -0.1f);
			ssAnimatedModel& animatedModel = static_cast<CPlayer*>(m_vecObjects.at(2))->m_model;
			if (animatedModel.bMoving == false) {
				animatedModel.bMoving = true;
				animatedModel.setCurrentAnimation(51, 70); // run animation 
			}
		}
		else
		{
			ssAnimatedModel& animatedModel = static_cast<CPlayer*>(m_vecObjects.at(2))->m_model;
			if (animatedModel.bMoving == true) {
				animatedModel.bMoving = false;
				animatedModel.setCurrentAnimation(0, 30); //idle animation 
			}
		}
		if (_keyState[(unsigned char)'h'] == BUTTON_DOWN || _keyState[(unsigned char)'H'] == BUTTON_DOWN)
		{
			static_cast<CPlayer*>(m_vecObjects.at(2))->Rotate(1.0f);
		}
		if (_keyState[(unsigned char)'k'] == BUTTON_DOWN || _keyState[(unsigned char)'K'] == BUTTON_DOWN)
		{
			static_cast<CPlayer*>(m_vecObjects.at(2))->Rotate(-1.0f);
		}
		if (_keyState[(unsigned char)'v'] == BUTTON_DOWN || _keyState[(unsigned char)'v'] == BUTTON_DOWN)
		{
			static_cast<CPlayer*>(m_vecObjects.at(2))->Jump(movementValue);

			ssAnimatedModel& animatedModel = static_cast<CPlayer*>(m_vecObjects.at(2))->m_model;
			animatedModel.bMoving = true;
			animatedModel.setCurrentAnimation(71, 80); //jump animation 
		}
	}

	if (_keyState[(unsigned char)'0'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(1);
	}
	if (_keyState[(unsigned char)'1'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(2);
	}
	if (_keyState[(unsigned char)'2'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(4);
	}
	if (_keyState[(unsigned char)'3'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(8);
	}
	if (_keyState[(unsigned char)'4'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(16);
	}
	if (_keyState[(unsigned char)'5'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(32);
	}
	if (_keyState[(unsigned char)'6'] == BUTTON_DOWN)
	{
		static_cast<CTerrain*>(m_vecObjects.at(1))->SetLevelOfDetail(64);
	}
	if (_keyState[(unsigned char)'e'] == BUTTON_DOWN && m_bEPressed == false)
	{
		postProcess->SwapMode();
		m_bEPressed = true;
	}
	if (_keyState[(unsigned char)'e'] == BUTTON_UP)
	{
		m_bEPressed = false;
	}
}
