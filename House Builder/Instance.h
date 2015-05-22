#include "Common.h"

#define HIGHTLIGHT_OFF 0
#define HIGHTLIGHT_NORMAL 1
#define HIGHTLIGHT_BOLD 2

class Instance {
public:
	Object* obj;
	glm::vec3 transform;
	GLfloat vertical, horizontal;
	int hightlight;
	bool clipping;
	bool moveable;
	Instance(Object* object);
};

