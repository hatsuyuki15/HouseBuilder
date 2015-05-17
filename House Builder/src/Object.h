#include "Common.h"
#include <vector>
#include <map>

struct vertex {
	 GLfloat x, y, z;
};

struct material {
	std::string name;
	GLuint textureID;
	GLfloat a1, a2, a3;
	GLfloat s1, s2, s3;
	GLfloat d1, d2, d3;
};

struct fvertex {
	int iv, ivt, ivn;
};

struct face {
	std::vector<fvertex> fv;
	material* mtl;
};

struct Object {
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	std::vector<vertex> vertexs, nvertexs, tvertexs;
	std::vector<face> faces;
	std::map<std::string, material*> materials;
};