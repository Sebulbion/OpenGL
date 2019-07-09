#ifndef _SHADER_HELPER__
#define _SHADER_HELPER__

#include <glew.h>
#include <freeglut.h>
#include <cstring>
#include <string>

class ShaderHelper {
private:
	//static const std::string TAG;

public:
	ShaderHelper();
	static std::string readShaderFileFromResource(const char* pFileName);
	static GLuint compileComputeShader(const char* shaderCode);
	static GLuint compileVertexShader(const char* shaderCode);
	static GLuint compileFragmentShader(const char* shaderCode);
	static GLuint compileGeometryShader(const char* shaderCode);
	static GLuint compileTessellationControlShader(const char* shaderCode);
	static GLuint compileTessellationEvaluationShader(const char* shaderCode);
	static GLuint compileShader(GLenum ShaderType, const char* shaderCode);
	static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
	static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint geometryShaderId);
	static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId, GLuint geometryShaderId, GLuint tessellationControlShaderId, GLuint tessellationEvaluationShaderId);
	static GLint validateProgram(GLuint programObjectId);
	static void compileAndLinkShaders(std::string compute_shader, GLuint& program);
	static void compileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, GLuint& program);
	static void compileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, std::string geometry_shader, GLuint& program);
	static void compileAndLinkShaders(std::string vertex_shader, std::string fragment_shader, std::string geometry_shader, std::string tessellation_control_shader, 
									  std::string tessellation_evaluation_shader, GLuint& program);
};

//std::string TAG = "ShaderHelper";
#endif