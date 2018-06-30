/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Mesh.h"

namespace Engine
{
	/**
	 * Holds the data to build different type of planes
	 * - Vertical plane used for deferred rendering
	 * - Half vertical plane (left and right) used for side-by-side rendering
	 */
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

	// Creates a full screen quad for deferred rendering
	Mesh CreatePlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::planeVertexPos, 0, 0, PlaneData::planeUVs, 0);
	}

	// Creates a left half screen quad for side-by-side rendering
	Mesh CreateLeftPlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::leftSmallPlaneVertex, 0, 0, PlaneData::planeUVs, 0);
	}

	// Creates a right half screen quad for side-by-side rendering
	Mesh CreateRightPlane()
	{
		return Mesh(0, PlaneData::planeNVertex, 0, PlaneData::rightSmallPlaneVertex, 0, 0, PlaneData::planeUVs, 0);
	}
}