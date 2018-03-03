#ifndef __BOXFILE__
#define __BOXFILE__

//Describimos un cubo
//*******************
//Se replican vértices para que cada cara 
//tenga una normal distinta.

//Número de vértices
const int cubeNVertex = 24; // 4 vértices x 6 caras
const int cubeNTriangleIndex = 12; // 2 triángulos por cara x  6 cara;


const unsigned int cubeTriangleIndex[] = {
	//Cara z = 1
	0,1,2,			1,3,2,
	//Cara z = -1
	4,6,5,			5,6,7,
	//Cara x = 1
	8,10,9,			9,10,11,
	//Cara x = -1
	12,13,14,		13,15,14,
	//Cara y = 1
	16,17,18,		17,19,18,
	//Cara y = -1
	20,22,21,		21,22,23,
};

//Posicíon de los vertices
const float cubeVertexPos[] = { 
	//Cara z = 1
	-1.0f,	-1.0f,	 1.0f, //0
	 1.0f,	-1.0f,	 1.0f, //1
	-1.0f,	 1.0f,	 1.0f, //2
	 1.0f,	 1.0f,	 1.0f, //3

	//Cara z = -1		   
	-1.0f,	-1.0f,	-1.0f, //4
	 1.0f,	-1.0f,	-1.0f, //5
	-1.0f,	 1.0f,	-1.0f, //6
	 1.0f,	 1.0f,	-1.0f, //7

	//Cara x = 1		   
	1.0f,	-1.0f,	-1.0f, //8
	1.0f,	-1.0f,	 1.0f, //9
	1.0f,	 1.0f,	-1.0f, //10
	1.0f,	 1.0f,	 1.0f, //11

	//Cara x = -1		   
	-1.0f,	-1.0f,	-1.0f, //12
	-1.0f,	-1.0f,	 1.0f, //13
	-1.0f,	 1.0f,	-1.0f, //14
	-1.0f,	 1.0f,	 1.0f, //15

	//Cara y = 1		   
	-1.0f,	 1.0f,	-1.0f, //16
	-1.0f,	 1.0f,	 1.0f, //17
	 1.0f,	 1.0f,	-1.0f, //18
	 1.0f,	 1.0f,	 1.0f, //19

	//Cara y = -1		   
	-1.0f,	-1.0f,	-1.0f, //20
	-1.0f,	-1.0f,	 1.0f, //21
	 1.0f,	-1.0f,	-1.0f, //22
	 1.0f,	-1.0f,	 1.0f  //23
 };


//Normal de los vertices
const float cubeVertexNormal[] = { 
	//Cara z = 1
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 

	//Cara z = -1		   
	0.0f,	0.0f,	-1.0f, 
	0.0f,	0.0f,	-1.0f, 
	0.0f,	0.0f,	-1.0f, 
	0.0f,	0.0f,	-1.0f, 

	//Cara x = 1		   
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 

	//Cara x = -1		   
	-1.0f,	0.0f,	 0.0f, 
	-1.0f,	0.0f,	 0.0f, 
	-1.0f,	0.0f,	 0.0f, 
	-1.0f,	0.0f,	 0.0f, 

	//Cara y = 1		   
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 

	//Cara y = -1		   
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f, 
	0.0f,	-1.0f,	0.0f  
 };


//Color de los vertices
const float cubeVertexColor[] = { 
	//Cara z = 1
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 
	0.0f,	0.0f,	 1.0f, 

	//Cara z = -1		   
	0.6f,	0.6f,	 1.0f, 
	0.6f,	0.6f,	 1.0f, 
	0.6f,	0.6f,	 1.0f, 
	0.6f,	0.6f,	 1.0f, 

	//Cara x = 1		   
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 
	1.0f,	0.0f,	 0.0f, 

	//Cara x = -1		   
	1.0f,	0.6f,	 0.6f, 
	1.0f,	0.6f,	 0.6f, 
	1.0f,	0.6f,	 0.6f, 
	1.0f,	0.6f,	 0.6f, 

	//Cara y = 1		   
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 
	0.0f,	1.0f,	0.0f, 

	//Cara y = -1		   
	0.6f,	1.0f,	0.6f, 
	0.6f,	1.0f,	0.6f, 
	0.6f,	1.0f,	0.6f, 
	0.6f,	1.0f,	0.6f  
 };

//Cood. de textura de los vertices
const float cubeVertexTexCoord[] = { 
	//Cara z = 1
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 1.0f,

	//Cara z = -1
	0.0f, 1.0f, 
	1.0f, 1.0f, 
	0.0f, 0.0f, 
	1.0f, 0.0f, 

	//Cara x = 1	
	0.0f,	1.0f,
	1.0f,	1.0f,
	0.0f,	0.0f,
	1.0f,	0.0f,

	//Cara x = -1
	0.0f,	0.0f,
	1.0f,	0.0f,
	0.0f,	1.0f,
	1.0f,	1.0f,

	//Cara y = 1	
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	//Cara y = -1
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
 };


const float cubeVertexTangent[] = { 
	//Cara z = 1
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,

	//Cara z = -1				
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		
	1.0f,	0.0f,	0.0f,		

	//Cara x = 1				
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		
	0.0f,	0.0f,	-1.0f,		

	//Cara x = -1				
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		
	0.0f,	0.0f,	1.0f,		

	//Cara y = 1	   
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
					
	//Cara y = -1					   
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
 };

#endif


