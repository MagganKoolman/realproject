#pragma once

#include <map>
#include "Model.h"
#include "Datastructs.h"
#include <string>
#include <gl\glew.h>
#include <vector>

class OBJimporter {
	private:
		struct vtx {
			float x, y, z;
		};
		struct nvtx {
			float x, y, z;
		};
		struct tex {
			float u, v;
		};
		struct face {
			int v[3], n[3], t[3];
		};
		struct object {
			Material* mtl;
			std::vector<face> faces;
		};

		std::vector<Material> allMaterials;
		std::vector<vtx> vertices;
		std::vector<nvtx> normalVertices;
		std::vector<tex> textureCoords;
		std::vector<object*> objects;

		std::map<std::string, GLuint> texmapid;

		Material standardMat;
		int objectOffset;
	public:
		OBJimporter();
		~OBJimporter();
		void loadObj(std::string fileName);
		std::vector<Model*>* CreateTriangleData();
		void loadMaterials(std::string materials);
		GLuint CreateTexture(std::string fileName);
};