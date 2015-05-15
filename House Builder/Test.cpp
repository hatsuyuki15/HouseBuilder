#include "Common.h"
#include <iostream>

Render* render;

void myDisplay(void) {
	render->render();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Test");
	//--------------------------------------------
	Loader loader;
	Object* obj = loader.read("E:\\test.obj");
	GridMap* map = new GridMap(50);
	map->add(obj);
	render = new Render();
	render->setGridMap(map);
	Camera* camera = Camera::getCamera();
	//--------------------------------------------
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}