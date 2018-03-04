#include "Material.h"

#include <iostream>

Engine::Material::AbstractMaterial::AbstractMaterial()
{

}

Engine::Material::AbstractMaterial::~AbstractMaterial()
{

}

const std::string & Engine::Material::AbstractMaterial::getMaterialName() const
{
	return name;
}

unsigned long Engine::Material::AbstractMaterial::getFlags() const
{
	return flags;
}

// ============================================================

glm::vec3 Engine::Material::StandardMaterial::DEFAULT_COLOR = glm::vec3(0.6, 0.6, 0.6);

Engine::Material::StandardMaterial::StandardMaterial()
	:Engine::Material::AbstractMaterial()
{
	name = "StandardMaterial";
	albedo = normalMap = specularMap = occlusionMap = displacementMap = NULL;
	unlit = autoLOD = false;
	specularCoefficent = 0.0f;
}

Engine::Material::StandardMaterial::~StandardMaterial()
{
	if (albedo != NULL)
	{
		delete albedo;
	}

	if (normalMap != NULL)
	{
		delete normalMap;
	}

	if (specularMap != NULL)
	{
		delete specularMap;
	}

	if (occlusionMap != NULL)
	{
		delete occlusionMap;
	}

	if (displacementMap != NULL)
	{
		delete displacementMap;
	}
}

void Engine::Material::StandardMaterial::setAlbedo(Engine::TextureInstance * albedo)
{
	if (this->albedo != NULL)
	{
		delete this->albedo;
	}

	this->albedo = albedo;
}

void Engine::Material::StandardMaterial::setNormalMap(Engine::TextureInstance * normalMap)
{
	if (this->normalMap != NULL)
	{
		delete this->normalMap;
	}

	this->normalMap = normalMap;
}

void Engine::Material::StandardMaterial::setSpecularMap(Engine::TextureInstance * specularMap)
{
	if (this->specularMap != NULL)
	{
		delete this->specularMap;
	}

	this->specularMap = specularMap;
}

void Engine::Material::StandardMaterial::setOcclusionMap(Engine::TextureInstance * occlusionMap)
{
	if (this->occlusionMap != NULL)
	{
		delete this->occlusionMap;
	}

	this->occlusionMap = occlusionMap;
}

void Engine::Material::StandardMaterial::setDisplacementMap(Engine::TextureInstance * displacementMap)
{
	if (this->displacementMap != NULL)
	{
		delete this->displacementMap;
	}

	this->displacementMap = displacementMap;
}

void Engine::Material::StandardMaterial::setSpecularCoefficent(float coefficent)
{
	coefficent = coefficent < 0.0f ? 0.0f : coefficent;
	this->specularCoefficent = coefficent;
}

void Engine::Material::StandardMaterial::setUnlit(bool unlit)
{
	this->unlit = unlit;
}

void Engine::Material::StandardMaterial::setAutoLOD(bool autolod)
{
	this->autoLOD = autolod;
}

Engine::Program * Engine::Material::StandardMaterial::compile()
{
	return NULL;
}

void Engine::Material::StandardMaterial::populateProgram(Engine::Program * prog)
{
	Engine::StandarProgram * standardProg = dynamic_cast<Engine::StandarProgram*>(prog);
	if (standardProg == NULL)
	{
		return;
	}

}

// ======================================================

Engine::Material::MaterialTable * Engine::Material::MaterialTable::INSTANCE = new Engine::Material::MaterialTable();

Engine::Material::MaterialTable & Engine::Material::MaterialTable::getInstance()
{
	return *Engine::Material::MaterialTable::INSTANCE;
}

Engine::Material::MaterialTable::MaterialTable()
{
}

Engine::Material::MaterialTable::~MaterialTable()
{
	std::map< std::string, std::map<unsigned long, Program*>>::iterator tableIt = table.begin();
	while (tableIt != table.end())
	{
		std::cout << "MaterialTable: Cleaning " << tableIt->first << std::endl;
		
		std::map<unsigned long, Program*>::iterator subTableIt = tableIt->second.begin();

		while (subTableIt != tableIt->second.end())
		{
			std::cout << "\tClearing " << subTableIt->first << std::endl;
			delete subTableIt->second;
		}
	}
}

void Engine::Material::MaterialTable::addMaterial(AbstractMaterial * material)
{
	if (material == NULL)
	{
		return;
	}

	std::string name = material->getMaterialName();
	std::map< std::string, std::map<unsigned long, Program*>>::iterator tableIt = table.find(name);

	unsigned long flags = material->getFlags();
	std::map<unsigned long, Program *>::iterator subTableIt;

	if (tableIt == table.end())
	{
		Program * prog = material->compile();
		if (prog != NULL)
		{
			std::map<unsigned long, Program *> subTable;
			subTable[flags] = prog;
			table[name] = subTable;
		}
		else
		{
			std::cout << "MaterialTable: Failed to compile " << name << " with flags " << flags << std::endl;
		}
	}
	else if ((subTableIt = tableIt->second.find(flags)) == tableIt->second.end())
	{
		Program * prog = material->compile();
		if (prog != NULL)
		{
			tableIt->second[flags] = prog;
		}
		else
		{
			std::cout << "MaterialTable: Failed to compile " << name << " with flags " << flags << std::endl;
		}
	}
}

Engine::Program * Engine::Material::MaterialTable::getProgramFromMaterial(AbstractMaterial * mat)
{
	std::map<std::string, std::map<unsigned long, Program*>>::iterator tableIt = table.find(mat->getMaterialName());
	if (tableIt != table.end())
	{
		std::map<unsigned long, Program *>::iterator subTableIt;
		if ((subTableIt = tableIt->second.find(mat->getFlags())) != tableIt->second.end())
		{
			return subTableIt->second;
		}
	}

	return NULL;
}