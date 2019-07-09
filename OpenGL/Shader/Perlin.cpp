#include "Perlin.h"



CPerlin::CPerlin()
{
}


CPerlin::~CPerlin()
{
}

float CPerlin::Random(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t) * 0.931322574615478515625e-9;
}

float CPerlin::Smooth(float x, float y)
{
	float corners = (Random(x - 1.0f, y - 1.0f) + Random(x + 1.0f, y - 1.0f) + Random(x - 1.0f, y + 1.0f) + Random(x + 1.0f, y + 1.0f)) / 16.0f;
	float sides = (Random(x - 1.0f, y) + Random(x + 1.0f, y) + Random(x, y - 1.0f) + Random(x, y + 1.0f)) / 8.0f;
	float center = Random(x, y) / 4.0f;
	return corners + sides + center;
}

float CPerlin::Interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5f;
	return a*(1 - f) + b*f;
}

float CPerlin::Noise(float x, float y)
{
	float	fractional_X = x - int(x);
	float 	fractional_Y = y - int(y);
	//float 	fractional_Z = z - int(z);

	//smooths
	float v1 = Smooth(int(x), int(y));
	float v2 = Smooth(int(x) + 1, int(y));
	float v3 = Smooth(int(x), int(y) + 1);
	float v4 = Smooth(int(x) + 1, int(y) + 1);

	// interpolates
	float i1 = Interpolate(v1, v2, fractional_X);
	float i2 = Interpolate(v3, v4, fractional_X);

	float final = Interpolate(i1, i2, fractional_Y);

	return final;
}

float CPerlin::TotalNoisePerPoint(int x, int y, int _iOctaves, float _fZoom, float _fPersistance)
{
	//int octaves = 7;
	//float zoom = 20.0f;
	//float persistance = 0.5f;

	float total = 0.0f;

	for (int i = 0; i < _iOctaves - 1; i++) {

		float frequency = pow(2, i) / _fZoom;
		float amplitude = pow(_fPersistance, i);

		total += Noise(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

void CPerlin::PerlinNoise(Tensor<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3>& _rtenImage, int _iOctaves, float _fZoom, float _fPersistance, int _iXoffset, int _iYoffset)
{
	for (size_t i = 0; i < PERLIN_WIDTH; ++i)
	{
		for (size_t j = 0; j < PERLIN_HEIGHT; ++j)
		{
			float fPerlinValue = TotalNoisePerPoint(i + _iXoffset, j + _iYoffset, _iOctaves, _fZoom, _fPersistance);
			fPerlinValue = (fPerlinValue + 1) / 2;
			_rtenImage[i][j][0] = fPerlinValue;		//R
			_rtenImage[i][j][1] = fPerlinValue;		//G
			_rtenImage[i][j][2] = fPerlinValue;		//B
		}
	}
}
