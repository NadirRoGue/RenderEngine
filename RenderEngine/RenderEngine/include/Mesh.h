/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <assimp\scene.h>

namespace Engine
{
	// Represents a triangle mesh
	// Is also in charge of syncing and releasing CPU and GPU resources
	// related to the mesh
	class Mesh
	{
	private:
		unsigned int numFaces;
		unsigned int numVertices;
		unsigned int verticesPerFace;

		unsigned int *faces;
		float *vertices;
		float *normals;
		float *colors;
		float *emission;
		float *uvs;
		float *tangents;
	public:
		unsigned int vao;
		unsigned int vboFaces;
		unsigned int vboVertices;
		unsigned int vboNormals;
		unsigned int vboColors;
		unsigned int vboEmission;
		unsigned int vboUVs;
		unsigned int vboTangents;

	public:
		Mesh();
		Mesh(aiMesh * mesh);
		Mesh(const unsigned int numF, const unsigned int numV, const unsigned int *f, const float *v, const float *c, const float *n, const float *uv, const float *t, const float *e = 0);
		Mesh(const Mesh &other);
		~Mesh();

		void loadFromMesh(aiMesh * mesh);

		const unsigned int getNumFaces() const;
		const unsigned int getNumVertices() const;
		const unsigned int getNumVerticesPerFace() const;

		const unsigned int * getFaces() const;
		const float * getVertices() const;
		const float * getNormals() const;
		const float * getColor() const;
		const float * getUVs() const;
		const float * getTangetns() const;
		const float * getEmissive() const;

		void computeNormals();
		void computeTangents();

		void syncGPU();

		void releaseGPU();
		void releaseCPU();

		void use() const;
	private:
		void extractTopology(aiMesh * mesh);
		void extractGeometry(aiMesh * mesh);
	};
}