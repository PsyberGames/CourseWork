#ifndef MESH_LOADER_H_INCLUDED
#define MESH_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct OBJIndex
{
    unsigned int vertIndex;
    unsigned int UVeeIndex;
    unsigned int normIndex;
    
    bool operator<(const OBJIndex& r) const { return vertIndex < r.vertIndex; }
};

class IndexedMesh
{
public:
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> texCords;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indcies;
    
    void CalculateNormals();
};

class OBJMesh
{
public:
    std::vector<OBJIndex> OBJIndcies;
    std::vector<glm::vec3> verts;
    std::vector<glm::vec2> uvees;
    std::vector<glm::vec3> norms;
    bool gotUVs;
    bool gotNorms;
    
    OBJMesh(const std::string& fileName);
    
	IndexedMesh ToIndexedMesh();
private:
    unsigned int FindLastVertID(const std::vector<OBJIndex*>& idLookup, const OBJIndex* currentID, const IndexedMesh& output);
    void CreateMeshFace(const std::string& line);
    
    glm::vec2 ParseMeshV2(const std::string& line);
    glm::vec3 ParseMeshV3(const std::string& line);
    OBJIndex ParseMeshID(const std::string& input, bool* hasUVs, bool* hasNormals);
};
// OBJ_LOADER_H_INCLUDED
#endif 
