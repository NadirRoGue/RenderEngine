#include "vegetation/FractalTree.h"

#include "datatables/MeshTable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

Engine::FractalTree::FractalTree(const TreeGenerationData & data) :Engine::ProceduralVegetation(data)
{
	// Get base shape to build the trees
	base = Engine::MeshTable::getInstance().getMesh("trunk");
	leaf = Engine::MeshTable::getInstance().getMesh("leaf");

	// Initialize random generator engine
	randEngine = std::default_random_engine(treeData.seed);
	randGen = std::uniform_real_distribution<float>(0.0f, 1.0f);

	// For ease config, data is setted on degrees, but we need radians
	treeData.minBranchRotation.x = glm::radians(treeData.minBranchRotation.x);
	treeData.minBranchRotation.y = glm::radians(treeData.minBranchRotation.y);
	treeData.minBranchRotation.z = glm::radians(treeData.minBranchRotation.z);
	treeData.maxBranchRotation.x = glm::radians(treeData.maxBranchRotation.x);
	treeData.maxBranchRotation.y = glm::radians(treeData.maxBranchRotation.y);
	treeData.maxBranchRotation.z = glm::radians(treeData.maxBranchRotation.z);
}

Engine::Mesh * Engine::FractalTree::generate()
{
	// Initial data to start growin the tree
	glm::vec3 scale = glm::vec3(0.04f, 0.2f, 0.04f) * treeData.scalingFactor;
	glm::vec3 translation(0, 0, 0);
	glm::vec3 rotation;

	if (treeData.rotateMainTrunk)
	{
		glm::vec3 & min = treeData.minBranchRotation;
		glm::vec3 & max = treeData.maxBranchRotation;
		rotation.x = randInInterval(min.x, max.x);
		rotation.y = randInInterval(min.y, max.y);
		rotation.z = randInInterval(min.z, max.z);
	}
	else
	{
		rotation = glm::vec3(0, 0, 0);
	}
	
	processChunk(glm::mat4(1.0f), scale, translation, rotation, 0, 1);

	// Copy generated data to the Engine::Mesh class format
	float * newVertices = new float[vertices.size() * 3];
	float * newColors = new float[vertices.size() * 3];
	float * newEmission = new float[vertices.size() * 3];
	float * newUVs = new float[vertices.size() * 2];
	for (size_t i = 0; i < vertices.size(); i++)
	{
		size_t index = i * 3;
		// Copy vertex pos
		glm::vec3 & v = vertices[i];
		newVertices[index] = v.x;
		newVertices[index + 1] = v.y;
		newVertices[index + 2] = v.z;
		// Copy vertex color
		glm::vec3 & c = colors[i];
		newColors[index] = c.x;
		newColors[index + 1] = c.y;
		newColors[index + 2] = c.z;
		// Copy vertex emission
		glm::vec3 & e = emission[i];
		newEmission[index] = e.x;
		newEmission[index + 1] = e.y;
		newEmission[index + 2] = e.z;

		size_t uvIndex = i * 2;
		glm::vec2 & uv = uvs[i];
		newUVs[uvIndex] = uv.x;
		newUVs[uvIndex + 1] = uv.y;
	}

	unsigned int * newFaces = new unsigned int[faces.size() * 3];
	for (size_t i = 0; i < faces.size(); i++)
	{
		size_t index = i * 3;
		glm::ivec3 & f = faces[i];
		newFaces[index] = f.x;
		newFaces[index + 1] = f.y;
		newFaces[index + 2] = f.z;
	}

	// Generate new mesh. Normals are automatically computed if not present in the constructor
	Engine::Mesh * tree = new Engine::Mesh(unsigned int(faces.size()), unsigned int(vertices.size()), newFaces, newVertices, newColors, 0, newUVs, 0, newEmission);

	return tree;
}

void Engine::FractalTree::processChunk(glm::mat4 & origin, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotation, size_t vOffset, unsigned int depth)
{
	if (depth >= treeData.depthStartingLeaf)
	{
		addLeaf(origin, scale / treeData.scalingFactor, vOffset, depth);
		if(depth >= treeData.maxDepth)
			return;
	}

	// Generate local branch model matrix
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), translate);
	glm::quat q(rotation);
	glm::mat4 rotateMat = glm::mat4_cast(q);

	// Compute global branch model matrix
	glm::mat4 modelMat = origin * translateMat * rotateMat;

	// Add vertices and faces applying the transformation
	appendVerticesAndFaces(base, modelMat, scale, depth, vOffset, depth==1, false);

	// Generate common translation to prevent branches from growing inside parent branches
	translate.y = scale.y;

	// Check for branching
	unsigned int intBranches = 1;
	if (depth >= treeData.startBranchingDepth)
	{
		float branches = randGen(randEngine) * float(treeData.maxBranchesSplit);
		intBranches = unsigned int(ceil(branches)); // ceil ensures there will be at least 1 branch
	}

	size_t currentOffset = vertices.size();

	float xRotation = randInInterval(treeData.minBranchRotation.x, treeData.maxBranchRotation.x);;// *(depth + 1);

	float deltaAngle = (2.f * 3.1415f) / intBranches;
	float sign = randSign();
	// Apply branching
	for (unsigned int i = 0; i < intBranches; i++)
	{
		// Copy and adjust next branch random data
		glm::vec3 scaleCopy = scale;
		scaleCopy *= treeData.scalingFactor;
		//scaleCopy.y *= 1.1f;

		glm::vec3 rotationCopy = rotation;
		rotationCopy.x = xRotation * sign;
		rotationCopy.y = deltaAngle * (i + 1);
		rotationCopy.z = 0.0f;

		// Next branch
		processChunk(modelMat, scaleCopy, translate, rotationCopy, currentOffset, depth + 1);
	}
}

