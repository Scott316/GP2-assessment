#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ObjectIndex
{
    unsigned int indexOfVertex;
    unsigned int indexOfUV;
    unsigned int indexOfNormals;
    
    bool operator<(const ObjectIndex& r) const { return indexOfVertex < r.indexOfVertex; }
};

class ModelThatHasBeenIndexed
{
public:
    std::vector<glm::vec3> positionsOfModel;
    std::vector<glm::vec2> texCoordinatesOfModel;
    std::vector<glm::vec3> normalsOfModel;
    std::vector<unsigned int> indicesOfModel;
    
    void CalculateTheNormals();
};

class ModelOfObject
{
public:
    std::vector<ObjectIndex> indicesOfObject;
    std::vector<glm::vec3> objectVertices;
    std::vector<glm::vec2> objectUVS;
    std::vector<glm::vec3> objectNormals;
    bool hasTheObjectGotUVs;
    bool hasTheObjectGotNormals;
    
    ModelOfObject(const std::string& fileName);
    
    ModelThatHasBeenIndexed ToModelThatHasBeenIndexed();
private:
    unsigned int FindLastVertexIndexOfObject(const std::vector<ObjectIndex*>& indexLookup, const ObjectIndex* currentIndex, const ModelThatHasBeenIndexed& result);
    void CreateAFaceForObject(const std::string& line);
    
    glm::vec2 ParseObjectVector2(const std::string& line);
    glm::vec3 ParseObjectVector3(const std::string& line);
    ObjectIndex ParseObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
