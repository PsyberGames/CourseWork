#include "CreateMesh.h"
#include <vector>




CreateMesh::CreateMesh()
{
	drawCount = NULL;
}



CreateMesh::~CreateMesh()
{
	// delete arrays
	glDeleteVertexArrays(1, &vertexArrayObject); 
}

void CreateMesh::LoadMesh(const std::string& filename)
{
	IndexedMesh mesh = OBJMesh(filename).ToIndexedMesh();
	initMesh(mesh);
	SphereCollider meshSphere();
}
void CreateMesh::initMesh(const IndexedMesh& mesh)
{

	drawCount = mesh.indcies.size();
	//generate a vertex array and store it in the VAO
	glGenVertexArrays(1, &vertexArrayObject);
	//bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)
	glBindVertexArray(vertexArrayObject);
	//generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];
	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	//tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]);
	//move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glBufferData(GL_ARRAY_BUFFER, mesh.pos.size() * sizeof(mesh.pos[0]), &mesh.pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	//move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glBufferData(GL_ARRAY_BUFFER, mesh.pos.size() * sizeof(mesh.texCords[0]), &mesh.texCords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.norms[0]) * mesh.norms.size(), &mesh.norms[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
	//move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indcies.size() * sizeof(mesh.indcies[0]), &mesh.indcies[0], GL_STATIC_DRAW);
	// unbind our VAO
	glBindVertexArray(0);
}

void CreateMesh::Draw()
{
	glBindVertexArray(vertexArrayObject);

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void CreateMesh::UpdateSphereColData(glm::vec3 pos, float radius)
{
	sphereCollider.SetPos(pos);
	sphereCollider.SetRadius(radius);
}

void CreateMesh::init(Vert* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedMesh mesh;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		mesh.pos.push_back(*vertices[i].GetPos());
		mesh.texCords.push_back(*vertices[i].GetTexCoord());
		mesh.norms.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		mesh.indcies.push_back(indices[i]);

	initMesh(mesh);
}

