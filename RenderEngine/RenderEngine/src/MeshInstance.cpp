/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MeshInstance.h"
#include <gl\glew.h>

Engine::MeshInstance::MeshInstance(Engine::Mesh * m, std::string materialName)
	:mesh(m)
{
	material = std::string(materialName);
}

Engine::MeshInstance::~MeshInstance()
{
}

std::string & Engine::MeshInstance::getMaterial()
{
	return material;
}

Engine::Mesh * Engine::MeshInstance::getMesh()
{
	return mesh;
}