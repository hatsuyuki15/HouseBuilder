#include "Common.h"
#include <string>

class Loader {
public:
	Object* read(char file[]);
	void write(char file[], Object* obj);
};