#include "Common.h"
#include <vector>
#include <map>

typedef glm::vec3 vertex;

struct material {
	std::string name;
	GLuint textureID;
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
};

struct fvertex {
	int iv, ivt, ivn;
};

struct face {
	std::vector<fvertex> fv;
	material* mtl;
};

struct Object {
	GLfloat width, height;
	void calculateSize();
	char *name;
public:
	std::vector<vertex> vertexs, nvertexs, tvertexs;
	std::vector<face> faces;
	std::map<std::string, material*> materials;
	Object();
	void setName(char name[]);
	GLfloat getWidth();
	GLfloat getHeight();
};