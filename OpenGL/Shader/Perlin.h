//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Perlin.h
// Description	: creates a pelin noise texture and populates a 3d array with it
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//

#ifndef __PERLIN_H__
#define __PERLIN_H__

#include "Utils.h"

class CPerlin
{
public:
	CPerlin();
	~CPerlin();

private:
	static float Random(int x, int y);
	static float Smooth(float x, float y);
	static float Interpolate(float a, float b, float x);
	static float Noise(float x, float y);
	static float TotalNoisePerPoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance);
public:
	static void PerlinNoise(Tensor<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3>& _rtenImage, int _iOctaves, float _fZoom, float _fPersistance, int _iXoffset, int _iYoffset);
};
#endif //__PERLIN_H__

