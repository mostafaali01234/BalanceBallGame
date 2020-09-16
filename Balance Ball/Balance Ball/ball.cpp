#define STB_IMAGE_IMPLEMENTATION
#include<Windows.h>
#include<iostream>
#include<cmath>
#include<gl/glut.h>
#include <stb_image.h>
using namespace std;


unsigned int texture;
int width, height, nrChannels;
unsigned char *dat1a = NULL;

//light properties
float lightamb[] = { 0.6,0.6,0.6,1.0 },	          //Ambient
												  //float lightamb[] = { 1.0,1.0,1.0,1.0 },	          //Ambient Max Intinstey
	lightdiff[] = { 0,.6,.6,1 },			          //Diffuse
	lightspec[] = { .2,.2,.2,1 };		              //Specualer
float lightpos1[] = { -5.0,20.0,-2.0, 0.0 };     //light Position in The Level 1
float lightpos2[] = { 20 , 25.0 , -20.0, 0.0 };	  //light Position in The Level 2

bool fullScreen;
int wScreen, hScreen;

bool stopGame = false;

int mov = 0, c = 0;     // Used For Condition To Move a Ball When Release Click Buttons
char type;

float zmov = 0;          // To Move Cube Used in Level 2
int cond = 1;

int drawLevel = 1;     // Change To 2 Whwn Win Level 1
int bk = 1;            // When The Ball Out The Path set bk = 2 To Prevent Move to any side
int x = 1;             // Used When Ball on eighth road
int xx = 1;            // Used When Ball on Down Hill
float v = 0, vv = 0;         // To Move Winner Box down
float x1 = 0.2, x2 = -0.2, y11, y2, y4, y5, z1, z2, z4, z5;          //To Move Cubes When Touch Them
int v1 = 1, v2 = 1, v4 = 1, v5 = 1;
float ratio;
float positiony = 6, positionz = 0, positionx = 17.5, directionx = 17.5, directiony = 5.6, directionz = -1;   // To Determine Position And Direction Of Camera
float XPosBall = 2, YPosBall = -1.4, ZPosBall = -16.5;        // To Set Position of Ball
float xRotated = 0.0, yRotated = 0.0, zRotated = 0.0;         // To Rotate The Ball
float radius = 0.4;

/////////////////////////////// Functions ////////////////////////////

void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void mouse(int, int, int, int);
/******************************************************************************/
/* ball light  */
void ball_light() {
	/****************************************************************************/
	/***************************************/
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float mat_ambient_color[] = { 0.8f, 0.8f, 0.2f, 1.0f };
	float mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
	float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	float no_shininess = 0.0f;
	float low_shininess = 5.0f;
	float mid_shininess = 50.0f;
	float high_shininess = 100.0f;
	float mat_emission[] = { 0.3f, 0.2f, 0.2f, 0.0f };
	/****************************************/
	/************************************************************************/
	glTranslatef(-1.25f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, mid_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);


	/**************************************************************************/
	/******************************************************************************/

}

void background() {
	glClearColor(0.3, 0.35, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//////////////////// Enable Lightting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//////////////////// Set The Properties To light Source #0		
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
	////////////////////// Cheack Light Position over Level
	if (drawLevel == 1)
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
	else if (drawLevel == 2)
		glLightfv(GL_LIGHT0, GL_POSITION, lightpos2);
	//////////////////// Enable Material and normalize for Light Effects
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	glutCreateWindow(" Balance Ball Game ");

	background();
	glutDisplayFunc(mydraw);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	ball_light();//////////////////////////////
	glutMainLoop();
}

void checkTexture(unsigned char *data) {
	if (data) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

void load(int imgnum) {
	if (imgnum == 1) {

		dat1a = stbi_load("p1.jpg", &width, &height, &nrChannels, 0);
		checkTexture(dat1a);
	}
	else if (imgnum == 2) {

		dat1a = stbi_load("p2.jpg", &width, &height, &nrChannels, 0);
		checkTexture(dat1a);
	}
}

void drawText(float x, float y, float z, char* string) {

	glRasterPos3f(x, y, z);                 // Set Position To Text
	int len = (int)strlen(string);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);        // Print Text (Char then Char)
	}
}

