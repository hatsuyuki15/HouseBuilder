#include "Common.h"

class Render {
	GridMap* map;
public:
	Render();
	void setGridMap(GridMap* map);
	void render();
};

