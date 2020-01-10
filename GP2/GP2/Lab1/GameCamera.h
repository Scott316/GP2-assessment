#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct GameCamera
{
public:
	GameCamera()
	{
	}
	void initialiseGameCamera(const glm::vec3& position, float fieldofview, float cameraAspect, float nearClip, float farClip)
	{
		this->position = position; //sets the camera’s position on the x-axis.
		this->cameraForward = glm::vec3(0.0f, 0.0f, 1.0f); //position on z-axis is set to 1.0.
		this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // Position on y-axis is set to 1.0.
		this->cameraProjection = glm::perspective(fieldofview, cameraAspect, nearClip, farClip); //cameraProjection is set to camera’s projection matrix - made up of camera’s field of view, the aspect ratio, the near clip and the far clip. 
	}
	void MoveForward(float amt)
	{
	//multiplies position on z-axis by specified amount. Result is added to position of camera and the camera will move accordingly.
		position += cameraForward * amt;
	}
	void MoveRight(float amt)
	{
	//stores cross product of y and z - axis then multiplies it by amount specified amount.
	//The result is then added to the position of the camera and the camera will move accordingly.
		position += glm::cross(cameraUp, cameraForward) * amt;
	}
	void left(float amt)
	{
	//stores cross product of y-axis and z-axis and multiplies by specified amount. 
	//result is subtracted from position of camera and the camera will move accordingly. 
		position -= glm::cross(cameraUp, cameraForward) * amt;
	}
	void back(float amt)
	{
	//multiply position on the z-axis by specified amount. 
	//result is subtracted from position of camera and the camera will move accordingly.
		position -= cameraForward * amt;
	}
	void MoveUp(float amt)
	{
	//multiplies position on the y-axis by the amount. 
	//result is added to the position and the camera will move accordingly.
		position += cameraUp * amt;
	}
	void MoveDown(float amt)
	{
		//multiply the position on the y - axis by the amount.
		//result is then subtracted from the position of the camera and the camera will move accordingly.
		position -= cameraUp * amt;
	}
	glm::vec3 getPosition()
	{
		
		return this->position; //returns the camera’s position to the game. 
	}
	inline glm::mat4 GetViewProjection() const
	{
	//creates view projection matrix by multiplying the camera’s projection matrice by the view variable. 
    //Once this calculation is complete, the program returns the result to the game. 
		return cameraProjection * glm::lookAt(position, position + cameraForward, cameraUp);
	}
protected:
private:
//sets up a matrix to hold the data for the camera’s projection matrix as well as 3D vectors to hold the camera’s 
//position on all three axes. 
	glm::mat4 cameraProjection;
	glm::vec3 position;
	glm::vec3 cameraForward;
	glm::vec3 cameraUp;		
};