void drawLevel1() {

	///////////road//////////////
	///////////////////////Begin Square
	////------------upper face of the big quad
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(22, -2, -5);
	glVertex3f(22, -2, -11);
	glVertex3f(17, -2, -11);
	glVertex3f(17, -2, -5);
	glEnd();
	////------------edges below upper face of big quad
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(22, -3, -5);
	glVertex3f(22, -2, -5);
	glVertex3f(17, -2, -5);
	glVertex3f(17, -3, -5);

	glVertex3f(22, -3, -11);
	glVertex3f(22, -2, -11);
	glVertex3f(17, -2, -11);
	glVertex3f(17, -3, -11);

	glVertex3f(22, -3, -11);
	glVertex3f(22, -2, -11);
	glVertex3f(22, -2, -5);
	glVertex3f(22, -3, -5);

	glVertex3f(17, -3, -11);
	glVertex3f(17, -2, -11);
	glVertex3f(17, -2, -5);
	glVertex3f(17, -3, -5);
	glEnd();

	///////////////////first road////////////////
	////------------upper face of first road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(17, -2, -6.5);
	glVertex3f(17, -2, -9.5);
	glVertex3f(5, -2, -9.5);
	glVertex3f(5, -2, -6.5);
	glEnd();
	////------------edges below first road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);


	glNormal3f(0, -1.0, 0);
	glVertex3f(17, -3, -6.5);
	glVertex3f(17, -2, -6.5);
	glVertex3f(5, -2, -6.5);
	glVertex3f(5, -3, -6.5);

	glVertex3f(17, -3, -9.5);
	glVertex3f(17, -2, -9.5);
	glVertex3f(5, -2, -9.5);
	glVertex3f(5, -3, -9.5);
	glEnd();

	// -------------1st cube

	glTranslatef(7, -1.5, -13.0);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0, 1.0, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, 0.5 + y11, 0.5 + z1);
	glVertex3f(0.5 + x1, 0.5 + y11, 0.5 + z1);

	//botton
	glNormal3f(0, -1.0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.5 + x1, -0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, 0.5 + z1);
	glVertex3f(0.5 + x1, -0.5 + y11, 0.5 + z1);

	//right
	glNormal3f(-1.0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(0.5 + x1, -0.5 + y11, -0.5 + z1);
	glVertex3f(0.5 + x1, -0.5 + y11, 0.5 + z1);
	glVertex3f(0.5 + x1, 0.5 + y11, 0.5 + z1);

	//left
	glNormal3f(1.0, 0, 0);
	glColor3f(1, .5, .5);
	glVertex3f(-0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, 0.5 + z1);
	glVertex3f(-0.5 + x1, 0.5 + y11, 0.5 + z1);

	//back
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, 0.5 + y11, -0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, -0.5 + z1);
	glVertex3f(0.5 + x1, -0.5 + y11, -0.5 + z1);

	//front
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.5 + x1, 0.5 + y11, 0.5 + z1);
	glVertex3f(-0.5 + x1, 0.5 + y11, 0.5 + z1);
	glVertex3f(-0.5 + x1, -0.5 + y11, 0.5 + z1);
	glVertex3f(0.5 + x1, -0.5 + y11, 0.5 + z1);
	glEnd();
	glTranslatef(-7, 1.5, 13.0);

	// -------------2nd cube

	glTranslatef(6, -1.5, -15);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0, 1.0, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, 0.5 + y2, 0.5 + z2);
	glVertex3f(0.5 + x2, 0.5 + y2, 0.5 + z2);

	//botton
	glNormal3f(0, -1.0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.5 + x2, -0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, 0.5 + z2);
	glVertex3f(0.5 + x2, -0.5 + y2, 0.5 + z2);

	//right
	glNormal3f(-1.0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(0.5 + x2, -0.5 + y2, -0.5 + z2);
	glVertex3f(0.5 + x2, -0.5 + y2, 0.5 + z2);
	glVertex3f(0.5 + x2, 0.5 + y2, 0.5 + z2);

	//left
	glNormal3f(1.0, 0, 0);
	glColor3f(1, .5, .5);
	glVertex3f(-0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, 0.5 + z2);
	glVertex3f(-0.5 + x2, 0.5 + y2, 0.5 + z2);

	//back
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, 0.5 + y2, -0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, -0.5 + z2);
	glVertex3f(0.5 + x2, -0.5 + y2, -0.5 + z2);

	//front
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.5 + x2, 0.5 + y2, 0.5 + z2);
	glVertex3f(-0.5 + x2, 0.5 + y2, 0.5 + z2);
	glVertex3f(-0.5 + x2, -0.5 + y2, 0.5 + z2);
	glVertex3f(0.5 + x2, -0.5 + y2, 0.5 + z2);
	glEnd();
	glTranslatef(-6, 1.5, 15);


	//////////////////Second road//////////
	////------------upper face
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(8, -2, -9.5);
	glVertex3f(8, -2, -20);
	glVertex3f(5, -2, -20);
	glVertex3f(5, -2, -9.5);
	glEnd();
	////------------edges below second road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(5, -3, -6.5);
	glVertex3f(5, -2, -6.5);
	glVertex3f(5, -2, -20);
	glVertex3f(5, -3, -20);


	glVertex3f(8, -3, -6.5);
	glVertex3f(8, -2, -6.5);
	glVertex3f(8, -2, -20);
	glVertex3f(8, -3, -20);
	glEnd();
	///////////////////third road////////////////
	////------------upper face of third road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(15, -2, -17);
	glVertex3f(15, -2, -20);
	glVertex3f(8, -2, -20);
	glVertex3f(8, -2, -17);

	glEnd();

	///////////////////bumpy road////////////////
	////------------upper face of bumpy road  ÇáãØÈ
	glBegin(GL_QUADS);
	glColor3f(.8, 0, .1);
	//ÇáãÕÚÏ
	glNormal3f(1.0, 0, 0);
	glVertex3f(9, -2, -17);
	glVertex3f(9.5, -1.5, -17);
	glVertex3f(9.5, -1.5, -20);
	glVertex3f(9, -2, -20);
	///////upper face
	glNormal3f(0, 1.0, 0);
	glVertex3f(10.5, -1.5, -17);
	glVertex3f(10.5, -1.5, -20);
	glVertex3f(9.5, -1.5, -20);
	glVertex3f(9.5, -1.5, -17);
	//////////ÇáãåÈØ
	glNormal3f(-1.0, -1.0, 0);
	glVertex3f(11, -2, -20);
	glVertex3f(10.5, -1.5, -20);
	glVertex3f(10.5, -1.5, -17);
	glVertex3f(11, -2, -17);
	////// below bumpy road
	glNormal3f(0, -1.0, 0);
	glVertex3f(11, -2, -17);
	glVertex3f(10.5, -1.5, -17);
	glVertex3f(9.5, -1.5, -17);
	glVertex3f(9, -2, -17);

	glVertex3f(11, -2, -20);
	glVertex3f(10.5, -1.5, -20);
	glVertex3f(9.5, -1.5, -20);
	glVertex3f(9, -2, -20);

	glEnd();

	////------------edges below third road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(15, -3, -17);
	glVertex3f(15, -2, -17);
	glVertex3f(8, -2, -17);
	glVertex3f(8, -3, -17);

	glVertex3f(12, -3, -20);
	glVertex3f(12, -2, -20);
	glVertex3f(5, -2, -20);
	glVertex3f(5, -3, -20);

	glEnd();

	///////////////////fourth road ( Left )////////////////
	////------------upper face of fourth road
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(12.75, -2, -20);
	glVertex3f(12.75, -2, -25);
	glVertex3f(12, -2, -25);
	glVertex3f(12, -2, -20);
	glEnd();

	////------------edges below fourth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(12.75, -3, -25);
	glVertex3f(12.75, -2, -25);
	glVertex3f(12.75, -2, -17);
	glVertex3f(12.75, -3, -17);

	glVertex3f(12, -3, -25);
	glVertex3f(12, -2, -25);
	glVertex3f(12, -2, -20);
	glVertex3f(12, -3, -20);
	glEnd();

	///////////////////fourth road ( Right )////////////////
	////------------upper face of fourth road
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(15, -2, -20);
	glVertex3f(15, -2, -25);
	glVertex3f(14.25, -2, -25);
	glVertex3f(14.25, -2, -20);
	glEnd();

	////------------edges below fourth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(15, -3, -28);
	glVertex3f(15, -2, -28);
	glVertex3f(15, -2, -17);
	glVertex3f(15, -3, -17);

	glVertex3f(14.25, -3, -28);
	glVertex3f(14.25, -2, -28);
	glVertex3f(14.25, -2, -20);
	glVertex3f(14.25, -3, -20);
	glEnd();

	// -------------- 4th cube 

	glTranslatef(14.6, -1.75, -21);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0, 1.0, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, 0.25 + y4, 0.25 + z4);
	glVertex3f(0.25, 0.25 + y4, 0.25 + z4);

	//botton
	glNormal3f(0, -1.0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.25, -0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, 0.25 + z4);
	glVertex3f(0.25, -0.25 + y4, 0.25 + z4);

	//right
	glNormal3f(-1.0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(0.25, -0.25 + y4, -0.25 + z4);
	glVertex3f(0.25, -0.25 + y4, 0.25 + z4);
	glVertex3f(0.25, 0.25 + y4, 0.25 + z4);

	//left
	glNormal3f(1.0, 0, 0);
	glColor3f(1, .5, .5);
	glVertex3f(-0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, 0.25 + z4);
	glVertex3f(-0.25, 0.25 + y4, 0.25 + z4);

	//back
	glColor3f(1, 1, 0);
	glVertex3f(0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, 0.25 + y4, -0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, -0.25 + z4);
	glVertex3f(0.25, -0.25 + y4, -0.25 + z4);

	//front
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.25, 0.25 + y4, 0.25 + z4);
	glVertex3f(-0.25, 0.25 + y4, 0.25 + z4);
	glVertex3f(-0.25, -0.25 + y4, 0.25 + z4);
	glVertex3f(0.25, -0.25 + y4, 0.25 + z4);
	glEnd();
	glTranslatef(-14.6, 1.75, 21);

	// -------------- 5th cube 

	glTranslatef(12.35, -1.75, -25);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0, 1.0, 0);
	glColor3f(1, 0, 0);
	glVertex3f(0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, 0.25 + y5, 0.25 + z5);
	glVertex3f(0.25, 0.25 + y5, 0.25 + z5);

	//botton
	glNormal3f(0, -1.0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.25, -0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, 0.25 + z5);
	glVertex3f(0.25, -0.25 + y5, 0.25 + z5);

	//right
	glNormal3f(-1.0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(0.25, -0.25 + y5, -0.25 + z5);
	glVertex3f(0.25, -0.25 + y5, 0.25 + z5);
	glVertex3f(0.25, 0.25 + y5, 0.25 + z5);

	//left
	glNormal3f(1.0, 0, 0);
	glColor3f(1, .5, .5);
	glVertex3f(-0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, 0.25 + z5);
	glVertex3f(-0.25, 0.25 + y5, 0.25 + z5);

	//back
	glColor3f(1, 1, 0);
	glVertex3f(0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, 0.25 + y5, -0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, -0.25 + z5);
	glVertex3f(0.25, -0.25 + y5, -0.25 + z5);

	//front
	glColor3f(0.3, 0.4, 0.3);
	glVertex3f(0.25, 0.25 + y5, 0.25 + z5);
	glVertex3f(-0.25, 0.25 + y5, 0.25 + z5);
	glVertex3f(-0.25, -0.25 + y5, 0.25 + z5);
	glVertex3f(0.25, -0.25 + y5, 0.25 + z5);
	glEnd();
	glTranslatef(-12.35, 1.75, 25);


	///////////////////fifth road////////////////
	////------------upper face of fifth road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(15, -2, -25);
	glVertex3f(15, -2, -28);
	glVertex3f(7, -2, -28);
	glVertex3f(7, -2, -25);
	glEnd();

	////------------edges below fifth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);
	glNormal3f(0, -1.0, 0);

	glVertex3f(15, -3, -28);
	glVertex3f(15, -2, -28);
	glVertex3f(10, -2, -28);
	glVertex3f(10, -3, -28);

	glVertex3f(15, -3, -25);
	glVertex3f(15, -2, -25);
	glVertex3f(7, -2, -25);
	glVertex3f(7, -3, -25);

	glEnd();

	///////////////////sixth road////////////////
	////------------upper face of sixth road
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(10, -2, -28);
	glVertex3f(10, -2, -36);
	glVertex3f(7, -2, -36);
	glVertex3f(7, -2, -28);
	glEnd();

	////------------edges below sixth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);
	glNormal3f(0, -1.0, 0);

	glVertex3f(10, -3, -33);
	glVertex3f(10, -2, -33);
	glVertex3f(10, -2, -28);
	glVertex3f(10, -3, -28);

	glVertex3f(7, -3, -36);
	glVertex3f(7, -2, -36);
	glVertex3f(7, -2, -25);
	glVertex3f(7, -3, -25);
	glEnd();

	///////////////////seventh road////////////////
	////------------upper face of seventh road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(15, -2, -33);
	glVertex3f(15, -2, -36);
	glVertex3f(10, -2, -36);
	glVertex3f(10, -2, -33);
	glEnd();

	////------------edges below seventh road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(15, -3, -33);
	glVertex3f(15, -2, -33);
	glVertex3f(10, -2, -33);
	glVertex3f(10, -3, -33);

	glVertex3f(15, -3, -36);
	glVertex3f(15, -2, -36);
	glVertex3f(7, -2, -36);
	glVertex3f(7, -3, -36);

	glEnd();

	///////////////////eighth road////////////////
	////------------upper face of eighth road
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(25, -7, -33);
	glVertex3f(25, -7, -36);
	glVertex3f(15, -2, -36);
	glVertex3f(15, -2, -33);
	glEnd();

	////------------edges below eighth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(25, -8, -33);
	glVertex3f(25, -7, -33);
	glVertex3f(15, -2, -33);
	glVertex3f(15, -3, -33);

	glVertex3f(25, -8, -36);
	glVertex3f(25, -7, -36);
	glVertex3f(15, -2, -36);
	glVertex3f(15, -3, -36);

	glEnd();

	///////////////////nineth road////////////////
	////------------upper face of nineth road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(28, -7, -33);
	glVertex3f(28, -7, -36);
	glVertex3f(25, -7, -36);
	glVertex3f(25, -7, -33);
	glEnd();

	////------------edges below ninth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(28, -7, -33);
	glVertex3f(28, -8, -33);
	glVertex3f(25, -8, -33);
	glVertex3f(25, -7, -33);
	glEnd();

	///////////////////tenth road////////////////
	////------------upper face of tenth road
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(30.5, -7, -27.5);
	glVertex3f(30.5, -7, -36);
	glVertex3f(27.5, -7, -36);
	glVertex3f(27.5, -7, -27.5);
	glEnd();

	////------------edges below tenth road
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(27.5, -7, -27.5);
	glVertex3f(27.5, -8, -27.5);
	glVertex3f(27.5, -8, -36);
	glVertex3f(27.5, -7, -36);
	glEnd();

	///////////////////////End Square
	////------------upper face of the finish quad
	glBegin(GL_QUADS);
	glColor3f(0.173, 0.804, 0.529);

	glNormal3f(0, 1.0, 0);
	glVertex3f(31.5, -7, -22.5);
	glVertex3f(31.5, -7, -27.5);
	glVertex3f(26.5, -7, -27.5);
	glVertex3f(26.5, -7, -22.5);
	glEnd();

	////------------edges below upper face of finish quad
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(31.5, -8, -22.5);
	glVertex3f(31.5, -7, -22.5);
	glVertex3f(26.5, -7, -22.5);
	glVertex3f(26.5, -8, -22.5);

	glVertex3f(31.5, -8, -27.5);
	glVertex3f(31.5, -7, -27.5);
	glVertex3f(31.5, -7, -22.5);
	glVertex3f(31.5, -8, -22.5);

	glVertex3f(26.5, -8, -27.5);
	glVertex3f(26.5, -7, -27.5);
	glVertex3f(26.5, -7, -22.5);
	glVertex3f(26.5, -8, -22.5);
	glEnd();

	//////////////////////////Winner Box
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.5);
	glVertex3f(30, -6.9, -23.5);
	glVertex3f(30, -6.9, -25.5);
	glVertex3f(28, -6.9, -25.5);
	glVertex3f(28, -6.9, -23.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0, 0);
	glVertex3f(29.5, -6.7 + v, -24);
	glVertex3f(29.5, -6.7 + v, -25);
	glVertex3f(28.5, -6.7 + v, -25);
	glVertex3f(28.5, -6.7 + v, -24);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0);
	glVertex3f(29.5, -6.7 + v, -24);
	glVertex3f(29.5, -6.9 + v, -24);
	glVertex3f(28.5, -6.9 + v, -24);
	glVertex3f(28.5, -6.7 + v, -24);

	glVertex3f(28.5, -6.7 + v, -25);
	glVertex3f(28.5, -6.9 + v, -25);
	glVertex3f(28.5, -6.9 + v, -24);
	glVertex3f(28.5, -6.7 + v, -24);

	glVertex3f(29.5, -6.7 + v, -25);
	glVertex3f(29.5, -6.9 + v, -25);
	glVertex3f(29.5, -6.9 + v, -24);
	glVertex3f(29.5, -6.7 + v, -24);

	glEnd();

}

