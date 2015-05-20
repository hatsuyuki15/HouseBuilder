#include "glText.h"


void RenderText(const TTF_Font *Font, const GLubyte& R, const GLubyte& G, const GLubyte& B, const double& X,
	const double& Y, const double& Z, const std::string& Text)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_Color Color = { R, G, B };
	SDL_Surface *Message = TTF_RenderText_Blended(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
	//SDL_Surface *Message = TTF_RenderText_Solid(const_cast<TTF_Font*>(Font), Text.c_str(), Color);
	if (Message == NULL){
		printf("Can't create surface for string \"%s\" \n", Text);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		return;
	}
	unsigned textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D, 0,  GL_RGBA,
		Message->w, Message->h, 0,
		GL_BGRA_EXT, GL_UNSIGNED_BYTE,
		Message->pixels
	);
	// render text
	glBegin(GL_QUADS);
	glTexCoord2d(0, 1); glVertex3d(X, Y, Z);
	glTexCoord2d(1, 1); glVertex3d(X + Message->w, Y, Z);
	glTexCoord2d(1, 0); glVertex3d(X + Message->w, Y + Message->h, Z);
	glTexCoord2d(0, 0); glVertex3d(X, Y + Message->h, Z);
	glEnd();

	// clean up
	glDeleteTextures(1, &textureID);
	SDL_FreeSurface(Message);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
