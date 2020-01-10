#include "TextureCreationClass.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

TextureCreation::TextureCreation(const std::string& fileName)
{
	int imageWidth;
	int imageHeight;
	int numberOfComponents; 
	unsigned char* dataOfImage = stbi_load((fileName).c_str(), &imageWidth, &imageHeight, &numberOfComponents, 4);

	if (dataOfImage == NULL) //However, if this data is unavailable, 
	{
		std::cerr << "texture load failed" << fileName << std::endl; //an error message will appear onscreen.
	}

	//The textures are then generated using the specified numberand addresses.
	//The program then binds the texture and specified that it is a 2D texture type.
	glGenTextures(1, &handleOurTexture); 
	glBindTexture(GL_TEXTURE_2D, handleOurTexture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // wrap texture outside height

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear filtering for magnifcation (texture is larger)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataOfImage); //Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Image Data

	stbi_image_free(dataOfImage);
}

TextureCreation::~TextureCreation()
{
	glDeleteTextures(1, &handleOurTexture); //glDeleteTexture takes specified number of textures and texture address and deletes all the information stored at each address. 
}

void TextureCreation::BindOurTexture(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31); // checks texture we are working with is in the correct unit range.
	glActiveTexture(GL_TEXTURE0 + unit); //glActiveTexture is then used to appropriately select an active texture unit. 
	glBindTexture(GL_TEXTURE_2D, handleOurTexture); //glBindTexture is used to bind specified texture to GL_TEXTURE_2D target.
}
