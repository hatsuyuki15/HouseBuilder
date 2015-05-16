#include "Common.h"
#include <iostream>

Render::Render() {
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void renderObj(Object* obj) {
	for (int i = 0; i < obj->faces.size(); i++) {
		glBegin(GL_POLYGON);
		face& f = (obj->faces)[i];
		material* mtl = f.mtl;
		glBindTexture(GL_TEXTURE_2D, mtl->textureID);
		for (int j = 0; j < f.fv.size(); j++) {
			fvertex fv = f.fv[j];
			vertex& v  = obj->vertexs[fv.iv - 1];
			vertex& vt = obj->tvertexs[fv.ivt - 1];
			glTexCoord2f(vt.x, vt.y);
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}

void Render::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vector<Object*>& objects = map->objects;
	for (int i = 0; i < objects.size(); i++)
		renderObj(objects[i]);
	glFlush();
}