#ifndef _GL_TEXT_H
#define _GL_TEXT_H

#include "Common.h"
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

void RenderText3D(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B, const double& X,
	const double& Y, const double& Z, const std::string& Text);

void RenderText2D(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B, const double& X,
	const double& Y, const std::string& Text);

#endif