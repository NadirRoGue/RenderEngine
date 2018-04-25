/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Mesh.h"

#include "CustomMaths.h"

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include <gl\glew.h>

Engine::Mesh::Mesh()
{
	faces = 0;
	vertices = colors = normals = tangents = uvs = 0;
}

Engine::Mesh::Mesh(aiMesh * mesh)
{
	faces = 0;
	vertices = colors = normals = tangents = uvs = 0;

	loadFromMesh(mesh);

	syncGPU();
}

Engine::Mesh::Mesh(const Engine::Mesh &other)
{
	faces = 0;
	vertices = colors = normals = tangents = uvs = 0;

	numFaces = other.numFaces;
	numVertices = other.numVertices;

	if (other.faces != 0)
	{
		faces = new unsigned int[numFaces * 3];
		memcpy(faces, other.faces, numFaces * 3 * sizeof(unsigned int));
	}

	const unsigned int bufferSize = numVertices * 3;
	const unsigned int copySize = bufferSize * sizeof(float);

	if (other.vertices != 0)
	{
		vertices = new float[bufferSize];
		memcpy(vertices, other.vertices, copySize);
	}

	if (other.normals != 0)
	{
		normals = new float[bufferSize];
		memcpy(normals, other.normals, copySize);
	}

	if (other.colors != 0)
	{
		colors = new float[bufferSize];
		memcpy(colors, other.colors, copySize);
	}

	if (other.tangents != 0)
	{
		tangents = new float[bufferSize];
		memcpy(tangents, other.tangents, copySize);
	}

	if (other.uvs != 0)
	{
		uvs = new float[numVertices * 2];
		memcpy(uvs, other.uvs, numVertices * 2 * sizeof(float));
	}

	verticesPerFace = other.verticesPerFace;

	vao = other.vao;
	vboColors = other.vboColors;
	vboFaces = other.vboFaces;
	vboNormals = other.vboNormals;
	vboTangents = other.vboTangents;
	vboUVs = other.vboUVs;
	vboVertices = other.vboVertices;
}

Engine::Mesh::Mesh(const unsigned int numF, const unsigned int numV, const unsigned int *f, const float *v, const float *c, const float *n, const float *uv, const float *t)
	:numFaces(numF), numVertices(numV)
{
	faces = 0;
	vertices = colors = normals = tangents = uvs = 0;

	if (numFaces > 0)
	{
		if (f != 0)
		{
			verticesPerFace = 3;
			faces = new unsigned int[numFaces * 3];
			memcpy(faces, f, numFaces * 3 * sizeof(unsigned int));
		}
	}

	if (numVertices > 0)
	{
		unsigned int totalV = numVertices * 3;
		if (v != 0)
		{
			vertices = new float[totalV];
			memcpy(vertices, v, totalV * sizeof(float));
		}

		if (c != 0)
		{
			colors = new float[totalV];
			memcpy(colors, c, totalV * sizeof(float));
		}

		if (n != 0)
		{
			normals = new float[totalV];
			memcpy(normals, n, totalV * sizeof(float));
		}

		if (uv != 0)
		{
			uvs = new float[numVertices * 2];
			memcpy(uvs, uv, numVertices * 2 * sizeof(float));
		}

		if (t != 0)
		{
			tangents = new float[totalV];
			memcpy(tangents, t, totalV * sizeof(float));
		}
	}

	syncGPU();
}

void Engine::Mesh::loadFromMesh(aiMesh * mesh)
{
	extractTopology(mesh);
	extractGeometry(mesh);
	computeNormals();
	computeTangents();
}

Engine::Mesh::~Mesh()
{
	releaseCPU();
	releaseGPU();
}

void Engine::Mesh::extractTopology(aiMesh * mesh)
{
	numFaces = mesh->mNumFaces;

	if (numFaces == 0)
	{
		return;
	}
	
	verticesPerFace = mesh->mFaces[0].mNumIndices;
	
	faces = new unsigned int[numFaces * verticesPerFace];

	for (unsigned int i = 0; i < numFaces; i++)
	{
		const int start = i * verticesPerFace;
		memcpy(faces + start, mesh->mFaces[i].mIndices, sizeof(unsigned int) * 3);
	}
}

