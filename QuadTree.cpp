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
	std::vector<Model*> modelsToDraw;
	glm::vec3 leftNormal = glm::normalize(glm::cross(frustum.left, vec3(0, 1, 0)));
	glm::vec3 rightNormal = glm::normalize(glm::cross(vec3(0, 1, 0), frustum.right));
	root->checkInFrustum2(modelsToDraw, lookat, leftNormal, rightNormal, frustum.position);
	return modelsToDraw;
}

void QuadTree::buildTree(const std::vector<Model*> model) {
	root->treeBuilding(model, 4);
}