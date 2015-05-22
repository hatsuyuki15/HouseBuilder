#include "Common.h"
#include <GL/GLUT.h>

using namespace glm;

Render* render;
Camera* camera;
GridMap* map;
Object* obj1;
Instance* selectedInstance;
vec2 lastGridPosition;

void loadData() {
	map = new GridMap(30);
	Loader* loader = Loader::getInstance();
	obj1 = loader->read("E:\\Downloads\\couch\\couch.obj");
//	obj1 = loader->read("E:\\Downloads\\house\\house.obj");
}

void init() {
	loadData();
	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
}

void onDrawing( void )  {
	render->render();
}

void onKeypressed(unsigned char key, int mouseX, int mouseY) {
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
		printf("ok");
		selectedInstance = new Instance(obj1);
		map->add(selectedInstance);
		break;
	case 'g':
		Loader* loader = Loader::getInstance();
		loader->export("save.obj", map->getInstances());
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
	if (oN.y == 0){
		return oN;
	} else if (u.y != 0) {
		float k = -oN.y / u.y;
		return oN + k*u;
	}
}

void onMouseMove(int mouseX, int mouseY) {
	if (selectedInstance) {
		vec3 world = getWorldCoordinate(mouseX, mouseY);
		vec2 position = map->getGridCoordinate(world);
		if (position != lastGridPosition) {
			map->put(selectedInstance, position.x, position.y, true);
			lastGridPosition = position;
			glutPostRedisplay();
		}
	}
}

void onMouseClick(int button, int state, int mouseX, int mouseY) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		vec3 world = getWorldCoordinate(mouseX, mouseY);
		vec2 position = map->getGridCoordinate(world);
		if (selectedInstance) {
			map->put(selectedInstance, position.x, position.y, false);
				selectedInstance->hightlight = false;
				selectedInstance = NULL;
				glutPostRedisplay();
		} else {
			selectedInstance = map->getInstanceAt(position.x, position.y);
			if (selectedInstance) {
				selectedInstance->hightlight = true;
				glutPostRedisplay();
			}
		}
	}
}


int main( int argc, char *argv[] )  {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 150);
	glutCreateWindow("House Builder");
	glutDisplayFunc(onDrawing);
	glutKeyboardFunc(onKeypressed);
	glutMouseFunc(onMouseClick);
	glutPassiveMotionFunc(onMouseMove);
	init();
	glutMainLoop( );
	return 0;
}

/* ----------------------------------------------------------------------- */