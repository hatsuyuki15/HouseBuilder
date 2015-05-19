#include "Common.h"
#include <vector>
#include <map>

typedef glm::vec3 vertex;

struct material {
	std::string name;
	GLuint textureID;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct fvertex {
	int iv, ivt, ivn;
};

struct face {
	std::vector<fvertex> fv;
	material* mtl;
};

struct Object {
	std::vector<vertex> vertexs, nvertexs, tvertexs;
	std::vector<face> faces;
	std::map<std::string, material*> materials;
};