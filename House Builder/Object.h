#include "Common.h"
#include <vector>
#include <map>

using namespace std;

struct vertex {
	 GLfloat x, y, z;
};

struct material {
	string name;
	GLuint textureID;
	GLfloat a1, a2, a3;
	GLfloat s1, s2, s3;
	GLfloat d1, d2, d3;
};

struct fvertex {
	int iv, ivt, ivn;
};

struct face {
	vector<fvertex> fv;
	material* mtl;
};

struct Object {
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	vector<vertex> vertexs, nvertexs, tvertexs;
	vector<face> faces;
	map<string, material*> materials;
};