void drawLevel2() {

	//////start qube////////
	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(31.5, 10, -22.5);
	glVertex3f(31.5, 10, -27.5);
	glVertex3f(26.5, 10, -27.5);
	glVertex3f(26.5, 10, -22.5);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(26.5, 10, -22.5);
	glVertex3f(26.5, 9, -22.5);
	glVertex3f(31.5, 9, -22.5);
	glVertex3f(31.5, 10, -22.5);

	glVertex3f(26.5, 10, -22.5);
	glVertex3f(26.5, 9, -22.5);
	glVertex3f(26.5, 9, -27.5);
	glVertex3f(26.5, 10, -27.5);
	glEnd();


	//////first road////////////
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(30, 10, -27.5);
	glVertex3f(30, 10, -37.5);
	glVertex3f(28, 10, -37.5);
	glVertex3f(28, 10, -27.5);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(30, 10, -27.5);
	glVertex3f(30, 9, -27.5);
	glVertex3f(30, 9, -37.5);
	glVertex3f(30, 10, -37.5);

	glVertex3f(28, 10, -22.5);
	glVertex3f(28, 9, -22.5);
	glVertex3f(28, 9, -37.5);
	glVertex3f(28, 10, -37.5);
	glEnd();

	//////second road//////////

	//1st part
	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(27, 10, -37.5);
	glVertex3f(31, 10, -37.5);
	glVertex3f(31, 10, -38.5);
	glVertex3f(27, 10, -38.5);
	glEnd();
	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(27, 10, -37.5);
	glVertex3f(27, 9, -37.5);
	glVertex3f(31, 9, -37.5);
	glVertex3f(31, 10, -37.5);

	//right part
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(30, 10, -38.5);
	glVertex3f(31, 10, -38.5);
	glVertex3f(31, 10, -41.5);
	glVertex3f(30, 10, -41.5);
	glEnd();
	//edges
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, -1.0, 0);
	glVertex3f(30, 10, -38.5);
	glVertex3f(30, 9.5, -38.5);
	glVertex3f(30, 9.5, -41.5);
	glVertex3f(30, 10, -41.5);

	//left part
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(27, 10, -38.5);
	glVertex3f(28, 10, -38.5);
	glVertex3f(28, 10, -41.5);
	glVertex3f(27, 10, -41.5);
	glEnd();
	//edges

	//inner
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, -1.0, 0);
	glVertex3f(28, 10, -41.5);
	glVertex3f(28, 9, -41.5);
	glVertex3f(28, 9, -38.5);
	glVertex3f(28, 10, -38.5);

	//outer
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(27, 10, -37.5);
	glVertex3f(27, 9, -37.5);
	glVertex3f(27, 9, -42.5);
	glVertex3f(27, 10, -42.5);

	//last part
	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(27, 10, -41.5);
	glVertex3f(31, 10, -41.5);
	glVertex3f(31, 10, -42.5);
	glVertex3f(27, 10, -42.5);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, -1.0, 0);
	glVertex3f(30, 10, -41.5);
	glVertex3f(30, 9.5, -41.5);
	glVertex3f(28, 9.5, -41.5);
	glVertex3f(28, 10, -41.5);

	//////Third road////////

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(28, 10, -42.5);
	glVertex3f(30, 10, -42.5);
	glVertex3f(30, 10, -44.5);
	glVertex3f(28, 10, -44.5);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);
	glColor3f(1, 1, 1);

	glNormal3f(0, -1.0, 0);
	glVertex3f(28, 10, -42.5);
	glVertex3f(28, 9, -42.5);
	glVertex3f(28, 9, -44.5);
	glVertex3f(28, 10, -44.5);


	////////* Moving Cube *///////////

	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(27.5, 10, -45 + zmov);
	glVertex3f(30.5, 10, -45 + zmov);
	glVertex3f(30.5, 10, -48 + zmov);
	glVertex3f(27.5, 10, -48 + zmov);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, -1.0, 0);
	glVertex3f(27.5, 10, -45 + zmov);
	glVertex3f(27.5, 9.5, -45 + zmov);
	glVertex3f(30.5, 9.5, -45 + zmov);
	glVertex3f(30.5, 10, -45 + zmov);

	glVertex3f(30.5, 10, -45 + zmov);
	glVertex3f(30.5, 9.5, -45 + zmov);
	glVertex3f(30.5, 9.5, -48 + zmov);
	glVertex3f(30.5, 10, -48 + zmov);

	glVertex3f(27.5, 10, -45 + zmov);
	glVertex3f(27.5, 9.5, -45 + zmov);
	glVertex3f(27.5, 9.5, -48 + zmov);
	glVertex3f(27.5, 10, -48 + zmov);
	glEnd();

	////Middle Box
	glBegin(GL_QUADS);
	glNormal3f(0, 1.0, 0);
	glColor3f(0.839, 0.908, 0.161);
	glVertex3f(28.25, 10.1, -45.75 + zmov);
	glVertex3f(29.75, 10.1, -45.75 + zmov);
	glVertex3f(29.75, 10.1, -47.25 + zmov);
	glVertex3f(28.25, 10.1, -47.25 + zmov);
	glEnd();

	////////Fourth road///////////

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(28, 10, -54.5);
	glVertex3f(30, 10, -54.5);
	glVertex3f(30, 10, -56.5);
	glVertex3f(28, 10, -56.5);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(28, 10, -54.5);
	glVertex3f(28, 9, -54.5);
	glVertex3f(30, 9, -54.5);
	glVertex3f(30, 10, -54.5);

	glVertex3f(30, 10, -54.5);
	glVertex3f(30, 9, -54.5);
	glVertex3f(30, 9, -56.5);
	glVertex3f(30, 10, -56.5);

	glVertex3f(28, 10, -54.5);
	glVertex3f(28, 9, -54.5);
	glVertex3f(28, 9, -56.5);
	glVertex3f(28, 10, -56.5);
	glEnd();

	////////Fifth road///////////

	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(29, 10, -56.5);
	glVertex3f(31, 10, -56.5);
	glVertex3f(31, 10, -58);
	glVertex3f(29, 10, -58);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(30, 10, -56.5);
	glVertex3f(30, 9, -56.5);
	glVertex3f(31, 9, -56.5);
	glVertex3f(31, 10, -56.5);

	glVertex3f(29, 10, -56.5);
	glVertex3f(29, 9, -56.5);
	glVertex3f(29, 9, -58);
	glVertex3f(29, 10, -58);
	glEnd();

	////////downhill//////////

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(31, 10, -56.5);
	glVertex3f(36, 8, -56.5);
	glVertex3f(36, 8, -58);
	glVertex3f(31, 10, -58);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(31, 10, -56.5);
	glVertex3f(31, 9, -56.5);
	glVertex3f(36, 7, -56.5);
	glVertex3f(36, 8, -56.5);
	glEnd();

	////////after downhill/////////

	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(35.5, 8, -55.5);
	glVertex3f(39, 8, -55.5);
	glVertex3f(39, 8, -59);
	glVertex3f(35.5, 8, -59);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(35.5, 8, -55.5);
	glVertex3f(35.5, 7, -55.5);
	glVertex3f(35.5, 7, -59);
	glVertex3f(35.5, 8, -59);

	glVertex3f(35.5, 8, -55.5);
	glVertex3f(35.5, 7, -55.5);
	glVertex3f(39, 7, -55.5);
	glVertex3f(39, 8, -55.5);

	glVertex3f(39, 8, -55.5);
	glVertex3f(39, 7, -55.5);
	glVertex3f(39, 7, -59);
	glVertex3f(39, 8, -59);
	glEnd();

	/////////thin road////////

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);

	glNormal3f(0, 1.0, 0);
	glVertex3f(37, 8, -59);
	glVertex3f(37.5, 8, -59);
	glVertex3f(37.5, 8, -67);
	glVertex3f(37, 8, -67);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(37, 8, -59);
	glVertex3f(37, 7, -59);
	glVertex3f(37, 7, -67);
	glVertex3f(37, 8, -67);

	glVertex3f(37.5, 8, -59);
	glVertex3f(37.5, 7, -59);
	glVertex3f(37.5, 7, -67);
	glVertex3f(37.5, 8, -67);
	glEnd();

	////////Final//////////

	glBegin(GL_QUADS);
	glColor3f(0.196, .0196, 0.227);

	glNormal3f(0, 1.0, 0);
	glVertex3f(35.75, 8, -67);
	glVertex3f(38.75, 8, -67);
	glVertex3f(38.75, 8, -70);
	glVertex3f(35.75, 8, -70);
	glEnd();

	//edges
	glBegin(GL_QUADS);
	glColor3f(0.0039, 0.0039, 0.0039);

	glNormal3f(0, -1.0, 0);
	glVertex3f(35.75, 8, -67);
	glVertex3f(35.75, 7, -67);
	glVertex3f(38.75, 7, -67);
	glVertex3f(38.75, 8, -67);

	glVertex3f(38.75, 8, -67);
	glVertex3f(38.75, 7, -67);
	glVertex3f(38.75, 7, -70);
	glVertex3f(38.75, 8, -70);

	glVertex3f(35.75, 8, -67);
	glVertex3f(35.75, 7, -67);
	glVertex3f(35.75, 7, -70);
	glVertex3f(35.75, 8, -70);
	glEnd();

	////////////////// Winner Box 2
	glBegin(GL_QUADS);
	glColor3f(0.3, .2, 0.5);
	glVertex3f(38.0, 8.03, -67.75);
	glVertex3f(38.0, 8.03, -69.25);
	glVertex3f(36.5, 8.03, -69.25);
	glVertex3f(36.5, 8.03, -67.75);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glVertex3f(37.75, 8.2 + vv, -68.0);
	glVertex3f(37.75, 8.2 + vv, -69.0);
	glVertex3f(36.75, 8.2 + vv, -69.0);
	glVertex3f(36.75, 8.2 + vv, -68.0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0.5, 0.2, 0);
	glVertex3f(37.75, 8.2 + vv, -68.0);
	glVertex3f(37.75, 8.1 + vv, -68.0);
	glVertex3f(36.75, 8.1 + vv, -68.0);
	glVertex3f(36.75, 8.2 + vv, -68.0);

	glVertex3f(36.75, 8.2 + vv, -69.0);
	glVertex3f(36.75, 8.1 + vv, -69.0);
	glVertex3f(36.75, 8.1 + vv, -68.0);
	glVertex3f(36.75, 8.2 + vv, -68.0);

	glVertex3f(37.75, 8.2 + vv, -69.0);
	glVertex3f(37.75, 8.1 + vv, -69.0);
	glVertex3f(37.75, 8.1 + vv, -68.0);
	glVertex3f(37.75, 8.2 + vv, -68.0);
	glEnd();

}

