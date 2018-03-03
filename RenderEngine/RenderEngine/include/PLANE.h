#ifndef __PLANEFILE__
#define __PLANEFILE__

//Describimos un plano en coordenadas normalizadas
//*************************************************

//Número de vértices
const int planeNVertex = 4; // 4 vértices 

//Posicíon de los vertices
const float planeVertexPos[] = { 
	//Cara z = 1
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

const float leftSmallPlaneVertex[] = {
	-1.0f, -1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

const float rightSmallPlaneVertex[] = {
	0.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

#endif