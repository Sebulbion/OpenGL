#ifndef __STAR_H__
#define __STAR_H__

#include "Object.h"
class CStar :
	public CObject
{
public:
	CStar(std::vector<VertexFormat> _vecVertices, std::vector<GLuint> _vecIndices, GLuint _shaders, glm::vec3 _position, glm::vec3 _rotation, std::string _strImagePath);
	~CStar();

	void DrawObject() override;

	// Creates a model based on the input
	// Takes what shaders the object shuld use and its position in world space
	// Returns the created model
	static CStar* CreateStar(GLuint _shaders, glm::vec3 _position);
	// Increases the multiplyer for the rotation counter
	// Takes what to increase the multiplyer by
	void IncreaseRotationSpeed(float _fValue);
private:
	// Gets the location of some variables for the shader for later edditiong
	void getUniformLocation();
	// Creates the texture and sends it to the shader
	void CreateAndBindTexture();

	// Sends the vertex data to the shader
	// Takes the vertex data
	void CreateVBOAttributes(std::vector<VertexFormat> _vecVertices);
	// Sends what order the vertices shuld be used
	// Takes the indices which contains the order of the vertices
	void CreateEBO(std::vector<GLuint> _vecIndices);

	static std::vector<VertexFormat> CreateVertices();
	static std::vector<GLuint> CreateIndices();

	GLuint VBO, VAO, EBO, aPositionLocation, aColorLocation, shaders, aTextureLocation;
	GLuint gScaleLocation, gRotateLocation, gTranslateLocation, gSampler;
	GLuint m_shaders;
	Texture* pTexture;
	std::string m_strImagePath;

	int m_iIndices;
	float m_fRotationSpeed;
	float m_fRotationCounter;
};

#endif // __STAR_H__