#include "Common.h"

using namespace std;

Render::Render() {
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}
#include <iostream>
void renderObj(Instance* instance) {
	glPushMatrix();
	glTranslatef(instance->x, instance->y, instance->z);
	Object* obj = instance->obj;
	for (int i = 0; i < obj->faces.size(); i++) {
		face& f = obj->faces[i];
		glBindTexture(GL_TEXTURE_2D, (f.mtl)->textureID);
		glBegin(GL_POLYGON);
		for (int j = 0; j < f.fv.size(); j++) {
			fvertex& fv = f.fv[j];
			vertex& v  = obj->vertexs[fv.iv - 1];
			vertex& vt = obj->tvertexs[fv.ivt - 1];
			glTexCoord2f(vt.x, vt.y);
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
	glPopMatrix();
}

void Render::render() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderGridLayout();
	vector<Instance*> instances = map->getInstances();
	for (int i = 0; i < instances.size(); i++)
		renderObj(instances[i]);
	glFlush();
	glMatrixMode(GL_PROJECTION);
}

void Render::renderGridLayout() {
	int cellSize = map->getCellSize();
	int width  = map->getWidth();
	int height = map->getHeight();
	GLfloat borderOX, borderOY, borderX, borderY;
	map->toWorldLocation(borderOX, borderOY, 0, 0);
	map->toWorldLocation(borderX, borderY, width - 1, height - 1);
	glBegin(GL_LINES);
	for (int i = 0; i < width; i++) {
		glVertex3f(borderOX + cellSize * i, 0, borderOY);
		glVertex3f(borderOX + cellSize * i, 0, borderY);
	}
	for (int i = 0; i < height; i++) {
		glVertex3f(borderOX, 0, borderOY + cellSize * i);
		glVertex3f(borderX, 0,  borderOY + cellSize * i);
	}
	glEnd();
}