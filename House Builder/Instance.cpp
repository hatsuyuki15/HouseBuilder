#include "Common.h"


Instance::Instance(Object* obj) {
	this->obj = obj;
	transform = glm::vec3(0, 0, 0);
	hightlight = false;
	clipping = false;
	moveable = true;
}
