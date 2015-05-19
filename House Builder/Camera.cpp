#include "Common.h"
#include <cmath>
using namespace glm;

Camera::Camera() {	
	viewAngle = 45;
	aspect = 1;
	zNear = 0.1;
	zFar = 1000;
	
	GLfloat R = 100;
	center = vec3(0, 0, 0);
	horizontal = 0;
	vertical = 0.3;
	eye.x = center.x + R * sin(horizontal) * cos(vertical);
	eye.y = center.y + R * sin(vertical);
	eye.z = center.z + R * cos(horizontal) * cos(vertical);
	update();
}

Camera* Camera::handle;

Camera* Camera::getCamera() {
	if (!handle)
		handle = new Camera();
	return handle;
}

void Camera::move(GLfloat deltaX, GLfloat deltaY, GLfloat deltaZ) {
	eye    += vec3(deltaX, deltaY, deltaZ);
	center += vec3(deltaX, deltaY, deltaZ);
	update();
}


GLfloat distance(vec3 a, vec3 b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

void Camera::zoom(GLfloat ratio) {
	GLfloat R = distance(eye, center);
	R /= ratio;
	eye.x = center.x + R * sin(horizontal) * cos(vertical);
	eye.y = center.y + R * sin(vertical);
	eye.z = center.z + R * cos(horizontal) * cos(vertical);
	update();
}

void Camera::rotate(GLfloat dVertical, GLfloat dHorizontal) {
	GLfloat R = distance(eye, center);
	vertical += dVertical;
	horizontal += dHorizontal;
	eye.x = center.x + R * sin(horizontal) * cos(vertical);
	eye.y = center.y + R * sin(vertical);
	eye.z = center.z + R * cos(horizontal) * cos(vertical);
	update();
}

void Camera::update() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, zNear, zFar);
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, 0, 1, 0);
}