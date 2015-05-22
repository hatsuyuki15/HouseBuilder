#include "Common.h"
#include "stdafx.h"

GLfloat windowWidth = 800;
GLfloat windowHeight = 600;
GLfloat maxFPS = 30;

using namespace glm;

ObjectManager* manager;
Render* render;
Camera* camera;
GridMap* map;
HUD* hud;
Object2D* obj2D;
Instance* selectedInstance;
Instance* house;
vec2 lastGridPosition;
vec2 relativeSelectedCell;
vec3 relativePosition;


void quit(int code){
	fprintf(stderr, "Quit !\n");
	SDL_Quit();
	exit(code);
}

void loadData() {
	manager = ObjectManager::getInstance();
	house = new Instance(manager->getHouse());
	house->transform = manager->getHouseTransform();
	map = new GridMap(30);
	map->addMask(house);

	hud = new HUD();
	obj2D = new Object2D("res\\hud.jpg");
	obj2D->position = vec2(0, 0);
	obj2D->width = 800;
	obj2D->height = 127;
	obj2D->setName("Huspro");
	hud->add(obj2D);
}

void init() {
	loadData();
	render = new Render();
	render->setGridMap(map);
	render->setHud(hud);
	camera = Camera::getCamera();

	glViewport(0, 0, windowWidth, windowHeight);
}

void onDrawing(void)  {
	render->render();
}

void onKeypressed(SDLKey key) {
	switch (key) {
	case SDLK_z:
		camera->zoom(1.1);
		break;
	case SDLK_x:
		camera->zoom(1.0 / 1.1);
		break;
	case SDLK_s:
		camera->rotate(0.2, 0);
		break;
	case SDLK_w:
		camera->rotate(-0.2, 0);
		break;
	case SDLK_a:
		camera->rotate(0, -0.2);
		break;
	case SDLK_d:
		camera->rotate(0, 0.2);
		break;
	case SDLK_h:
		house->clipping = !house->clipping;
		break;
	case SDLK_g: 
	{
		Loader* loader = Loader::getInstance();
		loader->export("save.obj", map->getInstances());
		break;
	}
	case SDLK_1:
		selectedInstance = new Instance(manager->objs[0]);
		selectedInstance->hightlight = true;
		map->add(selectedInstance);
		break;
	case SDLK_2:
		selectedInstance = new Instance(manager->objs[1]);
		selectedInstance->hightlight = true;
		map->add(selectedInstance);
		break;
	case SDLK_3:
		selectedInstance = new Instance(manager->objs[2]);
		selectedInstance->hightlight = true;
		map->add(selectedInstance);
		break;
	}
}

vec3 getWorldCoordinate(int mouseX, int mouseY, bool ghostMode) {
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
	if (ghostMode) {
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
	} else {
		glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
		return vec3(posX, posY, posZ);
	}
}

void onMouseMove(int mouseX, int mouseY) {
	if (selectedInstance) {
		vec3 world = getWorldCoordinate(mouseX, mouseY, true) + relativePosition;
		vec2 origin = map->getGridCoordinate(world) - relativeSelectedCell;
		if (origin != lastGridPosition) {
			bool result = map->put(selectedInstance, origin.x, origin.y, true);
			selectedInstance->hightlight = (result) ? HIGHTLIGHT_NORMAL : HIGHTLIGHT_BOLD;
			lastGridPosition = origin;		
		}
	}
}

void onMouseLeftClick(int mouseX, int mouseY) {
	if (selectedInstance) {
		vec3 world = getWorldCoordinate(mouseX, mouseY, true) + relativePosition;
		vec2 origin = map->getGridCoordinate(world) - relativeSelectedCell;
		bool result = map->put(selectedInstance, origin.x, origin.y, false);
		if (result) {
			selectedInstance->hightlight = HIGHTLIGHT_OFF;
			selectedInstance = NULL;
		}
	} else {
		vec3 pos_normal_mode = getWorldCoordinate(mouseX, mouseY, false);
		vec3 pos_ghost_mode  =  getWorldCoordinate(mouseX, mouseY, true);
		relativePosition = pos_normal_mode - pos_ghost_mode;
		vec2 selectedCell = map->getGridCoordinate(pos_normal_mode);
		selectedInstance = map->getInstanceAt(selectedCell.x, selectedCell.y);
		if (selectedInstance) {
			vec2 origin = map->getGridCoordinate(selectedInstance->transform);
			relativeSelectedCell = selectedCell - origin;
			selectedInstance->hightlight = HIGHTLIGHT_NORMAL;
		}
	}
}

void onMouseWheelUp() {
	camera->zoom(1.0 / 1.1);
}

void onMouseWheelDown() {
	camera->zoom(1.1);
}

void draw_screen(void) {
	render->render();
	SDL_GL_SwapBuffers();
}	

// Because process_event occur only when has events, so we need write this func to
// make something really cool when user is not interracting with the inputs (keyboard, mouse)

void input_handle() {
	int x, y;
	Uint8 mouse_state = SDL_GetMouseState(&x, &y);
	Uint8* key_state = SDL_GetKeyState(NULL);	
	
	// move camera when mouse reach the border of screen
	vec2 center = vec2(windowWidth / 2, windowHeight / 2);
	vec2 border = vec2(windowWidth - 10, windowHeight - 10) - center;
	vec2 cursor = vec2(x, y);
	vec2 direction = cursor - center;
	GLfloat moving_speed = 0.01;
	GLfloat margin = 10;
	if (abs(direction.x) > abs(border.x) - margin || abs(direction.y) > abs(border.y) - margin)
		camera->move(moving_speed * direction.x, 0, moving_speed * direction.y);	
}

void process_events(void){
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type)
		{
		case SDL_KEYDOWN:
			onKeypressed(event.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION: 
			onMouseMove(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) 
			{
			case SDL_BUTTON_WHEELDOWN:
				onMouseWheelDown();
				break;
			case SDL_BUTTON_WHEELUP:
				onMouseWheelUp();
				break;
			case SDL_BUTTON_LEFT:
				onMouseLeftClick(event.button.x, event.button.y);
				break;
			}
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

	init();

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