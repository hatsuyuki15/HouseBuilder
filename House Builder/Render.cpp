#include "Common.h"

using namespace std;

Render::Render() {
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//TODO: default value for Orthor
	glOrtho(-50, 50, -50, 50, -50, 50);
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void renderObj(Instance* instance) {
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
}

void Render::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vector<Instance*> instances = map->getInstances();
	for (int i = 0; i < instances.size(); i++)
		renderObj(instances[i]);
	glFlush();
}