void drawTextsFinal() {

	load(1);

	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	//glColor3f(0.2588, 0.490196, 0.917647);
	glTexCoord2d(0.0f, 0.0f);  glVertex3f(33.0, 24, -65);
	glTexCoord2d(1.0f, 0.0f);  glVertex3f(37.5, 24, -65);
	glTexCoord2d(1.0f, 1.0f);  glVertex3f(37.5, 28, -65);
	glTexCoord2d(0.0f, 1.0f);  glVertex3f(33.0, 28, -65);
	glEnd();

	/////////// Call Fuction To Draw Texts //////////

	glColor3f(0.0, 0.0, 0.0);                            // Set Color To Text
	char text1[] = { "Replay" };            // Initialize Text
	char text2[] = { "Exit" };

	drawText(34.9, 27.0, -64, text1);                      //Call Fuction
	drawText(35.0, 26.0, -64, text2);                      //Call Fuction

}

void drawTextsWhenStop() {

	load(1);

	glBegin(GL_QUADS);
	//glColor3f(0.2588, 0.490196, 0.917647);
	glColor3f(1.0, 1.0, 1.0);

	glTexCoord2d(0.0f, 0.0f);   glVertex3f(positionx - 2, positiony - 4.5, positionz);
	glTexCoord2d(1.0f, 0.0f);   glVertex3f(positionx + 2, positiony - 4.5, positionz);
	glTexCoord2d(1.0f, 1.0f);   glVertex3f(positionx + 2, positiony - 1.5, positionz);
	glTexCoord2d(0.0f, 1.0f);   glVertex3f(positionx - 2, positiony - 1.5, positionz);
	glEnd();

	/////////// Call Fuction To Draw Texts //////////

	glColor3f(0.0, 0.0, 0.0);                            // Set Color To Text
	char text1[] = { "New Game" };
	char text2[] = { "Resum" };            // Initialize Text
	char text3[] = { "Exit" };

	drawText(positionx - 0.3, positiony - 2.5, positionz + 0.1, text1);          //Call Fuction
	drawText(positionx - 0.2, positiony - 3, positionz + 0.1, text2);
	drawText(positionx - 0.1, positiony - 3.5, positionz + 0.1, text3);

}

