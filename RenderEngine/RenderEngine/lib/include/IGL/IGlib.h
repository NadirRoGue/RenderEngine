#ifndef __IGLIB__
#define __IGLIB__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define IGLIB_API __declspec(dllexport)

namespace IGlib {

	//Funciones básicas
	IGLIB_API bool init(const char* vertexShaderFileName, const char* fragmentShaderFileName);
	IGLIB_API void mainLoop();
	IGLIB_API void destroy();
	IGLIB_API void setProjMat(const glm::mat4 &projMat);
	IGLIB_API void setViewMat(const glm::mat4 &viewMat);

	//Creación y manejo de objetos 3D
	//Devuelve el ID del objeto creado
	IGLIB_API int createObj(unsigned int nTriangles, unsigned int nVertex,
		const unsigned int *triangleIdx, const float *vertexPos,
		const float *vertexColor = NULL, const float *vertexNormal = NULL,
		const float *vertexTexCoord = NULL, const float*vertexTangent = NULL);

	IGLIB_API void setModelMat(int ID, const glm::mat4 &modelMat);

	IGLIB_API void addColorTex(int ID, const char *fileName);
	IGLIB_API void addEmissiveTex(int ID, const char *fileName);
	IGLIB_API void addNormalTex(int ID, const char *fileName);
	IGLIB_API void addSpecularTex(int ID, const char *fileName);
	IGLIB_API void addAuxiliarTex(int ID, const char *fileName);

	//Eventos
	//Punteros a funciones
	//Anchura y altura de la nueva ventana
	typedef void (*tResizeCB)(int width, int height);
	
	//Sin parametros
	typedef void(*tIdleCB)(void);
	
	//key: tecla pulsada
	//x, y: posición del raton (en pixeles)
	typedef void(*tKeyboardCB) (unsigned char key, int x, int y);

	//Button: left 0, middle 1, right 2
	//State: down 0, up 1
	typedef void(*tMouseCB) (int button, int state, int x, int y);

	//x, y: posición del raton (en pixeles)
	typedef void(*tMouseMotionCB) (int x, int y);

	IGLIB_API void setResizeCB(tResizeCB resizeCB);
	IGLIB_API void setIdleCB(tIdleCB idleCB);
	IGLIB_API void setKeyboardCB(tKeyboardCB keyboardCB);
	IGLIB_API void setMouseCB(tMouseCB mouseCB);
	//Detecta el movimiento si se ha pulsado un boton
	IGLIB_API void setMouseMoveCB(tMouseMotionCB mouseMotionCB);
}



#endif