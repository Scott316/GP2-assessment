#include "ShaderCreationClass.h"
#include <iostream>
#include <fstream>

ShaderCreationClass::ShaderCreationClass(const std::string& filename)
{

	trackProgram = glCreateProgram(); // creates a new shader program. OpenGL will save this as a reference.
	shadersArray[0] = CreateGameShaders(LoadGameShaders("..\\res\\shader.vert"), GL_VERTEX_SHADER); // The shadersArray array used to create vertex and fragment shaders
	shadersArray[1] = CreateGameShaders(LoadGameShaders("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); 
	
	for (unsigned int i = 0; i < NUMBEROFSHADERS; i++) //When one or more shaders are created,
	{
		glAttachShader(trackProgram, shadersArray[i]); //they will all be added to the shader array. 
	}

	glBindAttribLocation(trackProgram, 0, "position"); // glBindAttribLocation used to associate our position, texCoordinate and normal variables with our shader program.
	glBindAttribLocation(trackProgram, 1, "texCoord");
	glBindAttribLocation(trackProgram, 2, "normals");

	glLinkProgram(trackProgram); //shader program then linked which will create executables that run on the GPU shaders.
	CheckForShaderError(trackProgram, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // If not the case, error message displayed onscreen.

	glValidateProgram(trackProgram); //checks if the shader program can run based on the current OpenGL state.
	CheckForShaderError(trackProgram, GL_VALIDATE_STATUS, true, "Error: Shader program not valid"); //If not, error message is displayed.

	uniformsArray[TRANSFORM_UNIFORM] = glGetUniformLocation(trackProgram, "transform"); //glGetUniformLocation used to obtain and display the location of the transform uniform variable.
}


ShaderCreationClass::~ShaderCreationClass()
{
	
	for (unsigned int i = 0; i < NUMBEROFSHADERS; i++)
	{
		glDetachShader(trackProgram, shadersArray[i]); //the shaders are detached from the program 
		glDeleteShader(shadersArray[i]); //shaders are then deleted
	}
	glDeleteProgram(trackProgram); // The specified program, in this case, trackProgram, is then deleted.
}

void ShaderCreationClass::BindGameShaders()
{
	
	glUseProgram(trackProgram); //the program specifies a program, which is then installed as part of the rendering state.
}

void ShaderCreationClass::UpdateGameShaders(const Transform& transform, const GameCamera& camera)
{
	
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetModel(); //creates model view projection matrix by taking view projection matrix and multiplying it by the model data.
	glUniformMatrix4fv(uniformsArray[TRANSFORM_UNIFORM], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint ShaderCreationClass::CreateGameShaders(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //creates shader of type specified by program.

	if (shader == 0) //However, if it fails to do so,
		std::cerr << "Error type creation failed " << type << std::endl; //an error message will be displayed.

	const GLchar* stringSource[1]; //strings associated with shaders will be converted in to lists of c-strings. 
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //The source code then gets sent to OpenGL 
	glCompileShader(shader);//and OpenGL will then compile the shader code.

	CheckForShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //If it fails to accomplish this task, an error message will be displayed. The shader is then returned to the game.

	return shader;
}

std::string ShaderCreationClass::LoadGameShaders(const std::string& fileName)
{
	//loads in and opens a texture from file and loads it as a string. if file is open and functioning appropriately, 
	//program will read every line and output it to the game, taking a new line where appropriate. If the shader fails to load, 
	//the game will display an error message. The output is then returned to the game.
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void ShaderCreationClass::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	//checks the shader program to see if the creation has been successful. 
	//If not, it sends back an error message detailing the error and sets the success variable to false.
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
