#include "Common.h"

class ObjectManager {
	static ObjectManager *handle;
	Object* house;
	glm::vec3 houseTransform;
public:
	std::vector<Object*> objs;
	ObjectManager();
	static ObjectManager* getInstance();
	Object* getHouse();
	glm::vec3 getHouseTransform();
};

