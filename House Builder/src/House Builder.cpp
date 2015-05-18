// House Builder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SDL.h>
#include "Common.h"
#include <iostream>
#include "Camera.h"
#include "Render.h"
#include "tiny_obj_loader.h"
#include "Obj.h"
#include "ObjLoader.h"
#include "GridMap.h"
#include <Rocket\Core.h>
#include <ShellSystemInterface.h>
#include <ShellRenderInterfaceOpenGL.h>

Render* render;
Camera* camera;

void myDisplay(void) {
	render->render();
}

void quit(int code){
	fprintf(stderr, "Quit !\n");
	SDL_Quit();
	exit(code);
}

void handle_key(SDL_keysym* keysym) {
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		quit(0);
		break;
	case SDLK_a:
		camera->move(-5, 0, 0);
		break;
	case SDLK_d:
		camera->move(5, 0, 0);
		break;
	case SDLK_w:
		camera->move(0, 0, -5);
		break;
	case SDLK_s:
		camera->move(0, 0, 5);
		break;
	case SDLK_z:
		camera->move(0, 5, 0);
		break;
	case SDLK_x:
		camera->move(0, -5, 0);
		break;
	case SDLK_j:
		camera->rotate(0.1, 0);
		break;
	case SDLK_k:
		camera->rotate(-0.1, 0);
		break;
	case SDLK_u:
		camera->rotate(0, 0.1);
		break;
	case SDLK_i:
		camera->rotate(0, -0.1);
		break;
	default:
		break;
	}
}

void process_events(void){
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type)
		{
		case SDL_KEYDOWN:
			handle_key(&event.key.keysym);
			break;
		case SDL_QUIT:
			quit(0);
			break;
		default:
			break;
		}
	}
}

void draw_screen(void) {
	render->render();
	SDL_GL_SwapBuffers();
}

void setup_opengl() {
	float ratio = windowWidth / windowHeight;
	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, windowWidth, windowHeight);
	Obj* o = ObjLoader::getInstance()->read("res/nielsen/nielsen.obj");
	//Obj* o = ObjLoader::getInstance()->read("res/House01/House01.obj");
	o->transform.rotation = glm::vec3(-90, -180, 0);	
	o->transform.scale = glm::vec3(0.25, 0.25, 0.25);
	//Obj* o = ObjLoader::getInstance()->read("res/rungholt/house.obj");
	GridMap* map = new GridMap(50);
	map->add(o);
	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
}
void myKeyboard(unsigned char key, int mouseX, int mouseY) {
	switch (key) {
	case 'a':
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


int _tmain(int argc, _TCHAR* argv[])
{
	const SDL_VideoInfo* info = NULL;
	int bpp = 0;
	int flags = 0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		quit(1);
	}

	info = SDL_GetVideoInfo();
	if (!info){
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		quit(1);
	}
	
	bpp = info->vfmt->BitsPerPixel;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	flags = SDL_OPENGL | SDL_FULLSCREEN;


	if (SDL_SetVideoMode(windowWidth, windowHeight, bpp, flags) == 0) {
		/*
		* This could happen for a variety of reasons,
		* including DISPLAY not being set, the specified
		* resolution not being available, etc.
		*/
		fprintf(stderr, "Video mode set failed: %s\n",
			SDL_GetError());
		quit(1);
	}

	setup_opengl();
	while (true)
	{
		process_events();
		draw_screen();
	}
	return 0;
}
