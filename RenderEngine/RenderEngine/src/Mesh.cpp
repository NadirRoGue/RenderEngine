/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "Mesh.h"

#include "CustomMaths.h"

#include <glm/glm.hpp>
#include <vector>

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
	if(faces != nullptr)
		delete[] faces;

	if(vertices != 0)
		delete[] vertices;

	if(colors != 0)
		delete[] colors;

	if(normals != 0)
		delete[] normals;

	if(uvs != 0)
		delete[] uvs;

	if(tangents != 0)
		delete[] tangents;
}

void Engine::Mesh::extractTopology(aiMesh * mesh)
{
	numFaces = mesh->mNumFaces;

	faces = new unsigned int[numFaces * 3];

	for (unsigned int i = 0; i < numFaces; i++)
	{
		const int start = i * 3;
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

unsigned int Engine::Mesh::getNumVertices()
{
	return numVertices;
}

unsigned int * Engine::Mesh::getFaces()
{
	return faces;
}

float * Engine::Mesh::getVertices()
{
	return vertices;
}

float * Engine::Mesh::getColor()
{
	return colors;
}

float * Engine::Mesh::getNormals()
{
	return normals;
}

float * Engine::Mesh::getUVs()
{
	return uvs;
}

float * Engine::Mesh::getTangetns()
{
	return tangents;
}