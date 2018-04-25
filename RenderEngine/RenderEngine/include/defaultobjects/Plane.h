#pragma once

#include "Mesh.h"

namespace Engine
{
	namespace PlaneData
	{
		const unsigned int planeNVertex = 4;

		// Post process - or any purpose - plane
		const float planeVertexPos[] = {
			-1.0f,	-1.0f,	 0.0f,
			1.0f,	-1.0f,	 0.0f,
			-1.0f,	 1.0f,	 0.0f,
			1.0f,	 1.0f,	 0.0f,
		};

		const float planeUVs[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f
		};

		// side-by-side render left pane
		const float leftSmallPlaneVertex[] = {
			-1.0f, -1.0f, 0.0f,
			0.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};

		// side-by-side render right plane
		const float rightSmallPlaneVertex[] = {
			0.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f
		};
	}

	Mesh CreatePlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::planeVertexPos, 0, 0, PlaneData::planeUVs, 0);
	}

	Mesh CreateLeftPlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::leftSmallPlaneVertex, 0, 0, PlaneData::planeUVs, 0);
	}

	Mesh CreateRightPlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::rightSmallPlaneVertex, 0, 0, PlaneData::planeUVs, 0);
	}
}