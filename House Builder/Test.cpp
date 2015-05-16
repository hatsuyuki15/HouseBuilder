#include "Common.h"

Render* render;

void myDisplay(void) {
	render->render();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Test");
	//--------------------------------------------
	Loader* loader = Loader::getInstance();
	Object* obj = loader->read("E:\\Downloads\\rungholt\\house.obj");
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