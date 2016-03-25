#pragma once
#include <glm\glm.hpp>
#include <iostream>
#include <vector>
#include "Model.h"
#include "Datastructs.h"

class QuadTree {
public:
	QuadTree();
	QuadTree(glm::vec3 startPos, float sizeOfTree);
	~QuadTree();

	void checkIntersection(const FrustumPoints &frustum);
	void buildTree(const std::vector<Model*> model);
private:
	
	class Branch {
	public:
		Branch(glm::vec3 startPoint, float distance, int id) {
			points[0] = startPoint;
			points[1] += startPoint + glm::vec3(distance, 0, -distance);
			levelInTree = 999;
			id = id;
			for (int i = 0; i < 4; i++)
				child[i] = nullptr;			
		}

		~Branch() {
			for (int i = 0; i < 4; i++)
			{
				if (child[i] != nullptr)
					delete child[i];
			}
		}
		
		bool checkIntersect(const Model* model) {
			glm::vec3* modelBox = model->getBBox();
			for (int i = 0; i < 4; i++)
			{			
				if (modelBox[i].x > points[0].x && modelBox[i].x < points[1].x && modelBox[i].z > points[1].z && modelBox[i].z < points[0].z)
					return true;
			}
			return false;
		}

		void treeBuilding(std::vector<Model*> modelVector, int levelOfTree) {
			bool willDivide = false;
			if (levelOfTree > 0) {
				levelOfTree--;
				for (int i = 0; i < modelVector.size(); i++)
				{
					if (checkIntersect(modelVector[i]))
					{
						willDivide = true;
						if (levelOfTree == 1)
						{
							modelNumberVec.push_back(modelVector[i]->getBuffID());
							std::cout << modelVector[i]->getBuffID() << "  Modell i trad nr: " << id << std::endl;
						}
					}
				}
				if (willDivide && levelOfTree != 1)
				{
					levelInTree = levelOfTree;
					float distance = abs((points[0].x - points[1].x) / 2);
					child[0] = new Branch(glm::vec3(points[0].x, 0, points[0].z), distance, levelInTree + 1);
					child[1] = new Branch(glm::vec3(points[0].x + distance, 0, points[0].z), distance, levelInTree + 2);
					child[2] = new Branch(glm::vec3(points[0].x, 0, points[0].z - distance), distance, levelInTree + 3);
					child[3] = new Branch(glm::vec3(points[0].x + distance, 0, points[0].z - distance), distance, levelInTree + 4);
					for (int i = 0; i < 4; i++)
						child[i]->treeBuilding(modelVector, levelOfTree);
				}
				
			}
		}

		void checkInFrustum(const FrustumPoints &frustum) {		
			for (int i = 0; i < 2; i++)
			{
				if (points[0].x > frustum.fPoints[i].x  && points[1].x < frustum.fPoints[i+1].x  && points[1].z > frustum.fPoints[i].y && points[0].z < frustum.fPoints[i+1].y) {
					if (levelInTree == 0)
					{
						for (int j = 0; j < modelNumberVec.size(); j++);
						//add to models to be drawn
						return;
					}
					else {
						for (int x = 0; x < 4; x++) {
							child[i]->checkInFrustum(frustum);
						}
					}
				}
			}
		}

		Branch* child[4];
		glm::vec3 points[2];
		std::vector<GLuint> modelNumberVec;
		int levelInTree, id;
	};
	Branch* root;

};