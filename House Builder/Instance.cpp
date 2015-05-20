#include "Common.h"


Instance::Instance(Object* obj) {
	this->obj = obj;
	x = y = z = 0;
	hightlight = true;
	clipping = false;
	moveable = true;
}
