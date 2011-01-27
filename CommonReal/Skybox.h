#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/gl.h> 
#include "sdlglutils.h"

class Skybox {
public:
	Skybox(){TexIDSkyBox = new GLuint[6];};
	~Skybox(){};

	void SkyBox_CreateTexture(void);
	void SkyBox_Draw(float x, float y, float z, float width, float height, float length);

private:
	GLuint *TexIDSkyBox;

};



#endif