void Engine::Mesh::extractGeometry(aiMesh * mesh)
{
	numVertices = mesh->mNumVertices;

	vertices = new float[numVertices * 3];
	uvs = new float[numVertices * 2];

	for (unsigned int i = 0; i < numVertices; i++)
	{
		// Vertex
		const int vStart = i * 3;
		const aiVector3D vertex = mesh->mVertices[i];
		vertices[vStart]		= vertex.x;
		vertices[vStart + 1]	= vertex.y;
		vertices[vStart + 2]	= vertex.z;

		// UVs (we load only channel 0)
		const int uvStart = i * 2;
		const aiVector3D uvVector = mesh->mTextureCoords[0][i];
		uvs[uvStart]		= uvVector.x;
		uvs[uvStart + 1]	= uvVector.y;
	}
}

void Engine::Mesh::computeNormals()
{
	std::vector<glm::vec3> perfaceNormals;
	perfaceNormals.resize(numVertices, glm::vec3(0, 0, 0));

	std::vector<float> voronoiArea;
	voronoiArea.resize(numVertices, 0.0f);

	for (unsigned int i = 0; i < numFaces; i++)
	{
		const int start = i * 3;

		// Vertex indexes
		unsigned int raw_a = faces[start];
		unsigned int raw_b = faces[start + 1];
		unsigned int raw_c = faces[start + 2];

		// Vertex indexes within our array
		unsigned int a = raw_a * 3;
		unsigned int b = raw_b * 3;
		unsigned int c = raw_c * 3;

		glm::vec3 A(vertices[a], vertices[a + 1], vertices[a + 2]);
		glm::vec3 B(vertices[b], vertices[b + 1], vertices[b + 2]);
		glm::vec3 C(vertices[c], vertices[c + 1], vertices[c + 2]);

		glm::vec3 faceNormal = glm::normalize(glm::cross(B - A, C - A));
		
		glm::vec3 faceAreas = voronoiTriangleAreas(A, B, C);

		perfaceNormals[raw_a] += faceNormal * faceAreas.x;
		perfaceNormals[raw_b] += faceNormal * faceAreas.y;
		perfaceNormals[raw_c] += faceNormal * faceAreas.z;

		voronoiArea[raw_a] += faceAreas.x;
		voronoiArea[raw_b] += faceAreas.y;
		voronoiArea[raw_c] += faceAreas.z;
	}

	normals = new float[numVertices * 3];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		perfaceNormals[i] /= voronoiArea[i];

		glm::vec3 normal = glm::normalize(perfaceNormals[i]);
		
		const int start = i * 3;
		normals[start] = normal.x;
		normals[start + 1] = normal.y;
		normals[start + 2] = normal.z;
	}
}

void Engine::Mesh::computeTangents()
{
	std::vector<glm::vec3> perFaceTangents;
	perFaceTangents.resize(numVertices, glm::vec3(0, 0, 0));

	std::vector<float> voronoiArea;
	voronoiArea.resize(numVertices, 0.0f);

	for (unsigned int i = 0; i < numFaces; i++)
	{
		const int start = i * 3;

		// Vertex indexes
		unsigned int raw_a = faces[start];
		unsigned int raw_b = faces[start + 1];
		unsigned int raw_c = faces[start + 2];

		// Vertex indexes within our array
		unsigned int a = raw_a * 3;
		unsigned int b = raw_b * 3;
		unsigned int c = raw_c * 3;

		glm::vec3 A(vertices[a], vertices[a + 1], vertices[a + 2]);
		glm::vec3 B(vertices[b], vertices[b + 1], vertices[b + 2]);
		glm::vec3 C(vertices[c], vertices[c + 1], vertices[c + 2]);

		int aStart = raw_a * 2;
		int bStart = raw_b * 2;
		int cStart = raw_c * 2;

		glm::vec2 st1A(uvs[bStart] - uvs[aStart], uvs[bStart + 1] - uvs[aStart + 1]);
		glm::vec2 st2A(uvs[cStart] - uvs[aStart], uvs[cStart + 1] - uvs[aStart + 1]);

		glm::vec2 st1B(uvs[aStart] - uvs[bStart], uvs[aStart + 1] - uvs[bStart + 1]);
		glm::vec2 st2B(uvs[cStart] - uvs[bStart], uvs[cStart + 1] - uvs[bStart + 1]);

		glm::vec2 st1C(uvs[aStart] - uvs[cStart], uvs[aStart + 1] - uvs[cStart + 1]);
		glm::vec2 st2C(uvs[bStart] - uvs[cStart], uvs[bStart + 1] - uvs[cStart + 1]);

		glm::vec3 tangentA = glm::normalize(tangent(st1A, st2A, B - A, C - A));
		glm::vec3 tangentB = glm::normalize(tangent(st1B, st2B, A - B, C - B));
		glm::vec3 tangentC = glm::normalize(tangent(st1C, st2C, A - C, B - C));

		glm::vec3 faceAreas = voronoiTriangleAreas(A, B, C);

		perFaceTangents[raw_a] += tangentA * faceAreas.x;
		perFaceTangents[raw_b] += tangentB * faceAreas.y;
		perFaceTangents[raw_c] += tangentC * faceAreas.z;

		voronoiArea[raw_a] += faceAreas.x;
		voronoiArea[raw_b] += faceAreas.y;
		voronoiArea[raw_c] += faceAreas.z;
	}

	tangents = new float[numVertices * 3];
	for (unsigned int i = 0; i < numVertices; i++)
	{
		perFaceTangents[i] /= voronoiArea[i];

		glm::vec3 normal = glm::normalize(perFaceTangents[i]);

		const int start = i * 3;
		tangents[start] = normal.x;
		tangents[start + 1] = normal.y;
		tangents[start + 2] = normal.z;
	}
}

