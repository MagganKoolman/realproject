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

	std::vector<Model*> checkIntersection(const FrustumPoints &frustum, const glm::vec3 &lookat);
	void buildTree(const std::vector<Model*> model);
private:
	class Branch {
	public:
		Branch(glm::vec3 startPoint, float distance, int id) {
			points[0] = startPoint;
			points[1] = startPoint + glm::vec3(distance, 0, 0);
			points[2] = startPoint + glm::vec3(0, 0, -distance);;
			points[3] = startPoint + glm::vec3(distance, 0, -distance);
			this->id = id;
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
			return !(modelBox[0].x > points[1].x ||
				modelBox[2].x < points[0].x ||
				modelBox[0].z > points[0].z||
				modelBox[3].z < points[2].z);
		}

		bool checkIntersect(const glm::vec3 &box) {
			return (box.x < points[1].x &&
				box.x > points[0].x &&
				box.z < points[0].z &&
				box.z > points[2].z);
		}

		void treeBuilding(std::vector<Model*> modelVector, int levelOfTree) {
			bool willDivide = false;
			if (levelOfTree >= 0) {
				for (int i = 0; i < modelVector.size(); i++)
				{
					if (checkIntersect(modelVector[i]))
					{
						willDivide = true;
						if (levelOfTree == 0)
						{
							modelPointVec.push_back(modelVector[i]);
							std::cout << modelVector[i]->getBuffID() << "  Modell i trad nr: " << id << std::endl;
						}
					}
				}
				if (willDivide && levelOfTree > 0)
				{
					levelOfTree;
					float distance = abs((points[0].x - points[1].x) / 2);
					child[0] = new Branch(glm::vec3(points[0].x, 0, points[0].z), distance, levelOfTree - 1);
					child[1] = new Branch(glm::vec3(points[0].x + distance, 0, points[0].z), distance, levelOfTree);
					child[2] = new Branch(glm::vec3(points[0].x, 0, points[0].z - distance), distance, levelOfTree +1);
					child[3] = new Branch(glm::vec3(points[0].x + distance, 0, points[0].z - distance), distance, levelOfTree +2);
					levelOfTree--;
					for (int i = 0; i < 4; i++)
						child[i]->treeBuilding(modelVector, levelOfTree);
				}
				
			}
		}

		void checkInFrustum(const glm::vec3 &v1, const glm::vec3 &v2, glm::vec3 &fart, const glm::vec3 &position, const float &angle, const float &angle2, std::vector<Model*> &modelsToDraw) {
			bool found = false;		
			vec3 la = glm::normalize(v1 + v2);
			for (int i = 0; i < 4 && !found; i++) {
				vec3 vTarget = glm::normalize(points[i] - position);
				vec3 vTarget2 = glm::normalize(points[i] - (position + v2));
				if (glm::length(vTarget) < 50.0 && glm::dot(la,vTarget) > 0) {
					if (!(angle > acosf(glm::dot(vTarget, v2)) && angle2 > acosf(glm::dot(vTarget2, fart))))
					{
						found = true;
						if (child[0] == nullptr)
						{
							for (int j = 0; j < modelPointVec.size(); j++)
							{
								modelsToDraw.push_back(modelPointVec[j]);
							}
						}
						else
							for (int x = 0; x < 4; x++)
								child[x]->checkInFrustum(v1, v2, fart, position, angle, angle2, modelsToDraw);
					}
				}
			}
		}

		void checkInFrustum2(std::vector<Model*> &modelsToDraw, const glm::vec3 &nearNormal, const glm::vec3 &leftNormal, const glm::vec3 &rightNormal, const glm::vec3 &position) {
			bool found = false;
			for (int i = 0; i < 4 && !found; i++) {
				float distanceToPoint = glm::length(points[i] - position);
				vec3 vTarget = glm::normalize(points[i] - position);
				if (glm::dot(vTarget, nearNormal) > 0) {
					if (glm::dot(vTarget, leftNormal) >= 0.0f && glm::dot(vTarget, rightNormal) >= 0.0f || checkIntersect(position)) {
						found = true;
						if (child[0] == nullptr)
						{
							for (int j = 0; j < modelPointVec.size(); j++)
							{
								modelsToDraw.push_back(modelPointVec[j]);
							}
						}
						else
							for (int x = 0; x < 4; x++)
								child[x]->checkInFrustum2(modelsToDraw, nearNormal, leftNormal, rightNormal, position);
					}
				}
			}
		}
		Branch* child[4];
		glm::vec3 points[4];
		std::vector<Model*> modelPointVec;
		int id;
	};
	Branch* root;

};