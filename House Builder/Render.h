#include "Common.h"

class Render {
	GridMap* map;
	HUD* hud;
	void renderGridLayout();
	void renderHud();
public:
	Render();
	void setGridMap(GridMap* map);
	void setHud(HUD* hud);
	void render();
};