void Engine::FractalTree::addLeaf(glm::mat4 & origin, glm::vec3 lastScaling, size_t offset, unsigned int depth)
{
	float maxScale = glm::max(glm::max(lastScaling.x, lastScaling.y), lastScaling.z);

	glm::vec3 translation(0, maxScale, 0);
	glm::mat4 model = origin * glm::translate(glm::mat4(1.0f), translation);

	//0.4 0.3 0.4
	appendVerticesAndFaces(base, model, glm::vec3(maxScale) * glm::vec3(0.4, 0.3, 0.4), 0, offset, true, true);
}

void Engine::FractalTree::appendVerticesAndFaces(Engine::Mesh * source, glm::mat4 & model, glm::vec3 scale, unsigned int depth, size_t vOffset, bool keepBase, bool isLeaf)
{
	// Add faces adding the offset of vertices already added to the main tree
	const unsigned int * fac = source->getFaces();
	size_t realOffset = vertices.size();
	for (unsigned int i = 0; i < source->getNumFaces(); i++)
	{
		unsigned int index = i * 3;

		size_t a = fac[index];
		size_t b = fac[index + 1];
		size_t c = fac[index + 2];

		if (!keepBase)
		{
			a = a < 4 ? a + vOffset - 4 : a + realOffset - 4;
			b = b < 4 ? b + vOffset - 4 : b + realOffset - 4;
			c = c < 4 ? c + vOffset - 4 : c + realOffset - 4;
		}
		else
		{
			a += realOffset;
			b += realOffset;
			c += realOffset;
		}

		glm::ivec3 f(a, b, c);
		faces.push_back(f);
	}

	// Add new vertices applying the transformations
	const float * verts = source->getVertices();
	unsigned int start = keepBase ? 0 : source->getNumVertices() / 2;
	float highestY = -99999.9f, lowestY = 99999.9f;
	for (unsigned int i = start; i < source->getNumVertices(); i++)
	{
		unsigned int index = (i * 3) + 1;

		if (verts[index] > highestY)
			highestY = verts[index];
		if (verts[index] < lowestY)
			lowestY = verts[index];
	}
	float delta = highestY - lowestY;

	for (unsigned int i = start; i < source->getNumVertices(); i++)
	{
		unsigned int index = i * 3;
		float x = verts[index] * scale.x;
		float y = verts[index + 1] * scale.y; 
		float z = verts[index + 2] * scale.z;
		glm::vec4 v(x, y, z, 1.0);

		unsigned int uvIndex = i * 2;
		float s = source->getUVs()[uvIndex] +  - 4;
		float t = source->getUVs()[uvIndex + 1] + vOffset - 4;
		glm::vec2 uv(s, t);
		uvs.push_back(uv);

		glm::vec4 transformedV = model * v;

		vertices.push_back(glm::vec3(transformedV.x, transformedV.y, transformedV.z));

		if (!isLeaf)
		{
			// Add color gradient based on depth + vertex height
			colors.push_back(glm::mix(treeData.startTrunkColor, treeData.endTrunkColor, depth * 2.0f + y));
			emission.push_back(glm::vec3(0, 0, 0));
		}
		else
		{
			glm::vec3 color = glm::mix(treeData.leafStartColor, treeData.leafEndColor, verts[index + 1] / delta);
			// Add leaf color and possible emission
			glm::vec3 data;
			data.x = 1.0f;
			data.y = treeData.emissiveLeaf ? 1.0f : 0.0f;
			emission.push_back(data);
			colors.push_back(color);
		}
	}
}

float Engine::FractalTree::randSign()
{
	float v = randGen(randEngine);
	if (v < 0.5)
		return -1.0f;
	else
		return 1.0f;
}

float Engine::FractalTree::randInInterval(float a, float b)
{
	return a + (b - a) * randGen(randEngine);
}