void mydraw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(positionx, positiony, positionz, directionx, directiony, directionz, 0, 1, 0);

	glTranslatef(0, 0, -7);

	/////////////////////////////////////////////////////

	load(2);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f);       glVertex3f(-50, -80, -70);
	glTexCoord2d(1.0f, 0.0f);       glVertex3f(80, -80, -70);
	glTexCoord2d(1.0f, 1.0f);       glVertex3f(80, 50, -70);
	glTexCoord2d(0.0f, 1.0f);       glVertex3f(-50, 50, -70);
	glEnd();


	/////////////////////////////////////////////////////

	if (drawLevel == 1) {
		drawLevel1();
	}
	else if (drawLevel == 2) {
		drawLevel2();
	}
	else if (drawLevel == 3) {
		drawTextsFinal();
	}

	if (stopGame) {
		drawTextsWhenStop();
	}

	/////////////////////////////
	//////////////////Ball
	glLoadIdentity();

	glTranslatef(XPosBall, YPosBall, ZPosBall);
	// Red color used to draw.
	//glColor3f(0.9, 0.3, 0.2);
	glColor3f(0.2745, 0.5098, 0.7058823529);  //for blue steel color
											  // rotation about X axis
	glRotatef(xRotated, 1.0, 0.0, 0.0);
	// rotation about Y axis
	glRotatef(yRotated, 0.0, 1.0, 0.0);
	// rotation about Z axis
	glRotatef(zRotated, 0.0, 0.0, 1.0);

	// scaling transfomation 
	glScalef(1.5, 1.5, 1.5);
	// built-in (glut library) function , draw you a sphere.
	glutSolidSphere(radius, 20, 20);

	// swap buffers called because we are using double buffering 
	glutSwapBuffers();

}

