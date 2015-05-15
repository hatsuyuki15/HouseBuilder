#include "Common.h"
#include <iostream>

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void renderObj(Object* obj) {
	for (int i = 0; i < obj->faces.size(); i++) {
		glBegin(GL_POLYGON);
		face& f = (obj->faces)[i];
		for (int j = 0; j < f.size(); j++) {
			int vth = f[j] - 1;
			vertex& v = obj->vertexs[vth];
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}

void Render::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vector<Object*>& objects = map->objects;
	for (int i = 0; i < objects.size(); i++)
		renderObj(objects[i]);
	glFlush();
}