unsigned int Engine::Mesh::getNumFaces()
{
	return numFaces;
}

unsigned int Engine::Mesh::getNumVerticesPerFace()
{
	return verticesPerFace;
}

unsigned int Engine::Mesh::getNumVertices()
{
	return numVertices;
}

const unsigned int * Engine::Mesh::getFaces() const
{
	return faces;
}

const float * Engine::Mesh::getVertices() const
{
	return vertices;
}

const float * Engine::Mesh::getColor() const
{
	return colors;
}

const float * Engine::Mesh::getNormals() const
{
	return normals;
}

const float * Engine::Mesh::getUVs() const
{
	return uvs;
}

const float * Engine::Mesh::getTangetns() const
{
	return tangents;
}

void Engine::Mesh::syncGPU()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int numFaces = getNumFaces();
	unsigned int numVertex = getNumVertices();

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, getVertices(), GL_STATIC_DRAW);

	if (colors != 0)
	{
		glGenBuffers(1, &vboColors);
		glBindBuffer(GL_ARRAY_BUFFER, vboColors);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, colors, GL_STATIC_DRAW);
	}

	if (normals != 0)
	{
		glGenBuffers(1, &vboNormals);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, normals, GL_STATIC_DRAW);
	}

	if (uvs != 0)
	{
		glGenBuffers(1, &vboUVs);
		glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 2, uvs, GL_STATIC_DRAW);
	}

	if (tangents != 0)
	{
		glGenBuffers(1, &vboTangents);
		glBindBuffer(GL_ARRAY_BUFFER, vboTangents);
		glBufferData(GL_ARRAY_BUFFER, numVertex * sizeof(float) * 3, tangents, GL_STATIC_DRAW);
	}

	if (faces != 0)
	{
		glGenBuffers(1, &vboFaces);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboFaces);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * sizeof(unsigned int) * 3, faces, GL_STATIC_DRAW);
	}
}

void Engine::Mesh::releaseCPU()
{
	if (faces != 0)
	{
		delete[] faces;
	}
	faces = 0;

	if (vertices != 0)
	{
		delete[] vertices;
	}
	vertices = 0;

	if (normals != 0)
	{
		delete[] normals;
	}
	normals = 0;

	if (uvs != 0)
	{
		delete[] uvs;
	}
	uvs = 0;

	if (tangents != 0)
	{
		delete[] tangents;
	}
	tangents = 0;

	if (colors != 0)
	{
		delete[] colors;
	}
	colors = 0;
}

void Engine::Mesh::releaseGPU()
{
	if (vboVertices != -1)
	{
		glDeleteBuffers(1, &vboVertices);
	}

	if (vboNormals != -1)
	{
		glDeleteBuffers(1, &vboNormals);
	}

	if (vboColors != -1)
	{
		glDeleteBuffers(1, &vboColors);
	}

	if (vboUVs != -1)
	{
		glDeleteBuffers(1, &vboUVs);
	}

	if (vboTangents != -1)
	{
		glDeleteBuffers(1, &vboTangents);
	}

	if (vboFaces != -1)
	{
		glDeleteBuffers(1, &vboFaces);
	}

	if (vao != -1)
	{
		glDeleteVertexArrays(1, &vao);
	}
}