void reshape(int w, int h) {
	wScreen = w;
	hScreen = h;
	if (h == 0) h = 1;
	ratio = w / (float)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void check(float x1, float x2, float z1, float z2, float y1, float y2) {

	if (positionx >= x1 && positionx <= x2 && positionz >= z1 && positionz <= z2 && positiony >= y1 && positiony <= y2) {
		YPosBall -= 0.08;
		bk = 2;            // When The Ball Out The Path set bk = 2 To Prevent Move to any side
	}
}

void checkWin(float x1, float x2, float z1, float z2, float y1, float y2) {

	if (positionx >= x1 && positionx <= x2 && positionz >= z1 && positionz <= z2 && positiony >= y1 && positiony <= y2) {
		v = -0.18;
		if (YPosBall < -1.5) {
			YPosBall += .005;
			positiony += .09;
			directiony += .09;
			bk = 2;            // When The Ball Out The Path set bk = 2 To Prevent Move to any side
		}
		else if (YPosBall >= -1.5) {
			bk = 1;
			positiony = 18.2;
			directiony = 17.8;
			drawLevel = 2;     // To Remove Level 1 And Draw Level 2
		}
	}
}

void checkWin2(float x1, float x2, float z1, float z2, float y1, float y2) {

	if (positionx >= x1 && positionx <= x2 && positionz >= z1 && positionz <= z2 && positiony >= y1 && positiony <= y2) {
		vv = -0.19;
		if (YPosBall < -1.2) {
			YPosBall += .005;
			positiony += .09;
			directiony += .09;
			bk = 2;            // When The Ball Out The Path set bk = 2 To Prevent Move to any side
		}
		else if (YPosBall >= -1.2) {
			drawLevel = 3;     // To Remove Level 1 And Draw Level 2
		}
	}
}

void TouchCube() {
	//////// This Function Used To Move Cubes When Touch Them ////////

	//////////////////////////////// Cube 1 && Cube 2 //////////////////////////////
	if (positionx >= 4.1 && positionx <= 4.7 && positionz >= -7.1 && positionz <= -6.1 && v1 == 3 && v2 == 1) {
		z2 = -1;
		v2 = 2;
		z1 = -3;
		v1 = 4;
	}
	else if (positionx >= 4.1 && positionx <= 4.7 && positionz >= -8.1 && positionz <= -7.1 && v1 == 4 && v2 == 2) {
		z2 = -2;
		v2 = 3;
		z1 = -4;
		v1 = 5;
	}
	else if (positionx >= 4.1 && positionx <= 4.7 && positionz >= -9.1 && positionz <= -8.1 && v1 == 5 && v2 == 3) {
		z2 = -3;
		v2 = 4;
		z1 = -5;
		v1 = 6;
	}
	else if (positionx >= 4.1 && positionx <= 4.7 && positionz >= -10.1 && positionz <= -9.1 && v1 == 6 && v2 == 4) {
		z2 = -4;
		v2 = 5;
		z1 = -6;
		v1 = 7;
	}
	else if (positionx >= 4.1 && positionx <= 4.7 && positionz >= -11.1 && positionz <= -10.1 && v1 == 7 && v2 == 5) {
		z2 = -5;
		y2 -= 0.08;
		z1 = -7;
		y11 -= 0.08;
		v1 = 8;
		v2 = 6;
	}
	else if (y2 < 0.0 && y2 > -10.0 && y11 < 0.0 && y11 > -10.0) {
		y2 -= 0.08;
		y11 -= 0.08;
	}

	//////////////////////////////// Cube 1 //////////////////////////////
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -5.1 && positionz <= -4.1 && v1 == 1) {
		z1 = -1;
		v1 = 2;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -6.1 && positionz <= -5.1 && v1 == 2) {
		z1 = -2;
		v1 = 3;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -7.1 && positionz <= -6.1 && v1 == 3) {
		z1 = -3;
		v1 = 4;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -8.1 && positionz <= -7.1 && v1 == 4) {
		z1 = -4;
		v1 = 5;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -9.1 && positionz <= -8.1 && v1 == 5) {
		z1 = -5;
		v1 = 6;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -10.1 && positionz <= -9.1 && v1 == 6) {
		z1 = -6;
		v1 = 7;
	}
	else if (positionx >= 4.3 && positionx <= 6.0 && positionz >= -11.1 && positionz <= -10.1 && v1 == 7) {
		z1 = -7;
		y11 -= 0.08;
		v1 = 8;
	}
	else if (y11 < 0.0 && y11 > -10.0) {
		y11 -= 0.08;
	}

	//////////////////////////////// Cube 2 //////////////////////////////
	else if (positionx >= 2.0 && positionx <= 4.7 && positionz >= -7.1 && positionz <= -6.1 && v2 == 1) {
		z2 = -1;
		v2 = 2;
	}
	else if (positionx >= 2.0 && positionx <= 4.7 && positionz >= -8.1 && positionz <= -7.1 && v2 == 2) {
		z2 = -2;
		v2 = 3;
	}
	else if (positionx >= 2.0 && positionx <= 4.7 && positionz >= -9.1 && positionz <= -8.1 && v2 == 3) {
		z2 = -3;
		v2 = 4;
	}
	else if (positionx >= 2.0 && positionx <= 4.7 && positionz >= -10.1 && positionz <= -9.1 && v2 == 4) {
		z2 = -4;
		v2 = 5;
	}
	else if (positionx >= 2.0 && positionx <= 4.7 && positionz >= -11.1 && positionz <= -10.1 && v2 == 5) {
		z2 = -5;
		y2 -= 0.08;
		v2 = 6;
	}
	else if (y2 < 0.0 && y2 > -10.0) {
		y2 -= 0.08;
	}


	//////////////////////////////// Cube 4 //////////////////////////////
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -13.5 && positionz <= -12.5 && v4 == 1) {
		z4 = -1.0;
		v4 = 2;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -14.5 && positionz <= -13.5 && v4 == 2) {
		z4 = -2.0;
		v4 = 3;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -15.5 && positionz <= -14.5 && v4 == 3) {
		z4 = -3.0;
		v4 = 4;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -16.5 && positionz <= -15.5 && v4 == 4) {
		z4 = -4.0;
		v4 = 5;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -17.5 && positionz <= -16.5 && v4 == 5) {
		z4 = -5.0;
		v4 = 6;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -18.5 && positionz <= -17.5 && v4 == 6) {
		z4 = -6.0;
		v4 = 7;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -19.5 && positionz <= -18.5 && v4 == 7) {
		z4 = -7.0;
		v4 = 8;
	}
	else if (positionx >= 12.0 && positionx <= 13.0 && positionz >= -20.5 && positionz <= -19.5 && v4 == 8) {
		z4 = -8.0;
		y4 -= 0.08;
	}
	else if (y4 < 0.0 && y4 > -10.0) {
		y4 -= 0.08;
	}

	//////////////////////////////// Cube 5 //////////////////////////////

	else if (positionx >= 9.0 && positionx <= 11.0 && positionz >= -17.4 && positionz <= -16.4 && v5 == 1) {
		z5 = -1.0;
		v5 = 2;
	}
	else if (positionx >= 9.0 && positionx <= 11.0 && positionz >= -18.4 && positionz <= -17.4 && v5 == 2) {
		z5 = -2.0;
		v5 = 3;
	}
	else if (positionx >= 9.0 && positionx <= 11.0 && positionz >= -19.4 && positionz <= -18.4 && v5 == 3) {
		z5 = -3.0;
		v5 = 4;
	}
	else if (positionx >= 9.0 && positionx <= 11.0 && positionz >= -20.4 && positionz <= -19.4 && v5 == 4) {
		z5 = -4.0;
		y5 -= 0.08;
	}
	else if (y5 < 0.0 && y5 > -10.0) {
		y5 -= 0.08;
	}

}

