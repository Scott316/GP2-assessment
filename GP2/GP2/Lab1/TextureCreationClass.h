#pragma once
#include <string>
#include <GL\glew.h>

class TextureCreation
{
public:
	TextureCreation(const std::string& fileName);

	void BindOurTexture(unsigned int unit); // bind upto 32 textures

	~TextureCreation();

protected:
private:

	GLuint handleOurTexture;
};
