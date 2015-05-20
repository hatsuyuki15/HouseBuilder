#include "Common.h"

HUD::HUD() {
}

std::vector<Object2D*> HUD::getObjectList() {
	printf("%d", objlist.size());
	return objlist;
}

void HUD::add(Object2D* obj) {
	objlist.push_back(obj);
}

Object2D* HUD::getObjectByXY(int x, int y) {
	Object2D* obj = NULL;
	glm::vec2 mouse = glm::vec2(x, y);
	for (int i = 0; i < objlist.size(); i++) {
		glm::vec2 delta = mouse - objlist[i]->position;
		obj = objlist[i];
		if (delta.x >= 0 && delta.y >= 0 && x <= obj->width && delta.y <= obj->height){
			return obj;
		}
	}
	return NULL;
}

Object2D* HUD::getObjectByName(char name[]) {
	return NULL;
}