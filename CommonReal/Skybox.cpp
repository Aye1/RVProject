#include "Skybox.h"

#define BACK_ID   0
#define FRONT_ID  1
#define BOTTOM_ID 2
#define TOP_ID    3
#define LEFT_ID   4
#define RIGHT_ID  5


void Skybox::SkyBox_CreateTexture(void)
{
	int i=1;

	TexIDSkyBox[0] = loadTexture("Data/back.jpg");
	TexIDSkyBox[1] = loadTexture("Data/front.jpg");
	TexIDSkyBox[2] = loadTexture("Data/bottom.jpg");
	TexIDSkyBox[3] = loadTexture("Data/top.jpg");
	TexIDSkyBox[4] = loadTexture("Data/left.jpg");
	TexIDSkyBox[5] = loadTexture("Data/right.jpg");

	for(i=0;i<=5;++i)
	{
		glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	}
}


void Skybox::SkyBox_Draw(float x, float y, float z, float width, float height, float length)
{
  x = x - width  / 2;  // Calcul l'emplacement d'un coin du cube
  y = y - height / 2;
  z = z - length / 2;

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[BACK_ID]);
  glBegin(GL_TRIANGLE_STRIP);					
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[FRONT_ID]);
  glBegin(GL_TRIANGLE_STRIP);			
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 	
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[BOTTOM_ID]);
  glBegin(GL_TRIANGLE_STRIP);				
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x,y,z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y,	z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,z + length); 	
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[TOP_ID]);
  glBegin(GL_TRIANGLE_STRIP);					
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x,y + height,z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 	
  glEnd();


  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[RIGHT_ID]);
  glBegin(GL_TRIANGLE_STRIP);				
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length); 
  glEnd();

  glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[LEFT_ID]);
  glBegin(GL_TRIANGLE_STRIP);				
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
  glEnd();

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

}
