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
Obj* person;
GridMap* map;

const static int MOUSE_RELEASE = 0;
const static int MOUSE_LEFT_PRESS = 1;
const static int MOUSE_RIGHT_PRESS = 2;
const static int MOUSE_MIDDLE_PRESS = 3;
const static int MOUSE_DRAGGING = 4;

int mouse_state = MOUSE_RELEASE;

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
		//person->move(1, 0, 0);		
		break;
	case SDLK_d:
		//person->move(-1, 0, 0);
		break;
	case SDLK_w:
		//person->move(0, 0, 1);
		break;
	case SDLK_s:
		//person->move(0, 0, -1);
		break;
	case SDLK_z:
		camera->move(0, 1, 0);
		break;
	case SDLK_x:
		camera->move(0, -1, 0);
		break;
	case SDLK_j:
		camera->rotate(0, 0.1);
		break;
	case SDLK_k:
		camera->rotate(0, -0.1);
		break;
	case SDLK_u:
		camera->rotate(0.1, 0);
		break;
	case SDLK_i:
		camera->rotate(-0.1, 0);
		break;
	default:
		break;
	}
}

// Because process_event occur only when has events, so we need write this func to
// make something really cool when user is not interracting with the inputs (keyboard, mouse)
void input_handle() {
	int x, y;
	Uint8 mouse_state =SDL_GetMouseState(&x, &y);

	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
		Obj* o;
		if (o = map->getObjectByMouseXY(x, y)){
			printf("%s \n", o->name);
		}
	}

	//Uint8* key_state = SDL_GetKeyState(NULL);

	// mouse handle
	float mouse_speed = 0.01;

	glm::vec2 center = glm::vec2(windowWidth / 2, windowHeight / 2);
	glm::vec2 border = glm::vec2(windowWidth - 10, windowHeight - 10) - center;
	glm::vec2 cursor = glm::vec2(x, y);
	glm::vec2 direction = cursor - center;
	if (abs(direction.x) > abs(border.x) || abs(direction.y) > abs(border.y)) {
		camera->move(mouse_speed * direction.x, 0, mouse_speed * direction.y);
	}
}

void handle_mouse_motion(SDL_MouseMotionEvent* motion) {
	Uint8 state = motion->state;
	
	int x = motion->x;
	int y = motion->y;

	//printf("%d %d\n", x, y);
	float mouse_speed = 0.01;

	glm::vec2 center = glm::vec2(windowWidth / 2, windowHeight / 2);
	glm::vec2 border = glm::vec2(windowWidth - 10, windowHeight - 10) - center;
	glm::vec2 cursor = glm::vec2(x, y);
	glm::vec2 direction = center - cursor;
	if (abs(direction.x) > abs(border.x) || abs(direction.y) > abs(border.y)) {		
		camera->move(mouse_speed * direction.x, 0, mouse_speed * direction.y);
	}
	
}


void handle_mouse_button(SDL_MouseButtonEvent* button) {
	switch (button->button){
	case SDL_BUTTON_LEFT:		
		break;
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
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
		case SDL_MOUSEMOTION:
			//handle_mouse_motion(&event.motion);
			break;
		case SDL_MOUSEBUTTONDOWN:
			//handle_mouse_button(&event.button);
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
	//glFrontFace(GL_CCW);
	
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, windowWidth, windowHeight);
	person = ObjLoader::getInstance()->read("res/nielsen/nielsen.obj");		
	person->transform.position = glm::vec3(5, 5, 5);
	person->transform.rotation = glm::vec3(90, -180, 0);	
	person->transform.scale = glm::vec3(0.2, 0.2, 0.2);
	//Obj* chair = ObjLoader::getInstance()->read("res/chair/chair.obj");
	Obj* skybox = ObjLoader::getInstance()->read("res/models/skybox.obj");
	//Obj* terrain = ObjLoader::getInstance()->read("res/models/terrain1.obj");
	//terrain->transform.scale = glm::vec3(100, 1, 100);
	Obj* capsule = ObjLoader::getInstance()->read("res/capsule/capsule.obj");
	capsule->setName("Obj 1 Gay");
	Obj* capsule2 = capsule->clone();
	capsule2->setName("Obj 2 Les");
	Obj* capsule3 = capsule->clone();	
	capsule3->setName("Obj 3 BD");
	Obj* capsule4 = capsule->clone();	
	capsule4->setName("Obj 4 Girl");
	skybox->transform.scale = glm::vec3(5, 5, 5);	
	skybox->transform.rotation = glm::vec3(-90, 0, 0);
	map = new GridMap(5);
	//map->add(person);
	//map->add(monkey);
	//map->add(chair);
	map->addObj(capsule, 0, 0);
	map->addObj(capsule2, 1, 1);
	map->addObj(capsule3, 2, 2);
	map->addObj(capsule4, 3, 5);
	map->addEnvironment(skybox);
	//map->addEnvironment(terrain);	

	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
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

	flags = SDL_OPENGL;// | SDL_FULLSCREEN;
	
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

	//SDL_ShowCursor(SDL_DISABLE);

	while (true)
	{
		Uint32 start = SDL_GetTicks();		
		process_events();
		input_handle();
		draw_screen();
		if (1000 / maxFPS > SDL_GetTicks() - start)
			SDL_Delay(1000 / maxFPS - (SDL_GetTicks() - start));
	}
	return 0;
}
