#ifndef _RENDER_H
#define _RENDER_H

#include "Common.h"
#include "GridMap.h"
#include "Camera.h"
#include "Obj.h"

class Render {
	GridMap* map;
	Camera* camera;
public:
	Render();
	void setGridMap(GridMap* map);
	void render();
	void renderObj(Obj* obj);
};

#endif

