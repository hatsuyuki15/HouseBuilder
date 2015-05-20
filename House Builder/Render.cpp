#include "Common.h"

using namespace std;

Render::Render() {
	//enviroment
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);

	//lighting
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat position[] = { 0, 10, 0, 1.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//clipping plane
	double eqr[] = { 0.0f, -1.0f, 0.0f, 0.0f };
	glClipPlane(GL_CLIP_PLANE0, eqr);
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void Render::setHud(HUD* hud) {
	this->hud = hud;	
}

void renderObj(Instance* instance) {
	glPushMatrix();

	//transformation
	glTranslatef(instance->transform.x, instance->transform.y, instance->transform.z);
	
	//clipping
	if (instance->clipping) 	
		glEnable(GL_CLIP_PLANE0);
	
	//rendering
	Object* obj = instance->obj;
	for (int i = 0; i < obj->faces.size(); i++) {
		face& f = obj->faces[i];
		if (instance->hightlight) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_POLYGON);
			for (int j = 0; j < f.fv.size(); j++) {
				fvertex& fv = f.fv[j];
				vertex& v = obj->vertexs[fv.iv - 1];
				if (fv.ivn > 0) {
					vertex& vn = obj->nvertexs[fv.ivn - 1];
					glNormal3f(vn.x, vn.y, vn.z);
				}
				glVertex3f(v.x, v.y, v.z);
			}
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glMaterialfv(GL_FRONT, GL_AMBIENT, f.mtl->ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, f.mtl->diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, f.mtl->specular);
			glBindTexture(GL_TEXTURE_2D, f.mtl->textureID);
			glBegin(GL_POLYGON);
			for (int j = 0; j < f.fv.size(); j++) {
				fvertex& fv = f.fv[j];
				vertex& v = obj->vertexs[fv.iv - 1];
				if (fv.ivn > 0) {
					vertex& vn = obj->nvertexs[fv.ivn - 1];
					glNormal3f(vn.x, vn.y, vn.z);
				}
				if (fv.ivt > 0) {
					vertex& vt = obj->tvertexs[fv.ivt - 1];
					glTexCoord2f(vt.x, vt.y);
				}			
				glVertex3f(v.x, v.y, v.z);
			}
			glEnd();
		}
	}
	
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
}


void renderObj2D(Object2D* obj) {
	glPushMatrix();

	//transformation
	glTranslatef(obj->position.x, obj->position.y, 0);

	//rendering	
	bool use_texture = (obj->textureID >= 0);
	
	if (use_texture) {
		glBindTexture(GL_TEXTURE_2D, obj->textureID);
		glBegin(GL_POLYGON);
			glTexCoord2d(0, 0);
			glVertex2i(0, 0);
			glTexCoord2d(1, 0);
			glVertex2i(obj->width, 0);
			glTexCoord2d(1, 1);
			glVertex2i(obj->width, obj->height);
			glTexCoord2d(0, 1);
			glVertex2i(0, obj->height);
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);		
			glVertex2i(obj->height, 0);
			glVertex2i(obj->height, obj->width);
			glVertex2i(0, obj->width);
			glVertex2i(0, 0);
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
	glMatrixMode(GL_PROJECTION);
	renderHud();
}

void Render::renderHud() {
	if (!hud) {
		printf("Can't find HUD instance\n");
		return;
	}
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	gluOrtho2D(0, 800, 0, 600);
	vector<Object2D*> objlist = hud->getObjectList();

	for (int i = 0; i < objlist.size(); i++){
		renderObj2D(objlist[i]);
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
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