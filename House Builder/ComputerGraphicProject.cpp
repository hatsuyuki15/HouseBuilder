// ComputerGraphicProject.cpp : Defines the entry point for the console application.
//

#include "Common.h"
#include "stdafx.h"

#ifndef __HOUSE_BUILDER_CONFIG
#define __HOUSE_BUILDER_CONFIG

GLfloat windowWidth = 800;
GLfloat windowHeight = 600;
GLfloat maxFPS = 30;

#endif

using namespace glm;

Render* render;
Camera* camera;
GridMap* map;
Object* obj1;
Instance* selectedInstance;
vec2 lastGridPosition;


void quit(int code){
	fprintf(stderr, "Quit !\n");
	SDL_Quit();
	exit(code);
}

void loadData() {
	map = new GridMap(30, -450, -900);
	Loader* loader = Loader::getInstance();
	obj1 = loader->read("res\\nielsen\\nielsen.obj");
	//	obj1 = loader->read("E:\\Downloads\\house\\house.obj");
}

void init() {
	loadData();
	render = new Render();
	render->setGridMap(map);
	camera = Camera::getCamera();
}

void onDrawing(void)  {
	render->render();
}

void onKeypressed(Uint8* key_state) {
	if (key_state[SDLK_a])
		camera->move(1, 0, 0);
	if (key_state[SDLK_d])
		camera->move(-1, 0, 0);
	if (key_state[SDLK_w])
		camera->move(0, 1, 0);
	if (key_state[SDLK_s])
		camera->move(0, -1, 0);
	if (key_state[SDLK_q])
		camera->move(0, 0, -5);
	if (key_state[SDLK_e])
		camera->move(0, 0, 5);
	if (key_state[SDLK_z])
		camera->zoom(1.5);
	if (key_state[SDLK_x])
		camera->zoom(2.0f / 3.0f);
	if (key_state[SDLK_j])
		camera->rotate(0.2, 0);
	if (key_state[SDLK_k])
		camera->rotate(-0.2, 0);
	if (key_state[SDLK_u])
		camera->rotate(0, -0.2);
	if (key_state[SDLK_i])
		camera->rotate(0, 0.2);
	if (key_state[SDLK_n]) {
		selectedInstance = new Instance(obj1);
		map->add(selectedInstance);
	}
	if (key_state[SDLK_g]) {
		Loader* loader = Loader::getInstance();
		loader->export("save.obj", map->getInstances());
	}
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
	}
	else if (u.y != 0) {
		float k = -oN.y / u.y;
		return oN + k*u;
	}
}

void onMouseMove(int mouseX, int mouseY) {
	if (selectedInstance) {
		vec3 world = getWorldCoordinate(mouseX, mouseY);
		vec2 position = map->getGridCoordinate(world);
		if (position != lastGridPosition) {
			map->put(selectedInstance, position.x, position.y);
			lastGridPosition = position;			
		}
	}
}

void onMouseLeftClick(int mouseX, int mouseY) {
	vec3 world = getWorldCoordinate(mouseX, mouseY);
	vec2 position = map->getGridCoordinate(world);
	if (selectedInstance) {
		if (map->isPuttable(selectedInstance, position.x, position.y)) {
			selectedInstance->hightlight = false;
			selectedInstance = NULL;
		}
	}
	else {
		selectedInstance = map->getInstanceAt(position.x, position.y);
		if (selectedInstance) {
			selectedInstance->hightlight = true;
		}
	}
}

void draw_screen(void) {
	render->render();
	SDL_GL_SwapBuffers();
}

void setup_opengl() {	
	glShadeModel(GL_SMOOTH);	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	
	glViewport(0, 0, windowWidth, windowHeight);	
	init();
}


// Because process_event occur only when has events, so we need write this func to
// make something really cool when user is not interracting with the inputs (keyboard, mouse)

void input_handle() {
	int x, y;
	Uint8 mouse_state = SDL_GetMouseState(&x, &y);

	Uint8* key_state = SDL_GetKeyState(NULL);
	
	onKeypressed(key_state);
	
	if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)){
		onMouseLeftClick(x, y);
	}
	else{
		onMouseMove(x, y);
	}

	// move camera when mouse reach the border of screen
	float mouse_speed = 0.01;
	glm::vec2 center = glm::vec2(windowWidth / 2, windowHeight / 2);
	glm::vec2 border = glm::vec2(windowWidth - 10, windowHeight - 10) - center;
	glm::vec2 cursor = glm::vec2(x, y);
	glm::vec2 direction = cursor - center;
	if (abs(direction.x) > abs(border.x) || abs(direction.y) > abs(border.y)) {
		camera->move(mouse_speed * direction.x, 0, mouse_speed * direction.y);
	}
}

void process_events(void){
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type)
		{
		case SDL_KEYDOWN:
			//handle_key(&event.key.keysym);
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

/* ----------------------------------------------------------------------- */