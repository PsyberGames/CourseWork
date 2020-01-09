#include "mesh_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

OBJMesh::OBJMesh(const std::string& fileName)
{
	gotUVs = false;
	gotNorms = false;
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
                        this->uvees.push_back(ParseMeshV2(line));
                    else if(lineCStr[1] == 'n')
                        this->norms.push_back(ParseMeshV3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->verts.push_back(ParseMeshV3(line));
                break;
                case 'f':
					CreateMeshFace(line);
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

void IndexedMesh::CalculateNormals()
{
    for(unsigned int i = 0; i < indcies.size(); i += 3)
    {
        int i0 = indcies[i];
        int i1 = indcies[i + 1];
        int i2 = indcies[i + 2];

        glm::vec3 v1 = pos[i1] - pos[i0];
        glm::vec3 v2 = pos[i2] - pos[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        norms[i0] += normal;
        norms[i1] += normal;
        norms[i2] += normal;
    }
    
    for(unsigned int i = 0; i < pos.size(); i++)
        norms[i] = glm::normalize(norms[i]);
}

IndexedMesh OBJMesh::ToIndexedMesh()
{
	IndexedMesh result;
	IndexedMesh normalModel;
    
    unsigned int numIndices = OBJIndcies.size();
    
    std::vector<OBJIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&OBJIndcies[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);
    
    std::map<OBJIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJIndcies[i];
        
        glm::vec3 currentPosition = verts[currentIndex->vertIndex];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(gotUVs)
            currentTexCoord = uvees[currentIndex->UVeeIndex];
        else
            currentTexCoord = glm::vec2(0,0);
            
        if(gotNorms)
            currentNormal = norms[currentIndex->normIndex];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        //Create model to properly generate normals on
        std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.pos.size();
        
            normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.pos.push_back(currentPosition);
            normalModel.texCords.push_back(currentTexCoord);
            normalModel.norms.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertID(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.pos.size();
        
            result.pos.push_back(currentPosition);
            result.texCords.push_back(currentTexCoord);
            result.norms.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.indcies.push_back(normalModelIndex);
        result.indcies.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!gotNorms)
    {
        normalModel.CalculateNormals();
        
        for(unsigned int i = 0; i < result.pos.size(); i++)
            result.norms[i] = normalModel.norms[indexMap[i]];
    }
    
    return result;
};

unsigned int OBJMesh::FindLastVertID(const std::vector<OBJIndex*>& idLookup, const OBJIndex* currentID, const IndexedMesh& output)
{
    unsigned int start = 0;
    unsigned int end = idLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = idLookup[current];
        
        if(testIndex->vertIndex == currentID->vertIndex)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = idLookup[current - i];
                
                if(possibleIndex == currentID)
                    continue;
                    
                if(possibleIndex->vertIndex != currentID->vertIndex)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < idLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = idLookup[current + i];
                
                if(possibleIndex == currentID)
                    continue;
                    
                if(possibleIndex->vertIndex != currentID->vertIndex)
                    break;
                else if((!gotUVs || possibleIndex->UVeeIndex == currentID->UVeeIndex)
                    && (!gotNorms || possibleIndex->normIndex == currentID->normIndex))
                {
                    glm::vec3 currentPosition = verts[currentID->vertIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    
                    if(gotUVs)
                        currentTexCoord = uvees[currentID->UVeeIndex];
                    else
                        currentTexCoord = glm::vec2(0,0);
                        
                    if(gotNorms)
                        currentNormal = norms[currentID->normIndex];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < output.pos.size(); j++)
                    {
                        if(currentPosition == output.pos[j]
                            && ((!gotUVs || currentTexCoord == output.texCords[j])
                            && (!gotNorms || currentNormal == output.norms[j])))
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
            if(testIndex->vertIndex < currentID->vertIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void OBJMesh::CreateMeshFace(const std::string& line)
{
    std::vector<std::string> input = SplitString(line, ' ');

    this->OBJIndcies.push_back(ParseMeshID(input[1], &this->gotUVs, &this->gotNorms));
    this->OBJIndcies.push_back(ParseMeshID(input[2], &this->gotUVs, &this->gotNorms));
    this->OBJIndcies.push_back(ParseMeshID(input[3], &this->gotUVs, &this->gotNorms));

    if((int)input.size() > 4)
    {
        this->OBJIndcies.push_back(ParseMeshID(input[1], &this->gotUVs, &this->gotNorms));
        this->OBJIndcies.push_back(ParseMeshID(input[3], &this->gotUVs, &this->gotNorms));
        this->OBJIndcies.push_back(ParseMeshID(input[4], &this->gotUVs, &this->gotNorms));
    }
}

OBJIndex OBJMesh::ParseMeshID(const std::string& input, bool* hasUVs, bool* hasNormals)
{
    unsigned int inputLength = input.length();
    const char* inputString = input.c_str();
    
    unsigned int vertIDStart = 0;
    unsigned int vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, '/');
    
    OBJIndex output;
	output.vertIndex = ParseOBJIndexValue(input, vertIDStart, vertIDEnd);
	output.UVeeIndex = 0;
	output.normIndex = 0;
    
    if(vertIDEnd >= inputLength)
        return output;
    
    vertIDStart = vertIDEnd + 1;
    vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, '/');
    
	output.UVeeIndex = ParseOBJIndexValue(input, vertIDStart, vertIDEnd);
    *hasUVs = true;
    
    if(vertIDEnd >= inputLength)
        return output;
    
    vertIDStart = vertIDEnd + 1;
    vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, '/');
    
	output.normIndex = ParseOBJIndexValue(input, vertIDStart, vertIDEnd);
    *hasNormals = true;
    
    return output;
}

glm::vec3 OBJMesh::ParseMeshV3(const std::string& line)
{
    unsigned int inputLength = line.length();
    const char* inputString = line.c_str();
    
    unsigned int vertIDStart = 2;
    
    while(vertIDStart < inputLength)
    {
        if(inputString[vertIDStart] != ' ')
            break;
        vertIDStart++;
    }
    
    unsigned int vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIDStart, vertIDEnd);
    
    vertIDStart = vertIDEnd + 1;
    vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIDStart, vertIDEnd);
    
    vertIDStart = vertIDEnd + 1;
    vertIDEnd = FindNextChar(vertIDStart, inputString, inputLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIDStart, vertIDEnd);
    
    return glm::vec3(x,y,z);

    
}

glm::vec2 OBJMesh::ParseMeshV2(const std::string& line)
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
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertIndex < b->vertIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
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

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
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
