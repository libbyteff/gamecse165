
/*
Resources:
	1. Learn Open GL
	2. Youtube (videos related to soil library, how to start opengl, etc.)
	3. Geek for Geeks
	4. Matsson
	5. Github
	6. https://cs.lmu.edu/~ray/notes/openglexamples/
*/

#include <windows.h>
#include <GL/glut.h> ///library that was used glut
#include <SOIL/SOIL.h> //library that was used soil
#include <iostream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <sstream>
#include <fstream>
using namespace std;

#define MIN_DST 500 //the elements that will be battling again each other
#define MAX_DST 800
#define MIN_HEIGHT 55	//the height of the element				 
#define MAX_HEIGHT 135 

double a=30, b, elementvelo, velocityPlayer, maxVelocity ;
double elemental[20], timediff ;
double separator;
bool over ;
int doneresults=0, recentElement ;
int waterballCount ;
int hs ;
clock_t oldTime, newTime ;
bool flag = false ;

// Converts int to string
string tostring(int j)
{
	stringstream ss;
	string s;
	ss<<j;
	ss>>s;
	return s;       
}

GLuint background, waterball, fireball, go, wind; // here is where all the ui is collected 
void loadTextures(){							//Gluit

	background = SOIL_load_OGL_texture
		(
		"userinterface/greytexture.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	waterball = SOIL_load_OGL_texture
		(
		"userinterface/waterball.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	
	wind = SOIL_load_OGL_texture
		(
		"userinterface/wind.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	fireball = SOIL_load_OGL_texture
		(
		"userinterface/fireball.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	go = SOIL_load_OGL_texture
		(
		"userinterface/elements.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
}

void initialize()  //parameters
{
	oldTime = clock() ;
	elemental[0] = 600;
	velocityPlayer = 0 ;
	elementvelo = 240 ;
	maxVelocity = 240;
	flag = false ;
	waterballCount = 0 ;
	recentElement = 0 ;
	separator = 640 ;
	b = MIN_HEIGHT ;
	over = 0 ;
}


void myInit()   ///the window being initializing
{
	glClearColor(1.0,4.0,6.0,0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
	loadTextures();
	initialize();
}


void createbackground(){ //
	separator-= elementvelo * timediff;
	if(separator<=0)
		separator+= 640 ;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, background);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor4f(1.0,1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(separator-640,0);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(separator,0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(separator,480);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(separator-640,480);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(separator,0);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(separator+640,0);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(separator+640,480);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(separator,480);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void freshElement() //adding a new element
{
	elemental[recentElement+1] = elemental[recentElement] + ( ((double)rand()/RAND_MAX)*(MAX_DST-MIN_DST) ) + MIN_DST ;
	recentElement++ ;
}


void detect()    // this is where the collision occurs (using the function detect)
{
	if( (abs(a-elemental[0])<25 && b<75) || (abs(a-elemental[1])<25 && b<75) )
	{
		
		over=true;
		doneresults=waterballCount;
		hs=max(hs,doneresults);
	
	}
} 
 
void disp() //
{
	glClear(GL_COLOR_BUFFER_BIT);

	if(!over)
	{	
		newTime = clock() ;
		timediff = ( (double)(newTime-oldTime)/CLOCKS_PER_SEC ) ;
		oldTime = newTime ; 

		createbackground();

		b+= velocityPlayer * timediff ;
		if(b>=MAX_HEIGHT){
			velocityPlayer = -1 * maxVelocity ;
			flag = false ;
		}
		if(b<=MIN_HEIGHT) {
			b = MIN_HEIGHT ;
			if(flag) {
				velocityPlayer = maxVelocity ;
				
			}
			else
				velocityPlayer = 0 ;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, fireball);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		

		glBegin(GL_POLYGON);
		glTexCoord2d(0.0, 0.0);
		glVertex2d(a-25, b-25);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(a-25, b+25);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(a+25, b+25);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(a+25, b-25);
		glEnd();

		double changePos = elementvelo * timediff ;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, waterball);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for(int i=0; i<=recentElement; i++) {	
			elemental[i]-= changePos ;
			double pos = elemental[i] ;

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glMatrixMode(GL_MODELVIEW);


			glLoadIdentity();
			glBegin(GL_POLYGON);
			glColor4f(1.0,1.0,1.0,1.0);

			glTexCoord2d(0.0, 0.0);
			glVertex2d(pos, 30);

			glTexCoord2d(0.0, 1.0);
			glVertex2d(pos, 50);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(pos+20, 50);

			glTexCoord2d(1.0, 0.0);
			glVertex2d(pos+20, 30);
			glEnd();
		}    
		glDisable(GL_TEXTURE_2D);  

		detect();

		glRasterPos2f(500, 400);
		string score=" ";
		score+=tostring(waterballCount);
		int len, i;
		len = score.size();
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , score[i]);
		}

		glFlush();

		if(elemental[recentElement]<640)
		{
			freshElement();
		}

		if(elemental[0]<-21) {
			for(int d=0 ; d<recentElement ; d++) 
				elemental[d] = elemental[d+1] ;
			recentElement-- ; // the recent element will disappear
			waterballCount++ ; //every element will be added once the previous on disappers
			if(waterballCount%5 == 0) {
				elementvelo += 10;
				maxVelocity += 15;
			}
		}
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, go);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0,0);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(640,0);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(640,480);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0,480);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		int len, i;

		string SC="   ";
		SC+=tostring(doneresults);
		len = SC.size();
		glRasterPos2f(280-len/2, 260);

		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , SC[i]);
		}

		string HC="    ";
		HC+=tostring(hs);
		len = HC.size();
		glRasterPos2f(280-len/2, 240);
		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , HC[i]);
		}
		string NG="  ";
		len = NG.size();
		glRasterPos2f(280-len/2, 220);

		for (i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , NG[i]);
		}
	}
	glFlush();
}


void specialKeyFunc(int k, int x, int y)
{
	if(!over && k==GLUT_KEY_UP) //the controller (the up button)
		flag = true ;
}

void keyFunc(unsigned char k, int x, int y) //choosing a key to restart the game
{
	if(k=='n' || k=='N')
	{
		initialize();
		glutPostRedisplay();
	}
}


void idleFunc()  //re-opening window content
{
	glutPostRedisplay();
}


int main(int argc, char **argv) // this main function helps with our window
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 675);
	glutInitWindowPosition(0, 0);           
	glutCreateWindow("Rural Skateboarder"); //name of the  created window
	glutDisplayFunc(disp);
	glutIdleFunc(idleFunc) ;
	glutSpecialFunc(specialKeyFunc);
	glutKeyboardFunc(keyFunc);
	myInit();
	glutMainLoop();
	return 0;
}
