#include "Common.h"
#include <cmath>

Camera::Camera() {
	eyeX = 0;
	eyeY = 0;
	eyeZ = 30;
	centerX = 0;
	centerY = 0;
	centerZ = 0;
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
}

Camera* Camera::handle;

Camera* Camera::getCamera() {
	if (!handle)
		handle = new Camera();
	return handle;
}

void Camera::move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ) {
	eyeX += deltaX;
	eyeY += deltaY;
	eyeZ += deltaZ;
	centerX += deltaX;
	centerY += deltaY;
	centerZ += deltaZ;
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
}

void Camera::zoom(GLfloat ratio) {

}

GLfloat distance(GLfloat a, GLfloat b, GLfloat c, GLfloat x, GLfloat y, GLfloat z) {
	return sqrt(pow(a - x, 2) + pow(b - y, 2) + pow(c - z, 2));
}

void Camera::rotate(GLfloat dVertical, GLfloat dHorizontal) {
	vertical += dVertical;
	horizontal += dHorizontal;
	GLfloat R = distance(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
	eyeX = centerX + R * cos(horizontal);
	eyeY = centerY + R * sin(horizontal);
	eyeZ = centerY + R * sin(vertical);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);

}