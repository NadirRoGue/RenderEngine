/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <assimp\scene.h>

namespace Engine
{
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
		float *uvs;
		float *tangents;
	public:
		unsigned int vao;
		unsigned int vboFaces;
		unsigned int vboVertices;
		unsigned int vboNormals;
		unsigned int vboColors;
		unsigned int vboUVs;
		unsigned int vboTangents;

	public:
		Mesh();
		Mesh(aiMesh * mesh);
		Mesh(const unsigned int numF, const unsigned int numV, const unsigned int *f, const float *v, const float *c, const float *n, const float *uv, const float *t);
		Mesh(const Mesh &other);
		~Mesh();

		void loadFromMesh(aiMesh * mesh);

		unsigned int getNumFaces();
		unsigned int getNumVertices();
		unsigned int getNumVerticesPerFace();

		const unsigned int * getFaces() const;
		const float * getVertices() const;
		const float * getNormals() const;
		const float * getColor() const;
		const float * getUVs() const;
		const float * getTangetns() const;

		void releaseGPU();
		void releaseCPU();
	private:
		void extractTopology(aiMesh * mesh);
		void extractGeometry(aiMesh * mesh);

		void computeNormals();
		void computeTangents();

		void syncGPU();
	};
}