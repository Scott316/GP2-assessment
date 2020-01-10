#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "GameCamera.h"

struct Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos; //makes the position variable equal to the position vec3 variable. 
		this->rot = rot; //makes the rotation variable equal to the rotation vec3 variable. 
		this->scale = scale; //makes the scale variable equal to the scale vec3 variable. 
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(pos); //translates model’s position to a 3D world context and stores it in a 4x4 position matrix. 
		glm::mat4 scaleMat = glm::scale(scale); //uses a 4x4 scale matrix to store the scale information for the model.
		glm::mat4 rotX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0)); //matrix is made for rotation on x-axis and made equal to the coordinate 1.0.
		glm::mat4 rotY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0)); //matrix is created to store rotation on y-axis, also set to 1.0
		glm::mat4 rotZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0)); //third matrix is created and stores the 1.0 rotation along the z-axis
		glm::mat4 rotMat = rotX * rotY * rotZ; //total rotation matrix created by multiplying the rotX, rotY and the the rotZ matrices.

		return posMat * rotMat * scaleMat; //code multiplies values of position, rotation and scale matrices and returns result to the game.
	}

	inline glm::vec3* GetPos() { return &pos; } //return the position to the game. 
	inline glm::vec3* GetRot() { return &rot; } //return the rottion to the game.
	inline glm::vec3* GetScale() { return &scale; } //return the scale to the game.

	inline void SetPos(glm::vec3& pos) { this->pos = pos; } // set value of pos vector.
	inline void SetRot(glm::vec3& rot) { this->rot = rot; } // set value of rot vector.
	inline void SetScale(glm::vec3& scale) { this->scale = scale; } //set value of scale vector.
protected:
private:
	// sets up private 3D vector variables to hold the position, rotation and scale information.
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};


