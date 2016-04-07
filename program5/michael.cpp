//Michael Thomas Program 5

#include "GL/glut.h"
#include <stdlib.h>
#include <cmath>
#include <iostream>

static float xrot = 0, yrot = 0, yTrans = -1.5;; //View rotation variables
float h;
int indv = 0, indt = 0;
float x, y, j, i;
int i1, i2, i3;
float v[10000][3];
int ltri[10000][3];
float vn[10000][4];
float tn[4];
float tempNorm;

//Menu variables
static int win;
static int resetMenu;
static int zoomMenu;
static int exitMenu;
static int rotMenu;
static int pause = 0;
static int ortho = 1;
void createMenu(void);
void menu(int value);
void disp(void);


#define K 50


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/*glShadeModel(GL_SMOOTH);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glColor3f(.8, .8, .8);*/

	h = 1.0 / K;

	for (j = 0; j <= K; j++)
	{
		y = j*h;
		for (i = 0; i <= K; i++)
		{
			x = i*h;
			v[indv][0] = x;
			v[indv][1] = y;
			v[indv][2] = .5*exp(-.04*sqrt(pow((80 * x - 40), 2) + pow((90 * y - 45), 2))) * cos(0.15*sqrt(pow((80 * x - 40), 2) + pow((90 * y - 45), 2)));
			indv++;
		}
	}

	//This would print the points
	/*glBegin(GL_POINTS);
	for (int i = 0; i < K*K; i++)
	{
	glVertex3f(v[i][0], v[i][1], v[i][2]);

	}
	glEnd();*/

	indt = 0;
	for (j = 1; j < K; j++)
	{
		for (i = 1; i < K; i++)
		{
			indv = j*(K + 1) + i;

			ltri[indt][0] = indv - K - 2;
			ltri[indt][1] = indv - K - 1;
			ltri[indt][2] = indv;
			ltri[indt + 1][0] = indv - K - 2;
			ltri[indt + 1][1] = indv;
			ltri[indt + 1][2] = indv - 1;
			indt = indt + 2;

		}
	}

	for (indv = 0; indv < ((K + 1) * (K + 1)); indv++)
	{
		vn[indv][0] = 0;
		vn[indv][1] = 0;
		vn[indv][2] = 0;
	}
	for (indv = 0; indv < ((K + 1) * (K + 1)); indv++)
	{

		tempNorm = sqrt((vn[indv][0] * vn[indv][0]) + (vn[indv][1] * vn[indv][1]) + (vn[indv][2] * vn[indv][2]));

		if (tempNorm != 0)
		{
			vn[indv][0] = vn[indv][0] / tempNorm;
			vn[indv][1] = vn[indv][1] / tempNorm;
			vn[indv][2] = vn[indv][2] / tempNorm;
		}
	}

	//for (indt = 0; indt < ((K + 1) * (K + 1)); indt++)
	for (indt = 0; indt < 2 * (K * K); indt++)
	{
		i1 = ltri[indt][0];
		i2 = ltri[indt][1];
		i3 = ltri[indt][2];

		tn[0] = (v[i2][1] - v[i1][1])*(v[i3][2] - v[i1][2]) - (v[i2][2] - v[i1][2])*(v[i3][1] - v[i1][1]);
		tn[1] = (v[i2][2] - v[i1][2])*(v[i3][0] - v[i1][0]) - (v[i2][0] - v[i1][0])*(v[i3][2] - v[i1][2]);
		tn[2] = (v[i2][0] - v[i1][0])*(v[i3][1] - v[i1][1]) - (v[i2][1] - v[i1][1])*(v[i3][0] - v[i1][0]);

		tempNorm = sqrt((tn[0] * tn[0]) + (tn[1] * tn[1]) + (tn[2] * tn[2]));

		if (tempNorm != 0)
		{
			tn[0] = tn[0] / tempNorm;
			tn[1] = tn[1] / tempNorm;
			tn[2] = tn[2] / tempNorm;
		}

		vn[i1][0] += tn[0];
		vn[i1][1] += tn[1];
		vn[i1][2] += tn[2];

		vn[i2][0] += tn[0];
		vn[i2][1] += tn[1];
		vn[i2][2] += tn[2];

		vn[i3][0] += tn[0];
		vn[i3][1] += tn[1];
		vn[i3][2] += tn[2];
	}


	//Lighting
	glShadeModel(GL_SMOOTH);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glColor3f(.8, .8, .8);

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslatef(-.5, -.5, 0);



	//Draw triangles
	indt = 0;
	for (j = 1; j < K; j++)
	{
		for (i = 1; i < K; i++)
		{
			glBegin(GL_TRIANGLES);

			glNormal3f(vn[ltri[indt][0]][0], vn[ltri[indt][0]][1], vn[ltri[indt][0]][2]);
			glVertex3f(v[ltri[indt][0]][0], v[ltri[indt][0]][1], v[ltri[indt][0]][2]);


			glNormal3f(vn[ltri[indt][1]][0], vn[ltri[indt][1]][1], vn[ltri[indt][1]][2]);
			glVertex3f(v[ltri[indt][1]][0], v[ltri[indt][1]][1], v[ltri[indt][1]][2]);

			glNormal3f(vn[ltri[indt][2]][0], vn[ltri[indt][2]][1], vn[ltri[indt][2]][2]);
			glVertex3f(v[ltri[indt][2]][0], v[ltri[indt][2]][1], v[ltri[indt][2]][2]);


			glNormal3f(vn[ltri[indt + 1][0]][0], vn[ltri[indt + 1][0]][1], vn[ltri[indt + 1][0]][2]);
			glVertex3f(v[ltri[indt + 1][0]][0], v[ltri[indt + 1][0]][1], v[ltri[indt + 1][0]][2]);

			glNormal3f(vn[ltri[indt + 1][1]][0], vn[ltri[indt + 1][1]][1], vn[ltri[indt + 1][1]][2]);
			glVertex3f(v[ltri[indt + 1][1]][0], v[ltri[indt + 1][1]][1], v[ltri[indt + 1][1]][2]);

			glNormal3f(vn[ltri[indt + 1][2]][0], vn[ltri[indt + 1][2]][1], vn[ltri[indt + 1][2]][2]);
			glVertex3f(v[ltri[indt + 1][2]][0], v[ltri[indt + 1][2]][1], v[ltri[indt + 1][2]][2]);


			indt = indt + 2;

			glEnd();

		}
	}



	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	//Keep the viewport as the biggest possible square in the window
	if (h > w)
		glViewport(0, 0, w, w);
	else
		glViewport(0, 0, h, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 1.0, 20);

	//Move the light
	GLfloat light0Position[] = { GL_LIGHT0, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, yTrans);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	//rotate in x
	case 'x':
		xrot += 5;
		glutPostRedisplay();
		break;
	case 'X':
		xrot -= 5;
		glutPostRedisplay();
		break;
	//rotate in y
	case 'y':
		yrot += 5;
		glutPostRedisplay();
		break;
	case 'Y':
		yrot -= 5;
		glutPostRedisplay();
		break;
	//Zoom
	case 'z':
		yTrans -= .1;
		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glutPostRedisplay();
		break;
	case 'Z':
		yTrans += .1;
		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		glutPostRedisplay();
		break;
	case 'r':
		xrot = 0;
		yrot = 0;
		glutPostRedisplay();
		break;
		//Quit
	case 27:
	case 8:
		exit(0);
		break;
	default:
		break;
	}
}

