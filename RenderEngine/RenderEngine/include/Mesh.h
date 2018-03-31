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

		unsigned int *faces;
		float *vertices;
		float *normals;
		float *colors;
		float *uvs;
		float *tangents;

	public:
		Mesh();
		Mesh(aiMesh * mesh);
		// BOX.h legacy constructor
		Mesh(const unsigned int numF, const unsigned int numV, const unsigned int *f, const float *v, const float *c, const float *n, const float *uv, const float *t);
		Mesh(const Mesh &other);
		~Mesh();

		void loadFromMesh(aiMesh * mesh);

		unsigned int getNumFaces();
		unsigned int getNumVertices();

		unsigned int * getFaces();
		float * getVertices();
		float * getNormals();
		float * getColor();
		float * getUVs();
		float * getTangetns();

	private:
		void extractTopology(aiMesh * mesh);
		void extractGeometry(aiMesh * mesh);
		void extractColors(aiMesh * mesh);

		void computeNormals();
		void computeTangents();
	};
}