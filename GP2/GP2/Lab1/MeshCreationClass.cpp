#include "MeshCreationClass.h"
#include <vector>

void MeshCreationClass::initialiseMeshes(VertexStruct* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{

	ModelThatHasBeenIndexed currentModel; //all the data of the current data associated with the current model is stored.

	for (unsigned int i = 0; i < numVertices; i++) //If one or more model is active,
	{
		//the method will call the GetPosition(), GetTexCoordinates() and GetNormal() methodsand sets their value as the current model’s position, textCoordsand normal.
		currentModel.positionsOfModel.push_back(*vertices[i].GetPosition());
		currentModel.texCoordinatesOfModel.push_back(*vertices[i].GetTexCoordinates());
		currentModel.normalsOfModel.push_back(*vertices[i].GetNormal());
	}

	//The program then uses a similar method to retrieve the indices of the model.
	for (unsigned int i = 0; i < numIndices; i++)
		currentModel.indicesOfModel.push_back(indices[i]);

	initialiseGameModel(currentModel); //  The initialiseGameModel method is then run and the data of the current model is passed in to it.
}
void MeshCreationClass::initialiseGameModel(const ModelThatHasBeenIndexed& currentModel)
{
	amountToDraw = currentModel.indicesOfModel.size(); //declares the amount of the vertexArrayObject we want to draw is equal to the size of the current model.

	glGenVertexArrays(1, &vertexArrayObject); //A vertex array will then be generated, and the array will be stored in the vertex array object.
	glBindVertexArray(vertexArrayObject); //The program will then bind our vertex array object.

	glGenBuffers(NUMBEROFBUFFERS, vertexArrayBuffers); //buffers for the shaders will be generated and will be based on our current array of data and buffers.

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, currentModel.positionsOfModel.size() * sizeof(currentModel.positionsOfModel[0]), &currentModel.positionsOfModel[0], GL_STATIC_DRAW); //The glBufferData moves data to the GPU. The information contained in the brackets represents the type of data being passed and its size, the data’s pointer and where the data is stored on the GPU.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORDINATES_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, currentModel.positionsOfModel.size() * sizeof(currentModel.texCoordinatesOfModel[0]), &currentModel.texCoordinatesOfModel[0], GL_STATIC_DRAW); //The glBufferData moves data to the GPU. The information contained in the brackets represents the type of data being passed and its size, the data’s pointer and where the data is stored on the GPU.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VERTEXBUFFER]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(currentModel.normalsOfModel[0]) * currentModel.normalsOfModel.size(), &currentModel.normalsOfModel[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentModel.indicesOfModel.size() * sizeof(currentModel.indicesOfModel[0]), &currentModel.indicesOfModel[0], GL_STATIC_DRAW); //The glBufferData moves data to the GPU. The information contained in the brackets represents the type of data being passed and its size, the data’s pointer and where the data is stored on the GPU.

	glBindVertexArray(0); // Finally, the glBindVertexArray function is used to unbind our vertex array object.
}

MeshCreationClass::MeshCreationClass()
{
	amountToDraw = NULL; //the amount of the vertexArrayObject we want to draw is made to equal  0.
}

void MeshCreationClass::loadModelToGame(const std::string& filename)
{
	
	ModelThatHasBeenIndexed model = ModelOfObject(filename).ToModelThatHasBeenIndexed(); //takes model from specified file, converts it to  indexed model and assigns it to an IndexedModel variable called model
	initialiseGameModel(model); //The method then calls the initialiseGameModel() methodand uses it to initialise the model attached to the variable it has just declared
	SphereStruct meshSphere(); //Finally, the code calls the meshSphere method from the SphereStruct struct to add a sphere that detects collision to the model that is being loaded.
}

MeshCreationClass::~MeshCreationClass()
{
	glDeleteVertexArrays(1, &vertexArrayObject); //glDeleteVertexArrays deletes all the vertex arrays.
}
void MeshCreationClass::drawMeshToGame()
{
	glBindVertexArray(vertexArrayObject); // used to bind the specified vertex array object.

	glDrawElements(GL_TRIANGLES, amountToDraw, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshCreationClass::updateSphereData(glm::vec3 pos, float radius)
{
	//the SetPos() and SetRadius() methods are called and used to set the position and radius of the sphere within the mesh.
	sphereInMesh.SetPos(pos);
	sphereInMesh.SetRadius(radius);
}

