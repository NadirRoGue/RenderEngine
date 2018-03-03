/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MeshInstance.h"

using namespace Engine;

MeshInstance::MeshInstance(Mesh * m, std::string materialName)
	:mesh(m)
{
	material = std::string(materialName);
}

MeshInstance::~MeshInstance()
{
}

std::string & MeshInstance::getMaterial()
{
	return material;
}

Mesh * MeshInstance::getMesh()
{
	return mesh;
}