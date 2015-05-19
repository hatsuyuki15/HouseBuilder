#include "Common.h"
#include <iostream>

using namespace glm;

Render* render;
Camera* camera;
GridMap* map;
Object* obj1;
Instance* instance;

void loadData() {
	map = new GridMap(30, -450, -900);
	Loader* loader = Loader::getInstance();
	obj1 = loader->read("E:\\Downloads\\nielsen\\nielsen.obj");
}

void init() {
	loadData();
	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
}

void myDisplay( void )  {
	render->render();
}

void myKeyboard(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 'a':
		camera->move(1, 0, 0);
		break;
	case 'd':
		camera->move(-1, 0, 0);
		break;
	case 'w':
		camera->move(0, 1, 0);
		break;
	case 's':
		camera->move(0, -1, 0);
		break;
	case 'q':
		camera->move(0, 0, -5);
		break;
	case 'e':
		camera->move(0, 0, 5);
		break;
	case 'z':
		camera->zoom(1.5);
		break;
	case 'x':
		camera->zoom(2.0f / 3.0f);
		break;
	case 'j':
		camera->rotate(0.2, 0);
		break;
	case 'k':
		camera->rotate(-0.2, 0);
		break;
	case 'u':
		camera->rotate(0, -0.2);
		break;
	case 'i':
		camera->rotate(0, 0.2);
		break;
	case 'n':
		instance = new Instance(obj1);
		map->add(instance);
		break;
	}
	glutPostRedisplay();
}

vec3 getWorldCoordinate(int mouseX, int mouseY) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)mouseX;
	winY = (float)viewport[3] - (float)mouseY;
	gluUnProject(winX, winY, 0, modelview, projection, viewport, &posX, &posY, &posZ);
	vec3 oN = vec3(posX, posY, posZ);
	gluUnProject(winX, winY, 1, modelview, projection, viewport, &posX, &posY, &posZ);
	vec3 oF = vec3(posX, posY, posZ);
	vec3 u = oF - oN;
	u = glm::normalize(u);
	float k;
	if (oN.y == 0){
		return oN;
	}
	else if (u.y != 0) {
		k = -oN.y / u.y;
		return oN + k*u;
	}
}

int lastX, lastY;
void myMouse(int mouseX, int mouseY) {
	if (instance) {
		int x, y;
		vec3 world = getWorldCoordinate(mouseX, mouseY);
		map->toGridLocation(world.x, world.z, x, y);
		if (x != lastX || y != lastY) {
			map->put(instance, x, y);
			lastX = x;
			lastY = y;
			glutPostRedisplay();
		}
	}
}


int main( int argc, char *argv[] )  {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("House Builder");
  glutDisplayFunc(myDisplay);
  glutKeyboardFunc(myKeyboard);
  glutPassiveMotionFunc(myMouse);
  init();
  glutMainLoop( );
}

/* ----------------------------------------------------------------------- */