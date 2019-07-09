//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Model.cpp
// Description	: this is a factory class that creates ceveral types of models
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#include "Object.h"
#include "Skybox.h"
#include "Square.h"
#include "Floor.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Terrain.h"
#include "Star.h"
#include "Particle.h"
#include "Player.h"

CObject::~CObject()
{
}

glm::vec3 CObject::GetPosition()
{
	return m_position;
}

CObject::CObject()
{

}

CObject* CObject::CreateObject(ModelType _modelType, GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position)
{
	CObject* model;
	switch (_modelType)
	{
	case SQUARE:
		model = CSquare::CreateSquare(_shaders, _position);
		return model;
		break;
	case SKYBOX:
		model = CSkybox::CreateSkybox(_shaders, _position);
		return model;
		break;
	case FLOOR:
		model = CFloor::CreateFloor(_shaders, _position);
		return model;
		break;
	case CUBOID:
		model = CCuboid::CreateCuboid(_shaders, _position);
		return model;
		break;
	case SPHERE:
		model = CSphere::CreateSphere(_shaders, _shadowShaders, _position);
		return model;
		break;
	case TERRAIN:
		model = CTerrain::CreateTerrain(_shaders, _shadowShaders, _position);
		return model;
		break;
	case STAR:
		model = CStar::CreateStar(_shaders, _position);
		return model;
		break;
	case PARTICLE_SYSTEM:
		model = CParticleSystem::CreateParticleSystem(_shaders, _position);
		return model;
		break;
		case PLAYER:
		model = CPlayer::CreatePlayer(_shaders, _shadowShaders, _position);
		return model;
		break;
	default:
		break;
	}

	return nullptr;
}

ModelType CObject::GetModelType()
{
	return m_eModelType;
}