void timer(int v) {

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);

	if (stopGame == false) {  // if stopGame equal true don't move or check

							  /////////////////////////////////////////// Check Level 1 /////////////////////////////////////////////////
							  ///////////// Begin Square ////////////
		check(20.2, 30, -3.7, 2.9, 5.5, 6.5);     //right

		check(10, 14.75, -3.7, -2.1, 5.5, 6.5);     //left_top
		check(10, 14.75, 1.3, 2.9, 5.5, 6.5);       //left_down

		check(14.5, 20.2, -15.0, -3.5, 5.5, 6.5);   //top
		check(14.5, 20.2, 2.7, 15.0, 5.5, 6.5);     //down

													//////////////// Fisrt Road //////////////////
		check(2.6, 14.5, 1.1, 10.0, 5.5, 6.5);      //top
		check(6.0, 14.5, -9.3, -1.9, 5.5, 6.5);     //down

													//////////////// Second Road //////////////////
		check(-6.0, 2.8, -12.5, 1.1, 5.5, 6.5);       //left

													  //////////////// Third Road //////////////////
		check(2.8, 9.9, -17.4, -12.5, 5.5, 6.5);      //top

													  //////////////// Fourth Road //////////////////
		check(13.0, 20.0, -21.0, -9.0, 5.5, 6.5);     //right

													  //////////////// Hole //////////////////
		check(10.9, 12.1, -17.4, -12.5, 5.5, 6.5);    //All Sides

													  //////////////// Fifth Road //////////////////
		check(8.0, 13.0, -25.4, -20.4, 5.5, 6.5);     //top

													  //////////////// Sixth Road //////////////////
		check(-4.0, 4.6, -29.0, -17.2, 5.5, 6.5);     //left

													  //////////////// Seventh Road //////////////////
		check(4.0, 13.0, -35.0, -28.5, 5.5, 6.5);     //top

													  //////////////// Eighth Road //////////////////
		check(13, 22.8, -35.0, -28.7, 2.0, 6.0);     //top
		check(13, 22.8, -25.5, -15.0, 2.0, 6.0);     //down

													 //////////////// Nineth Road //////////////////
		check(22.0, 28.0, -35.0, -28.3, 1.8, 2.2);     //top
		check(22.0, 25.2, -25.5, -20.0, 1.8, 2.2);     //down

													   //////////////// Tenth Road //////////////////
		check(28.6, 32.0, -28.0, -20.0, 1.8, 2.2);     //right

													   ///////////// End Square ////////////
		check(18.2, 24.2, -20.5, -14.5, 1.8, 2.2);     //left
		check(29.6, 38.0, -20.5, -14.5, 1.8, 2.2);     //right
		check(24.0, 30.0, -15.0, -10.0, 1.8, 2.2);     //down

													   /////////////// BoxWin /////////////
		checkWin(26.6, 27.4, -17.2, -16.8, 1.8, 20.0);


		/////////////////////////////////////////// Check Level 2 /////////////////////////////////////////////////

		///////////// Start Cube //////////////
		check(18.2, 24.2, -20.5, -14.5, 17.8, 18.4);     //left
		check(29.6, 38.0, -20.5, -14.5, 17.8, 18.4);     //right
		check(24.0, 30.0, -15.0, -10.0, 17.8, 18.4);     //down

														 /////////// First Road /////////////////
		check(27.8, 31.5, -29.6, -19.6, 17.8, 18.4);	   //right
		check(22, 26, -29.6, -19.6, 17.8, 18.4);		  //left

														  /////////// Second Road ////////////////
		check(18.0, 25, -35, -29.3, 17.8, 18.4);      // left outer hole 
		check(29, 35, -35, -29.3, 17.8, 18.4);        // right outer hole 
		check(26, 27.9, -33.5, -30.6, 17.8, 18.4);   // the hole

													 //////////// station ///////////////////
		check(27.8, 31.5, -36.4, -34.6, 17.8, 18.4);	  //right
		check(22, 26, -36.4, -34.6, 17.8, 18.4);		  //left

														  /////////// moving cube ///////////////////
		check(28.4, 31.5, -45.9, -36.9, 17.8, 18.4);	  //right
		check(22, 25.4, -45.9, -36.9, 17.8, 18.4);	     //left

														 //////////// Fourth Road ///////////////////
		check(27.9, 31.5, -47.7, -46.5, 17.8, 18.4);	  //right
		check(22, 26, -48.2, -46.5, 17.8, 18.4);		  //left
		check(22, 26.9, -50.9, -48.6, 17.8, 18.4);	      //top

														  //////////// Fifth Road ////////////////
		check(26.0, 29.5, -55.0, -50.0, 17.8, 18.4);     //Up

														 //////////// Down Hill /////////////////
		check(29.0, 33.3, -55.0, -50.0, 16.0, 17.8);     //Up
		check(29.0, 33.3, -48.6, -45.0, 16.0, 17.8);     //Down

														 /////////// after Down Hill /////////////
		check(37, 40, -50.5, -47.4, 16.4, 17);	       //right
		check(33.2, 36.9, -47.4, -40, 16.4, 17);	   // behind

													   ///////////// thin road //////////////
		check(35.3, 39, -58.7, -50.9, 16.4, 17);	   //right
		check(30, 34.8, -58.7, -50.9, 16.4, 17);	   //left

													   ////////////// final ////////////////
		check(36.6, 39, -62, -58.7, 16.4, 17);       //right
		check(29, 33.6, -62, -58.7, 16.4, 17);       //left
		check(33.6, 36.6, -62, -61.8, 16.4, 17);     //top

													 /////////////// BoxWin 2 /////////////
		checkWin2(34.5, 35.5, -61.0, -60.0, 16.4, 40.0);



		///////////////////////// When The Ball Fall Down (Level 1) ////////////////////////
		if (YPosBall <= -7 && drawLevel == 1) {         //Reset All Things
			positionx = 17.5, positiony = 6, positionz = 0, directionx = 17.5, directiony = 5.6, directionz = -1;
			XPosBall = 2, YPosBall = -1.4, ZPosBall = -16.5;
			x1 = 0.2, x2 = -0.2, y11 = y2 = y4 = y5 = z1 = z2 = z4 = z5 = 0.0;
			v1 = v2 = v4 = v5 = 1;
			x = 1;
			bk = 1;
		}
		///////////////////////// When The Ball Fall Down (Level 2) ////////////////////////
		else if (YPosBall <= -7.0 && drawLevel == 2) {         //Reset All Things
			positionx = 27.0, positiony = 18.2, positionz = -17.0, directionx = 27.0, directiony = 17.8, directionz = -18.0;
			XPosBall = 2, YPosBall = -1.49999, ZPosBall = -16.5;
			zmov = 0;
			cond = 1;
			xx = 1;
			bk = 1;
		}

		///////////////////////// When The Ball on Down Hill (eighth road) In Level 1 ////////////////////////////
		if (positionx >= 12.9 && positionx <= 22.8 && positionz >= -28.7 && positionz <= -24.0 && x == 1) {
			YPosBall -= .02;
			positiony -= .08;
			directiony -= .08;
			positionx += .7*cos(5.0);
			directionx += .7*cos(5.0);
			zRotated -= 6;
		}
		else if (positionx > 22.8 && positionx < 27.0 && positionz >= -28.7 && positionz <= -24.0 && x == 1) {
			positionx += .55*cos(5.0);
			directionx += .55*cos(5.0);
			zRotated -= 6;
		}
		else if (positionx >= 27.0 && positionz >= -28.7 && positionz <= -24.0) {
			x = 2;
		}
		else if (positionx >= 13.5 && positionx <= 20.5 && positionz >= -28.7 && positionz <= -24.0 && x == 2) {
			x = 1;
		}

		///////////////////////// When The Ball on Down Hill In Level 2 //////////////////////////
		if (positionx >= 29.0 && positionx <= 33.5 && positionz >= -50.0 && positionz <= -48.5 && xx == 1) {
			YPosBall -= .02;
			positiony -= .06;
			directiony -= .06;
			positionx += .7*cos(5.0);
			directionx += .7*cos(5.0);
			zRotated -= 6;
		}
		else if (positionx > 33.5 && positionx < 35.0 && positionz >= -50.0 && positionz <= -48.5 && xx == 1) {
			positionx += .4*cos(5.0);
			directionx += .4*cos(5.0);
			zRotated -= 6;
		}
		else if (positionx >= 35.0 && positionz >= -50.0 && positionz <= -48.5) {
			positiony = 16.82;
			directiony = 16.42;
			YPosBall = -1.96;
			xx = 2;
		}
		else if (positionx >= 30.0 && positionx <= 32.0 && positionz >= -50.0 && positionz <= -48.5 && xx == 2) {
			xx = 1;
		}


		//////////////////////////// This Code For Move Cube (Flying Cube) In Level 2 ///////////////////////////
		if (positionx >= 26.4 && positionx <= 27.4 && positionz > -44.8 && positionz <= -38.4 && positiony >= 18.0 && positiony <= 18.4 && cond == 1) {
			zmov += -0.025;
			positionz -= .09*cos(5.0);
			directionz -= .09*cos(5.0);
			bk = 2;
		}
		else if (positionx >= 26.4 && positionx <= 27.4 && positionz >= -45.0 && positionz <= -44.8 && positiony >= 18.0 && positiony <= 18.4 && cond == 1) {
			bk = 1;
			cond = 2;
		}
		else if (positionx >= 26.4 && positionx <= 27.4 && positionz > -44.8 && positionz <= -38.4 && positiony >= 18.0 && positiony <= 18.4 && cond == 2) {
			zmov += 0.025;
			positionz += .09*cos(5.0);
			directionz += .09*cos(5.0);
			bk = 2;
		}
		else if (positionx >= 26.4 && positionx <= 27.4 && positionz >= -38.4 && positionz <= -36.0 && positiony >= 18.0 && positiony <= 18.4 && cond == 2) {
			bk = 1;
			cond = 1;
		}


		////////////////// This Code For Move a Ball When Release Click Buttons ////////////////
		mov = 2;
		if (mov == 2 && c <= 50) {
			++c;
			// This Condtion For Box Winer In Level 1 To Avoid Some Problems
			if (positionx >= 26.2 && positionx <= 27.8 && positionz >= -17.6 && positionz <= -16.4 && positiony >= 1.8 && positiony <= 10.0);
			// This Condtion For cube Flying To Avoid Some Problems
			else if (positionx >= 25.0 && positionx <= 29.0 && positionz >= -45.0 && positionz <= -38.0 && positiony >= 18.0 && positiony <= 18.4);
			// This Condtion For Down Hill To Avoid Some Problems
			else if (positionx >= 28.0 && positionx <= 34.3 && positionz >= -50.0 && positionz <= -48.5 && positiony >= 16.0 && positiony <= 17.8);
			// This Condtion For Box Winer In Level 2 To Avoid Some Problems
			else if (positionx >= 34.6 && positionx <= 35.8 && positionz >= -62.0 && positionz <= -60.0 && positiony >= 16.4 && positiony <= 17.0);
			else {
				if (type == 'r') {  // For Right
									// This Condtion For Bumpy Road To Avoid Some Problems
					if (positionx >= 6.2 && positionx <= 9.4 && positionz >= -12.5 && positionz <= -9);
					else {
						// To Move Ball With Specific Distance When Don't Click Any Buttons
						positionx += .04*cos(5.0);
						directionx += .04*cos(5.0);
						zRotated -= 0.5;
					}
				}
				else if (type == 'l') {  // For Left
										 // This Condtion For Bumpy Road To Avoid Some Problems
					if (positionx >= 6.4 && positionx <= 9.6 && positionz >= -12.5 && positionz <= -9);
					else {
						// To Move Ball With Specific Distance When Don't Click Any Buttons
						positionx -= .04*cos(5.0);
						directionx -= .04*cos(5.0);
						zRotated += 0.5;
					}
				}
				else if (type == 'd') {  // For Down
										 // To Move Ball With Specific Distance When Don't Click Any Buttons
					positionz += .04*cos(5.0);
					directionz += .04*cos(5.0);
					xRotated += 0.5;
				}
				else if (type == 'u') {  // For UP
										 // To Move Ball With Specific Distance When Don't Click Any Buttons
					positionz -= .04*cos(5.0);
					directionz -= .04*cos(5.0);
					xRotated -= 0.5;
				}
			}
		}

		/////////////////////////////////////////////////////////

		TouchCube();   // Call This Function
	}
}

