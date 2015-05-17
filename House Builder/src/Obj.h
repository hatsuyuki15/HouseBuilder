#ifndef _OBJ_H
#define _OBJ_H

#include "Common.h"
#include "tiny_obj_loader.h"

typedef std::vector<tinyobj::shape_t> Shapes;
typedef std::vector<tinyobj::material_t> Materials;

struct Transformation {
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

class Obj {
public:
	char* fileName;
	std::string resourceFolder;
	Shapes shapes;
	Materials materials;
	Transformation transform;
	Obj* clone();
	glm::mat4 getModelMatrix();
private:
	glm::mat4 modelMatrix;
};

#endif