#include "QuadTree.h"

QuadTree::QuadTree() {
	root = nullptr;
}

QuadTree::QuadTree(glm::vec3 startPos, float sizeOfTree) {
	root = new Branch(startPos, sizeOfTree, 0);
}

QuadTree::~QuadTree() {	
	delete root;
}

std::vector<Model*> QuadTree::checkIntersection(const FrustumPoints &frustum, const glm::vec3 &lookat)
{
	/*std::vector<Model*> modelsToDraw;
	FrustumPoints frust = frustum;
	frust.position.y = 0;
	vec3 v2 = glm::normalize(frust.right - frust.position);
	vec3 v1 = glm::normalize(frust.left - frust.position);
	float angle = acosf(glm::dot(v1, v2));
	float angle2 = acosf(glm::dot(-v2, frust.fart));
	root->checkInFrustum(v1, v2, frust.fart, frust.position, angle, angle2, modelsToDraw);
	return modelsToDraw;*/
	std::vector<Model*> modelsToDraw;
	glm::vec3 leftNormal = glm::normalize(glm::cross(frustum.left, vec3(0, 1, 0)));
	glm::vec3 rightNormal = glm::normalize(glm::cross(vec3(0, 1, 0), frustum.right));
	root->checkInFrustum2(modelsToDraw, lookat, leftNormal, rightNormal, frustum.position);
	return modelsToDraw;
}

void QuadTree::buildTree(const std::vector<Model*> model) {
	root->treeBuilding(model, 4);
}