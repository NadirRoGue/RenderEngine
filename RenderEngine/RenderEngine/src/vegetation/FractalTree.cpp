#include "vegetation/FractalTree.h"

#include "datatables/MeshTable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

Engine::FractalTree::FractalTree(const TreeGenerationData & data) :Engine::ProceduralVegetation(data)
{
	// Get base shape to build the trees
	base = Engine::MeshTable::getInstance().getMesh("cube");

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
	glm::vec3 scale = glm::vec3(0.02f, 0.1f, 0.02f);
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
	
	processChunk(glm::mat4(1.0f), scale, translation, rotation, 1);

	// Copy generated data to the Engine::Mesh class format
	float * newVertices = new float[vertices.size() * 3];
	float * newColors = new float[vertices.size() * 3];
	float * newEmission = new float[vertices.size() * 3];
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
	Engine::Mesh * tree = new Engine::Mesh(unsigned int(faces.size()), unsigned int(vertices.size()), newFaces, newVertices, newColors, 0, 0, 0, newEmission);

	return tree;
}

void Engine::FractalTree::processChunk(glm::mat4 & origin, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotation, unsigned int depth)
{
	if (depth >= treeData.maxDepth)
	{
		addLeaf(origin, scale / treeData.scalingFactor);
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
	appendVerticesAndFaces(modelMat, scale, depth, false);

	// Generate common translation to prevent branches from growing inside parent branches
	translate.y = 2.0f * scale.y;

	// Check for branching
	unsigned int intBranches = 1;
	if (depth >= treeData.startBranchingDepth)
	{
		float branches = randGen(randEngine) * float(treeData.maxBranchesSplit);
		intBranches = unsigned int(ceil(branches)); // ceil ensures there will be at least 1 branch
	}
	
	// Apply branching
	for (unsigned int i = 0; i < intBranches; i++)
	{
		// Copy and adjust next branch random data
		glm::vec3 scaleCopy = scale;
		scaleCopy *= treeData.scalingFactor;

		glm::vec3 rotationCopy = rotation;
		rotationCopy.x += randInInterval(treeData.minBranchRotation.x, treeData.maxBranchRotation.x);
		rotationCopy.y += randInInterval(treeData.minBranchRotation.y, treeData.maxBranchRotation.y);
		rotationCopy.z += randInInterval(treeData.minBranchRotation.z, treeData.maxBranchRotation.z);

		// Next branch
		processChunk(modelMat, scaleCopy, translate, rotationCopy, depth + 1);
	}
}

void Engine::FractalTree::addLeaf(glm::mat4 & origin, glm::vec3 lastScaling)
{
	float minScale = lastScaling.x < lastScaling.y && lastScaling.x < lastScaling.z ? lastScaling.x : lastScaling.y < lastScaling.z ? lastScaling.y : lastScaling.z;
	float maxScale = minScale * 2.5f;
	float moveD = 2.0f;

	glm::vec3 top(0, moveD * lastScaling.y, 0);
	glm::mat4 topModel = origin * glm::translate(glm::mat4(1.0f), top); // lol!
	appendVerticesAndFaces(topModel, glm::vec3(minScale, maxScale, minScale), 0, true);

	glm::vec3 up(0, moveD * lastScaling.y, moveD * maxScale);
	glm::mat4 upModel = origin * glm::translate(glm::mat4(1.0f), up);
	appendVerticesAndFaces(upModel, glm::vec3(minScale, minScale, maxScale), 0, true);

	glm::vec3 down(0, moveD * lastScaling.y,-moveD * maxScale);
	glm::mat4 downModel = origin * glm::translate(glm::mat4(1.0f), down);
	appendVerticesAndFaces(downModel, glm::vec3(minScale, minScale, maxScale), 0, true);

	glm::vec3 left(-moveD * maxScale, moveD * lastScaling.y, 0);
	glm::mat4 leftModel = origin * glm::translate(glm::mat4(1.0f), left);
	appendVerticesAndFaces(leftModel, glm::vec3(maxScale, minScale, minScale), 0, true);

	glm::vec3 right(moveD * maxScale, moveD * lastScaling.y, 0);
	glm::mat4 rightModel = origin * glm::translate(glm::mat4(1.0f), right);
	appendVerticesAndFaces(rightModel, glm::vec3(maxScale, minScale, minScale), 0, true);
}

void Engine::FractalTree::appendVerticesAndFaces(glm::mat4 & model, glm::vec3 scale, unsigned int depth, bool isLeaf)
{
	// Add faces adding the offset of vertices already added to the main tree
	const unsigned int * fac = base->getFaces();
	size_t offset = vertices.size();
	for (unsigned int i = 0; i < base->getNumFaces(); i++)
	{
		unsigned int index = i * 3;
		glm::ivec3 f(fac[index] + offset, fac[index + 1] + offset, fac[index + 2] + offset);
		faces.push_back(f);
	}

	// Add new vertices applying the transformations
	const float * verts = base->getVertices();
	for (unsigned int i = 0; i < base->getNumVertices(); i++)
	{
		unsigned int index = i * 3;
		float x = verts[index] * scale.x;
		float y = (verts[index + 1] + 1.0f) * scale.y; // + 1.0f to fix the y coordinate (the original cube is defined between -1 and 1)
		float z = verts[index + 2] * scale.z;
		glm::vec4 v(x, y, z, 1.0);

		glm::vec4 transformedV = model * v;

		vertices.push_back(glm::vec3(transformedV.x, transformedV.y, transformedV.z));

		if (!isLeaf)
		{
			// Add color gradient based on depth + vertex height
			colors.push_back(lerpColor(treeData.startTrunkColor, treeData.endTrunkColor, depth * 2.0f + y));
			emission.push_back(glm::vec3(0, 0, 0));
		}
		else
		{
			// Add leaf color and possible emission
			if (treeData.emissiveLeaf)
			{
				emission.push_back(treeData.leafColor);
			}
			else
			{
				emission.push_back(glm::vec3(0, 0, 0));
			}
			colors.push_back(treeData.leafColor);
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

glm::vec3 Engine::FractalTree::lerpColor(const glm::vec3 & a, const glm::vec3 & b, float alpha)
{
	glm::vec3 result;
	result.x = a.x + (b.x - a.x) * alpha;
	result.y = a.y + (b.y - a.y) * alpha;
	result.z = a.z + (b.z - a.z) * alpha;
	return result;
}
