#include "BOX.h"

#include <windows.h>

#include <fstream>

//Carga de texturas
#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

//////////////////////////////////////////
// Funciones auxiliares ya implementadas

//Funciones para la carga de los shaders
char *loadStringFromFile(const char *fileName, unsigned int &fileLen)
{
	//Se carga el fichero
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file) return 0;

	//Se calcula la longitud del fichero
	file.seekg(0, std::ios::end);
	fileLen = unsigned int(file.tellg());
	file.seekg(std::ios::beg);

	//Se lee el fichero
	char *source = new char[fileLen + 1];

	int i = 0;
	while (file.good())
	{
		source[i] = char(file.get());
		if (!file.eof()) i++;
		else fileLen = i;
	}
	source[fileLen] = '\0';
	file.close();

	return source;
}

unsigned char *loadTexture(const char* fileName, unsigned int &w, unsigned int &h)
{
	FreeImage_Initialise(TRUE);

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName, 0);
	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename(fileName);
	if ((format == FIF_UNKNOWN) || !FreeImage_FIFSupportsReading(format))
		return NULL;

	FIBITMAP* img = FreeImage_Load(format, fileName);
	if (img == NULL)
		return NULL;

	FIBITMAP* tempImg = img;
	img = FreeImage_ConvertTo32Bits(img);
	FreeImage_Unload(tempImg);

	w = FreeImage_GetWidth(img);
	h = FreeImage_GetHeight(img);

	//BGRA a RGBA
	unsigned char * map = new unsigned char[4 * w*h];
	char *buff = (char*)FreeImage_GetBits(img);

	for (unsigned int j = 0; j<w*h; j++){
		map[j * 4 + 0] = buff[j * 4 + 2];
		map[j * 4 + 1] = buff[j * 4 + 1];
		map[j * 4 + 2] = buff[j * 4 + 0];
		map[j * 4 + 3] = buff[j * 4 + 3];
	}

	FreeImage_Unload(img);
	FreeImage_DeInitialise();

	return map;
}