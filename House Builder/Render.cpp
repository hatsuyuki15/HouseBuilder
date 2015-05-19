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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	glm::vec3 minBorder = map->getWorldCoordinate(0, 0);
	glm::vec3 maxBorder = map->getWorldCoordinate(width - 1, height - 1);
	glBegin(GL_LINES);
	for (int i = 0; i < width; i++) {
		GLfloat x = minBorder.x + cellSize * i;
		glVertex3f(x, 0, minBorder.z);
		glVertex3f(x, 0, maxBorder.z);
	}
	for (int i = 0; i < height; i++) {
		GLfloat z = minBorder.z + cellSize * i;
		glVertex3f(minBorder.x, 0, z);
		glVertex3f(maxBorder.x, 0, z);
	}
	glEnd();
}