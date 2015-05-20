#include "Common.h"


class Object2D {
	char* texture;
public:
	char* name;
	Object2D();
	Object2D(char* texture);
	void setName(char name[]);
	glm::vec2 position;
	Uint32 textureID;
	int width;
	int height;
};
