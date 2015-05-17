#include "Common.h"
#include <iostream>
#include "Camera.h"
#include "Render.h"
#include "tiny_obj_loader.h"
#include "Obj.h"
#include "ObjLoader.h"
#include "GridMap.h"

Render* render;
Camera* camera;

void myDisplay(void) {
	render->render();
}


std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;

void init() {
	std::string fileName = "res/nielsen/nielsen.obj";
	std::string err = tinyobj::LoadObj(shapes, materials, fileName.c_str(), "res/nielsen/");
	if (!err.empty()) {
		std::cerr << err << std::endl;
		exit(1);
	}
}

void myKeyboard(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 'a' :
		camera->move(-5, 0, 0);
		break;
	case 'd':
		camera->move(5, 0, 0);
		break;
	case 'w':
		camera->move(0, 5, 0);
		break;
	case 's':
		camera->move(0, -5, 0);
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
		camera->zoom(2.0f/3.0f);
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
	}
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){

}

void myMotion(int x, int y) {
	// crazy motion T.T, someone fix it, please
	glm::vec2 origin = glm::vec2(windowWidth / 2, windowHeight / 2);
	glm::vec2 cursor = glm::vec2(x, y);
	glm::vec2 direction = cursor - origin;
	camera->rotate(0.002*direction.y, 0);
	camera->rotate(0, 0.002*direction.x);
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Test");
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	//--------------------------------------------
	//init();	
	Obj* o = ObjLoader::getInstance()->read("res/nielsen/nielsen.obj");
	o->transform.rotation = glm::vec3(-90, -180, 0);	
	o->transform.scale = glm::vec3(0.25, 0.25, 0.25);
	//Obj* o = ObjLoader::getInstance()->read("res/rungholt/house.obj");
	GridMap* map = new GridMap(50);
	map->add(o);
	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
	//--------------------------------------------
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}