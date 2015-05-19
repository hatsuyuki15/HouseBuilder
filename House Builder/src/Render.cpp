#include <stdio.h>
#include <stdlib.h>
#include "TextureManager.h"
#include "Render.h"

Render::Render() {
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_NORMALIZE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// Somewhere in the initialization part of your program…
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Create light components
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat position[] = { 0, 10, 0, 1.0f };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	camera = Camera::getCamera();
}

void Render::setGridMap(GridMap* map) {
	this->map = map;
}

void Render::renderObj(Obj* obj) {
	if (obj == NULL){
		//log("Obj is NULL");
		return;
	}
	glPushMatrix();
	
	// Rotate
	glRotatef(obj->transform.rotation.x, 1, 0, 0);
	glRotatef(obj->transform.rotation.y, 0, 1, 0);
	glRotatef(obj->transform.rotation.z, 0, 0, 1);
	// Scale
	glScalef(obj->transform.scale.x, obj->transform.scale.y, obj->transform.scale.z);
	// Translate
	glTranslatef(obj->transform.position.x, obj->transform.position.y, obj->transform.position.z);
	//glutWireSphere(10.f, 16, 16);
	Shapes shapes = obj->shapes;	
	for (size_t i = 0; i < shapes.size(); i++) {		
		tinyobj::mesh_t mesh = shapes[i].mesh;
		bool use_texture = false;		

		if (mesh.material_ids.size() > 0 && mesh.material_ids[0] != -1) {			
						
			int material_id = mesh.material_ids[0];			
			tinyobj::material_t material = obj->materials[material_id];
			
			glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, material.emission);
			glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
			
			if (material.diffuse_texname.length() > 0){
				use_texture = true;
				glEnable(GL_TEXTURE_2D);				
				std::string texturePath = obj->resourceFolder + material.diffuse_texname;
				GLuint textureID = TextureManager::getInstance()->loadTexture(texturePath.c_str());
				glBindTexture(GL_TEXTURE_2D, textureID);
			}
			else {
				glDisable(GL_TEXTURE_2D);
			}
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}
		
		int face_nums = mesh.indices.size() / 3;
		for (size_t f = 0; f < face_nums; ++f) {
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; ++j) {
				int idm = mesh.indices[f*3 + j]; // index
				int idv = idm * 3; // vertex index
				int idt = idm * 2; // texture coord
				glNormal3fv(&(mesh.normals[idv]));
				if (use_texture)
					glTexCoord2fv(&(mesh.texcoords[idt]));
				glVertex3fv(&(mesh.positions[idv]));												
			}
			glEnd();
		}
		
	}
	glPopMatrix();
}

void Render::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	//glutWireSphere(10.f, 16, 16);
	renderGridLayout();
	for (int i = 0; i < map->objects.size(); i++) {
		renderObj(map->objects[i]);
	}	
}

void Render::renderGridLayout() {
	int cellSize = map->getCellSize();
	glColor3f(1.0, 0.2, 0.2);
	glBegin(GL_LINES);
	int border = cellSize * 50;
	for (int i = -50; i < 50; i++){
		glVertex3d(cellSize*i, 0, border);
		glVertex3d(cellSize*i, 0, -border);
		glVertex3d(border, 0, cellSize*i);
		glVertex3d(-border, 0, cellSize*i);
	}
	glEnd();
}