#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "GameCamera.h"

class ShaderCreationClass
{
public:
	ShaderCreationClass(const std::string& filename);

	void BindGameShaders(); //Set gpu to use our shaders
	void UpdateGameShaders(const Transform& transform, const GameCamera& camera);

	std::string ShaderCreationClass::LoadGameShaders(const std::string& fileName);
	void ShaderCreationClass::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint ShaderCreationClass::CreateGameShaders(const std::string& text, unsigned int type);

	~ShaderCreationClass();


protected:
private:
	static const unsigned int NUMBEROFSHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_UNIFORM,

		NUM_UNIFORMS
	};

	GLuint trackProgram; // Track the shader program
	GLuint shadersArray[NUMBEROFSHADERS]; //array of shaders
	GLuint uniformsArray[NUM_UNIFORMS]; //no of uniform variables
};