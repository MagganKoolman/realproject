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

void QuadTree::checkIntersection(const FrustumPoints &frustum)
{
	
}

void QuadTree::buildTree(const std::vector<Model*> model) {
	root->treeBuilding(model, 3);
}