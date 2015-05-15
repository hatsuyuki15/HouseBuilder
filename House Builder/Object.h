#include "Common.h"
#include <vector>

using namespace std;

typedef vector<int> face;
struct vertex {
	 GLfloat x, y, z;
};

class Object {
public:
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	vector<vertex> vertexs;
	vector<face> faces;
};