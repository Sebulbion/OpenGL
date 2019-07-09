//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: Terrain.h
// Description	: The terrain object
// Author		: Sebastian Tengdahl
// Mail			: sebastian.ten7230@mediadesign.school.nz
//
#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "Object.h"
class CTerrain :
	public CObject
{
public:
	CTerrain(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath);
	~CTerrain();

	void DrawObject() override;
	// Renders the object for the shadow map. Renders the entire terrain in the same way as draw but the fragment shader is different and the vp part of the mvp is the shadow maps vp
	void ShadowPass();
	// Sets the level of detail for the tessellation shader
	void SetLevelOfDetail(int _bNewDetail);

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CTerrain* CreateTerrain(GLuint _shaders, GLuint _shadowShaders, glm::vec3 _position);
	static float m_fSize, m_fMaxHeight;
	static Tensor<float, PERLIN_HEIGHT, PERLIN_WIDTH, 3> tenImage;
private:
	// Gets the location of some variables for the shader for later edditiong
	void getUniformLocation(GLuint shader);
	// Creates the texture and sends it to the shader
	void CreateAndBindTexture();

	// Sends the vertex data to the shader
	// Takes the vertex data
	void CreateVBOAttributes(std::vector<VertexFormat> _vecVertices);
	// Sends what order the vertices shuld be used
	// Takes the indices which contains the order of the vertices
	void CreateEBO(std::vector<GLuint> _vecIndices);

	// Creates the terrain based on m_fSize
	// Returns a vector of the vertices
	static std::vector<VertexFormat> CreateVertices();
	// Generates the indices in sets of 3
	// Returns a vector of indices
	static std::vector<GLuint> CreateIndices();

	GLuint VBO, VAO, EBO, aPositionLocation, aColorLocation, aTextureLocation;
	GLuint gScaleLocation, gRotateLocation, gTranslateLocation, gSampler, gMVPLocation, gLightMVPLocation, gCameraPos, gCurrentTimeLocation, gMaxHeightLocation, gGrassSampler, gCameraFrontLocation
		, gShadowMapSampler;
	GLuint m_shaders, m_shadowShader;
	GLfloat currentTime;
	Texture* pTexture;
	Texture* pGrassTexture;
	std::string m_strImagePath;

	GLuint gLevelOfDetailLocation;
	int m_iLevelOfDetail;

	static int m_iNumOfCols, m_iNumOfRows;

	int m_iIndices;
};

#endif // __TERRAIN_H__