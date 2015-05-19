#include "Common.h"

class Instance {
	Object* obj;
public:
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	Instance(Object* object);
};

