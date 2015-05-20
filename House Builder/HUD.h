
#include "Common.h"
#include <vector>

class HUD {
public:
	std::vector<Object2D*> objlist;
	HUD ();
	std::vector<Object2D*> getObjectList();
	void add(Object2D* obj);
	Object2D* getObjectByXY(int x, int y);
	Object2D* getObjectByName(char name[]);
};
