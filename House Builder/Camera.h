#include "Common.h"

class Camera {
	GLfloat eyeX, eyeY, eyeZ;
	GLfloat centerX, centerY, centerZ;
	GLfloat vertical, horizontal;
	static Camera* handle;
	Camera();
public:
	static Camera* getCamera();
	void move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ);
	void zoom(GLfloat ratio);
	void rotate(GLfloat dVertical, GLfloat dHorizontal);
};

