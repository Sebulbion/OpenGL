//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Utils.h
// Description	: some useful defines and structs
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __UTILLS_H__
#define __UTILLS_H__

#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <random>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1024
#define PERLIN_WIDTH  250
#define PERLIN_HEIGHT 250
#define BUFFER_OFFSET(i) ((GLvoid*)(i*sizeof(float)))
#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2
#define Lerp(a,b,c) a + c * (b - a)
#define NUM_PARTICLES 128 * 20000

// A neater interface for arrays of arrays of arrays
template <typename T, size_t I, size_t J, size_t K>
using Tensor = std::array<std::array<std::array<T, K>, J>, I>;

// The data in glm::vec3 and 2 is packed next to each other so it can be sent to the shaders
struct VertexFormat
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCord;
};

enum ModelType
{
	SQUARE,
	SKYBOX,
	FLOOR,
	CUBOID,
	SPHERE,
	TERRAIN,
	STAR,
	PARTICLE_SYSTEM,
	PLAYER
};

static float randomFloat() {
	float random = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return random;
}
#endif //__UTILLS_H__