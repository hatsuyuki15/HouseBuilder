#include "Common.h"

class Instance {
public:
	Object* obj;
	glm::vec3 transform;
	GLfloat vertical, horizontal;
	bool hightlight;
	bool clipping;
	bool moveable;
	Instance(Object* object);
};

