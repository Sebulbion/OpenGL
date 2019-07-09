#include "Player.h"
#include "Terrain.h"



CPlayer::CPlayer(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position) :
	m_iIndices(0),
	m_shaders(_shaders),
	m_shadowShaders(_shadowShaders),
	//pTexture(nullptr),
	m_model("Models/Player/theDude.DAE", "Models/Player/theDude.png", _shaders, _shadowShaders),
	m_bJumping(false),
	m_percentage(0.0f)
{
	m_eModelType = PLAYER;
	m_position = _position;

	m_model.setCurrentAnimation(0, 30);// set idle animation
	m_model.setScale(glm::vec3(0.0275f));
	m_model.setSpeed(50.0f);
}

CPlayer::~CPlayer()
{
	//if (pTexture != nullptr)
	//{
	//	delete pTexture;
	//	pTexture = 0;
	//}
}

void CPlayer::DrawObject()
{
	m_model.render(false);
}

void CPlayer::ShadowPass()
{
	CalculateTerrainPos();
	m_model.setPosition(m_position);
	m_model.setRotation(m_rotation);
	m_model.render(true);
}

CPlayer * CPlayer::CreatePlayer(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position)
{
	CPlayer* player = new CPlayer(_shaders, _shadowShaders, _position);
	return player;
}

void CPlayer::Move(float _fDeltaX, float _fDeltaZ)
{
	glm::vec2 movement = { _fDeltaX, _fDeltaZ};
	float theta = glm::radians(m_rotation.y);

	float cs = cos(theta);
	float sn = sin(theta);

	float px = movement.x * cs - movement.y * sn;
	float py = movement.x * sn + movement.y * cs;

	m_position.x += -px;
	m_position.z += py;
	
}

void CPlayer::Rotate(float _fDeltaRotation)
{
	m_rotation.y += _fDeltaRotation;
}

void CPlayer::Jump(glm::vec2 _lastMovement)
{
	m_bJumping = true;
	m_percentage = 0.0f;
	preJumpPosition = m_position;
	m_lastMovement = _lastMovement;
}

bool CPlayer::GetIsJumping()
{
	return m_bJumping;
}

void CPlayer::Update()
{
	// Jumps in a curve based on a percentage
	if (m_bJumping) {
		if (m_percentage < 1.0f) {
			m_percentage += 0.01f;
			m_position.y = preJumpPosition.y + glm::sin(m_percentage * glm::pi<float>()) * 10.0f;
			Move(m_lastMovement.x, m_lastMovement.y);

			// Slightly different from the CalculateTerrainPos function
			float fHalfSize = CTerrain::m_fSize / 2.0f;
			float fXPercentage = (m_position.x + fHalfSize) / CTerrain::m_fSize;
			float fZPercentage = (m_position.z + fHalfSize) / CTerrain::m_fSize;

			if (fXPercentage <= 1.0f && fXPercentage >= 0.0f && fZPercentage <= 1.0f && fZPercentage >= 0.0f)
			{
				int iPerlinXvalue = static_cast<int>(fXPercentage * static_cast<float>(PERLIN_WIDTH - 1));
				int iPerlinYvalue = static_cast<int>(fZPercentage * static_cast<float>(PERLIN_HEIGHT - 1));

				if (m_position.y < (static_cast<float>(CTerrain::tenImage[iPerlinXvalue][iPerlinYvalue][0]) * CTerrain::m_fMaxHeight))
					m_bJumping = false;
				//m_position.y += 2.0f;
			}
		}
		else {
			m_bJumping = false;
			if(m_lastMovement.y != 0.0f)
				m_model.bMoving = false;
		}
	}
}

void CPlayer::CalculateTerrainPos()
{
	if (!m_bJumping) {
		float fHalfSize = CTerrain::m_fSize / 2.0f;
		float fXPercentage = (m_position.x + fHalfSize) / CTerrain::m_fSize;
		float fZPercentage = (m_position.z + fHalfSize) / CTerrain::m_fSize;

		if (fXPercentage <= 1.0f && fXPercentage >= 0.0f && fZPercentage <= 1.0f && fZPercentage >= 0.0f)
		{
			int iPerlinXvalue = static_cast<int>(fXPercentage * static_cast<float>(PERLIN_WIDTH - 1));
			int iPerlinYvalue = static_cast<int>(fZPercentage * static_cast<float>(PERLIN_HEIGHT - 1));

			m_position.y = Lerp(m_position.y, (static_cast<float>(CTerrain::tenImage[iPerlinXvalue][iPerlinYvalue][0]) * CTerrain::m_fMaxHeight), 0.2f);
			//m_position.y += 2.0f;
		}
	}
}