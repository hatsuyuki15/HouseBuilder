#include "Common.h"

class Instance {
public:
	Object* obj;
	GLfloat x, y, z;
	GLfloat vertical, horizontal;
	bool hightlight;
	bool clipping;
	bool moveable;
	Instance(Object* object);
};

