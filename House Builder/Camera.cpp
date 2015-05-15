#include "Common.h"

Camera::Camera() {
	eyeX = 0;
	eyeY = 0;
	eyeZ = 30;
	
	centerX = 0;
	centerY = 0;
	centerZ = 0;

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//TODO: default value for Orthor
	glOrtho(-50, 50, -50, 50, -50, 50);
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

void Camera::rotate(GLfloat vertical, GLfloat horizontal) {

}