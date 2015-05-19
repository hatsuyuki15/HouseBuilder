#include "Common.h"

class Instance {
public:
	Object* obj;
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	Instance(Object* object);
};