void clickRight() {
	// This Condition For (Bumpy Road)
	if (positionx >= 6.6 && positionx <= 7.5 && positionz >= -12.5 && positionz <= -9) {
		YPosBall += 0.12;
	}
	else if (positionx >= 8.4 && positionx <= 9.2 && positionz >= -12.5 && positionz <= -9) {
		YPosBall -= 0.12;
	}
	else if (positionx > 9.2 && positionx < 10.0 && positionz >= -12.5 && positionz <= -9) {
		YPosBall = -1.4;
	}

	// This Condition For (Eighth Road) To Move Down In Level 1
	else if (positionx >= 12.9 && positionx <= 22.8 && positionz >= -28.7 && positionz <= -24.0) {
		YPosBall -= .02;
		positiony -= .08;
		directiony -= .08;
	}

	// This Condition For (Down Hill) To Move Down In Level 2
	else if (positionx >= 29.0 && positionx <= 34.0 && positionz >= -50.0 && positionz <= -48.5) {
		YPosBall -= .02;
		positiony -= .06;
		directiony -= .06;
	}

	positionx += .7*cos(5.0);
	directionx += .7*cos(5.0);
	zRotated -= 6;
}

void clickLeft() {
	// This Condition For (Bumpy Road)
	if (positionx >= 6.6 && positionx <= 7.5 && positionz >= -12.5 && positionz <= -9) {
		YPosBall -= 0.12;
	}
	else if (positionx >= 8.4 && positionx <= 9.2 && positionz >= -12.5 && positionz <= -9) {
		YPosBall += 0.12;
	}
	else if (positionx > 6 && positionx < 6.6 && positionz >= -12.5 && positionz <= -9) {
		YPosBall = -1.4;
	}

	// This Condition For (Eighth Road) To Move Up
	else if (positionx >= 13.0 && positionx <= 23.0 && positionz >= -28.7 && positionz <= -24.0) {
		YPosBall += .02;
		positiony += .08;
		directiony += .08;
	}

	// This Condition For (Down Hill) To Move Down In Level 2
	else if (positionx >= 29.0 && positionx <= 34.0 && positionz >= -50.0 && positionz <= -48.5) {
		YPosBall += .02;
		positiony += .06;
		directiony += .06;
	}

	positionx -= .7*cos(5.0);
	directionx -= .7*cos(5.0);
	zRotated += 6;
}

void clickUp() {
	positionz -= .7*cos(5.0);
	directionz -= .7*cos(5.0);
	xRotated -= 8;
}

void clickDown() {
	positionz += .7*cos(5.0);
	directionz += .7*cos(5.0);
	xRotated += 8;
}

void keyboard(unsigned char key, int x, int y) {

	if (key == 27) {
		if (stopGame) {
			bk = 1;
			stopGame = false;
		}
		else {
			bk = 2;
			stopGame = true;
		}
	}
	else if (bk == 1) {
		mov = 1;
		c = 0;
		//Left
		if (key == 'a') {
			clickLeft();
			type = 'l';
		}
		//Right
		if (key == 'd') {
			clickRight();
			type = 'r';
		}
		//Up
		if (key == 'w') {
			clickUp();
			type = 'u';
		}
		//Down
		if (key == 's') {
			clickDown();
			type = 'd';
		}
	}
}

void specialKeyboard(int key, int x, int y) {

	if (key == GLUT_KEY_F1) {
		fullScreen = !fullScreen;
		if (fullScreen)
			glutFullScreen();
		else {
			glutReshapeWindow(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			glutPositionWindow(4, 30);
		}
	}

	else if (bk == 1) {
		mov = 1;
		c = 0;
		//Right
		if (key == GLUT_KEY_RIGHT) {
			clickRight();
			type = 'r';
		}
		//Left
		if (key == GLUT_KEY_LEFT) {
			clickLeft();
			type = 'l';
		}
		//Down
		if (key == GLUT_KEY_DOWN) {
			clickDown();
			type = 'd';
		}
		//Up
		if (key == GLUT_KEY_UP) {
			clickUp();
			type = 'u';
		}
	}
}

void newGame() {
	drawLevel = 1;
	positionx = 17.5, positiony = 6, positionz = 0, directionx = 17.5, directiony = 5.6, directionz = -1;
	XPosBall = 2, YPosBall = -1.4, ZPosBall = -16.5;
	x1 = 0.2, x2 = -0.2, y11 = y2 = y4 = y5 = z1 = z2 = z4 = z5 = 0.0;
	v1 = v2 = v4 = v5 = 1;
	x = 1;	 xx = 1;
	v = 0;   vv = 0;
	zmov = 0;  cond = 1;
	bk = 1;
}

void mouse(int button, int state, int x, int y) {
	int x1Pos = (wScreen / 2);
	int y1Pos = ((float)hScreen / 2.3);
	int y2Pos = ((float)hScreen / 1.86);
	int y3Pos = ((float)hScreen / 1.7);

	// When You Finish This Game And Choose Replay or Exit
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > x1Pos - 30 && x < x1Pos + 50 && y > y1Pos - 10 && y < y1Pos + 20 && drawLevel == 3) {
		newGame();
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > x1Pos - 30 && x < x1Pos + 50 && y > y2Pos - 10 && y < y2Pos + 10 && drawLevel == 3) {
		exit(0);
	}

	//// When Click Esc Key And Choose NewGame or Resume or Exit
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > x1Pos - 50 && x < x1Pos + 60 && y > y1Pos + 5 && y < y1Pos + 25 && stopGame) {
		newGame();
		stopGame = false;
		bk = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > x1Pos - 40 && x < x1Pos + 40 && y > y2Pos - 20 && y < y2Pos && stopGame) {
		stopGame = false;
		bk = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > x1Pos - 20 && x < x1Pos + 30 && y > y3Pos - 5 && y < y3Pos + 25 && stopGame) {
		exit(0);
	}

}


