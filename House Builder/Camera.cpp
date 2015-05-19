#include "Common.h"
#include <cmath>

Camera::Camera() {	
	viewAngle = 45;
	aspect = 1;
	zNear = 0.1;
	zFar = 1000;
	
	GLfloat R = 100;
	centerX = 0;
	centerY = 0;
	centerZ = 0;
	horizontal = 0;
	vertical = 0;
	eyeX = centerX + R * sin(vertical) * cos(horizontal);
	eyeY = centerY + R * sin(horizontal);
	eyeZ = centerZ + R * cos(vertical) * cos(horizontal);
	update();
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
	update();
}


GLfloat distance(GLfloat a, GLfloat b, GLfloat c, GLfloat x, GLfloat y, GLfloat z) {
	return sqrt(pow(a - x, 2) + pow(b - y, 2) + pow(c - z, 2));
}

void Camera::zoom(GLfloat ratio) {
	GLfloat R = distance(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
	R /= ratio;
	eyeX = centerX + R * sin(vertical) * cos(horizontal);
	eyeY = centerY + R * sin(horizontal);
	eyeZ = centerZ + R * cos(vertical) * cos(horizontal);
	update();
}

void Camera::rotate(GLfloat dVertical, GLfloat dHorizontal) {
	GLfloat R = distance(eyeX, eyeY, eyeZ, centerX, centerY, centerZ);
	vertical += dVertical;
	horizontal += dHorizontal;
	eyeX = centerX + R * sin(vertical) * cos(horizontal);
	eyeY = centerY + R * sin(horizontal);
	eyeZ = centerZ + R * cos(vertical) * cos(horizontal);
	update();
}

void Camera::update() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, zNear, zFar);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
}