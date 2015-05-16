#include "Common.h"
#include <string>

class Loader {
	static Loader* handle;
	Loader();
public:
	static Loader* getInstance();
	Object* read(char file[]);
	void write(char file[], Object* obj);
};