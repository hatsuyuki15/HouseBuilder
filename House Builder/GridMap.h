#include "Common.h"

const int MAX_GRID_SIZE = 1000;

struct BBox {
	int x, y;
	int width, height;
	bool standby;
};

class GridMap {
	GLfloat cellSize;
	GLfloat originX, originY;
	Instance* grid[MAX_GRID_SIZE][MAX_GRID_SIZE];
	std::map<Instance*, BBox> bmap;
public:
	GridMap(GLfloat cellSize, GLfloat originX, GLfloat originY);
	GLfloat getCellSize();
	GLfloat getWidth();
	GLfloat getHeight();
	Instance* getInstanceAt(int x, int y);
	void toGridLocation(GLfloat worldX, GLfloat worldY, int& x, int& y);
	void toWorldLocation(GLfloat& worldX, GLfloat& worldY, int x, int y);
	bool isPuttable(Instance* instance, int x, int y);
	void put(Instance* instance, int x, int y);
	void add(Instance* instance);
	std::vector<Instance*> getInstances();
};

