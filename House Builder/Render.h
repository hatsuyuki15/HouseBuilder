#include "Common.h"

class Render {
	GridMap* map;
	void renderGridLayout();
public:
	Render();
	void setGridMap(GridMap* map);
	void render();
};