void createMenu(void)
{
	//Create the rotation menu
	rotMenu = glutCreateMenu(menu);

	//Add submenu items to rotation
	glutAddMenuEntry("+ X (x)", 5);
	glutAddMenuEntry("- X (X)", 6);
	glutAddMenuEntry("+ Y (y)", 7);
	glutAddMenuEntry("- Y (Y)", 8);

	//Create the zoom menu
	zoomMenu = glutCreateMenu(menu);

	//Create the Reset menu
	resetMenu = glutCreateMenu(menu);

	//Create the Quit menu
	exitMenu = glutCreateMenu(menu);


	//Main menu entries
	glutAddSubMenu("Rotation", rotMenu);
	glutAddMenuEntry("z: Zoom Out", 1);
	glutAddMenuEntry("Z: Zoom In", 2);
	glutAddMenuEntry("r: Reset", 4);
	glutAddMenuEntry("ESC: Quit", 0);

	//Attach the menu to right click
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int value)
{
	//Quit
	if (value == 0)
	{
		glutDestroyWindow(win);
		exit(0);
	}

	//Reset
	else if (value == 4)
	{
		xrot = 0;
		yrot = 0;
		glutPostRedisplay();
	}
	//+X
	else if (value == 5)
	{
		xrot += 10;
		glutPostRedisplay();
	}
	//-X
	else if (value == 6)
	{
		xrot -= 10;
		glutPostRedisplay();
	}
	//+Y
	else if (value == 7)
	{
		yrot += 10;
		glutPostRedisplay();
	}
	//-Y
	else if (value == 8)
	{
		yrot -= 10;
		glutPostRedisplay();
	}
	else if (value == 1)
	{
		yTrans -= .2;
		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	else if (value == 2)
	{
		yTrans += .2;
		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(glutGet(GLUT_SCREEN_HEIGHT), glutGet(GLUT_SCREEN_HEIGHT));
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - glutGet(GLUT_SCREEN_HEIGHT)) / 2, 0);
	glutCreateWindow("Michael Thomas Program 5 - 3D Function Plotting");;
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	createMenu();
	glutMainLoop();
	return 0;
}
