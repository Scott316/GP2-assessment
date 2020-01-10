#include "LoadObject.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareObjectIndexPointer(const ObjectIndex* a, const ObjectIndex* b);
static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseObjectIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseObjectFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

ModelOfObject::ModelOfObject(const std::string& fileName)
{
    hasTheObjectGotUVs = false;
    hasTheObjectGotNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->objectUVS.push_back(ParseObjectVector2(line));
                    else if(lineCStr[1] == 'n')
                        this->objectNormals.push_back(ParseObjectVector3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->objectVertices.push_back(ParseObjectVector3(line));
                break;
                case 'f':
                    CreateAFaceForObject(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void ModelThatHasBeenIndexed::CalculateTheNormals()
{
    for(unsigned int i = 0; i < indicesOfModel.size(); i += 3)
    {
        int i0 = indicesOfModel[i];
        int i1 = indicesOfModel[i + 1];
        int i2 = indicesOfModel[i + 2];

        glm::vec3 v1 = positionsOfModel[i1] - positionsOfModel[i0];
        glm::vec3 v2 = positionsOfModel[i2] - positionsOfModel[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        normalsOfModel[i0] += normal;
        normalsOfModel[i1] += normal;
        normalsOfModel[i2] += normal;
    }
    
    for(unsigned int i = 0; i < positionsOfModel.size(); i++)
        normalsOfModel[i] = glm::normalize(normalsOfModel[i]);
}

ModelThatHasBeenIndexed ModelOfObject::ToModelThatHasBeenIndexed()
{
    ModelThatHasBeenIndexed result;
    ModelThatHasBeenIndexed normalModel;
    
    unsigned int numberOfIndices = indicesOfObject.size();
    
    std::vector<ObjectIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numberOfIndices; i++)
        indexLookup.push_back(&indicesOfObject[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareObjectIndexPointer);
    
    std::map<ObjectIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numberOfIndices; i++)
    {
        ObjectIndex* currentIndex = &indicesOfObject[i];
        
        glm::vec3 currentPosition = objectVertices[currentIndex->indexOfVertex];
        glm::vec2 currentTexCoordinates;
        glm::vec3 currentNormal;
        
        if(hasTheObjectGotUVs)
            currentTexCoordinates = objectUVS[currentIndex->indexOfUV];
        else
            currentTexCoordinates = glm::vec2(0,0);
            
        if(hasTheObjectGotNormals)
            currentNormal = objectNormals[currentIndex->indexOfNormals];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        //Create model to properly generate normals on
        std::map<ObjectIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.positionsOfModel.size();
        
            normalModelIndexMap.insert(std::pair<ObjectIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.positionsOfModel.push_back(currentPosition);
            normalModel.texCoordinatesOfModel.push_back(currentTexCoordinates);
            normalModel.normalsOfModel.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexIndexOfObject(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.positionsOfModel.size();
        
            result.positionsOfModel.push_back(currentPosition);
            result.texCoordinatesOfModel.push_back(currentTexCoordinates);
            result.normalsOfModel.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.indicesOfModel.push_back(normalModelIndex);
        result.indicesOfModel.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!hasTheObjectGotNormals)
    {
        normalModel.CalculateTheNormals();
        
        for(unsigned int i = 0; i < result.positionsOfModel.size(); i++)
            result.normalsOfModel[i] = normalModel.normalsOfModel[indexMap[i]];
    }
    
    return result;
};

unsigned int ModelOfObject::FindLastVertexIndexOfObject(const std::vector<ObjectIndex*>& indexLookup, const ObjectIndex* currentIndex, const ModelThatHasBeenIndexed& result)
{
    unsigned int startIndex = 0;
    unsigned int endIndex = indexLookup.size();
    unsigned int onCurrentIndex = (endIndex - startIndex) / 2 + startIndex;
    unsigned int previousIndex = startIndex;
    
    while(onCurrentIndex != previousIndex)
    {
        ObjectIndex* testIndex = indexLookup[onCurrentIndex];
        
        if(testIndex->indexOfVertex == currentIndex->indexOfVertex)
        {
            unsigned int countStart = onCurrentIndex;
        
            for(unsigned int i = 0; i < onCurrentIndex; i++)
            {
                ObjectIndex* possibleIndex = indexLookup[onCurrentIndex - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->indexOfVertex != currentIndex->indexOfVertex)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                ObjectIndex* possibleIndex = indexLookup[onCurrentIndex + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->indexOfVertex != currentIndex->indexOfVertex)
                    break;
                else if((!hasTheObjectGotUVs || possibleIndex->indexOfUV == currentIndex->indexOfUV)
                    && (!hasTheObjectGotNormals || possibleIndex->indexOfNormals == currentIndex->indexOfNormals))
                {
                    glm::vec3 currentPosition = objectVertices[currentIndex->indexOfVertex];
                    glm::vec2 currentTexCoordinates;
                    glm::vec3 currentNormal;
                    
                    if(hasTheObjectGotUVs)
                        currentTexCoordinates = objectUVS[currentIndex->indexOfUV];
                    else
                        currentTexCoordinates = glm::vec2(0,0);
                        
                    if(hasTheObjectGotNormals)
                        currentNormal = objectNormals[currentIndex->indexOfNormals];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result.positionsOfModel.size(); j++)
                    {
                        if(currentPosition == result.positionsOfModel[j]
                            && ((!hasTheObjectGotUVs || currentTexCoordinates == result.texCoordinatesOfModel[j])
                            && (!hasTheObjectGotNormals || currentNormal == result.normalsOfModel[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->indexOfVertex < currentIndex->indexOfVertex)
                startIndex = onCurrentIndex;
            else
                endIndex = onCurrentIndex;
        }
    
        previousIndex = onCurrentIndex;
        onCurrentIndex = (endIndex - startIndex) / 2 + startIndex;
    }
    
    return -1;
}

void ModelOfObject::CreateAFaceForObject(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->indicesOfObject.push_back(ParseObjectIndex(tokens[1], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));
    this->indicesOfObject.push_back(ParseObjectIndex(tokens[2], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));
    this->indicesOfObject.push_back(ParseObjectIndex(tokens[3], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));

    if((int)tokens.size() > 4)
    {
        this->indicesOfObject.push_back(ParseObjectIndex(tokens[1], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));
        this->indicesOfObject.push_back(ParseObjectIndex(tokens[3], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));
        this->indicesOfObject.push_back(ParseObjectIndex(tokens[4], &this->hasTheObjectGotUVs, &this->hasTheObjectGotNormals));
    }
}

ObjectIndex ModelOfObject::ParseObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int lengthOfToken = token.length();
    const char* stringOfToken = token.c_str();
    
    unsigned int vertexIndexStart = 0;
    unsigned int vertexIndexEnd = FindNextCharacter(vertexIndexStart, stringOfToken, lengthOfToken, '/');
    
    ObjectIndex result;
    result.indexOfVertex = ParseObjectIndexValue(token, vertexIndexStart, vertexIndexEnd);
    result.indexOfUV = 0;
    result.indexOfNormals = 0;
    
    if(vertexIndexEnd >= lengthOfToken)
        return result;
    
    vertexIndexStart = vertexIndexEnd + 1;
    vertexIndexEnd = FindNextCharacter(vertexIndexStart, stringOfToken, lengthOfToken, '/');
    
    result.indexOfUV = ParseObjectIndexValue(token, vertexIndexStart, vertexIndexEnd);
    *hasUVs = true;
    
    if(vertexIndexEnd >= lengthOfToken)
        return result;
    
    vertexIndexStart = vertexIndexEnd + 1;
    vertexIndexEnd = FindNextCharacter(vertexIndexStart, stringOfToken, lengthOfToken, '/');
    
    result.indexOfNormals = ParseObjectIndexValue(token, vertexIndexStart, vertexIndexEnd);
    *hasNormals = true;
    
    return result;
}

glm::vec3 ModelOfObject::ParseObjectVector3(const std::string& line)
{
    unsigned int lengthOfToken = line.length();
    const char* stringOfToken = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < lengthOfToken)
    {
        if(stringOfToken[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, stringOfToken, lengthOfToken, ' ');
    
    float x = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, stringOfToken, lengthOfToken, ' ');
    
    float y = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, stringOfToken, lengthOfToken, ' ');
    
    float z = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x,y,z);

 
}

glm::vec2 ModelOfObject::ParseObjectVector2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextCharacter(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseObjectFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareObjectIndexPointer(const ObjectIndex* a, const ObjectIndex* b)
{
    return a->indexOfVertex < b->indexOfVertex;
}

static inline unsigned int FindNextCharacter(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline unsigned int ParseObjectIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseObjectFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
