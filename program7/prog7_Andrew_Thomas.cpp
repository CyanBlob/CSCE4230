/*
 * Andrew Thomas
 * CSCE 4230
 * 5/5/2016
 * Program 7
 *
 * COMPILATION AND RUNNING: g++ prog7_Andrew_Thomas.cpp -o prog7_Andrew_Thomas -lGL -lGLU -lglut && ./prog7_Andrew_Thomas
 *
 * CONTROLS: x,X = rotate about x axis
 *           y,Y = rorate about y axis
 *           w,a,s,d = pan
 *           z,Z zoom in/out
 *           esc = quit
 *
 *           click+drag = move point
 */

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glut.h>

#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

//Points used as control points
static GLfloat p0[3] = {-9.0, -9.0, 0.0};
static GLfloat p1[3] = {-5.0, -8.0, 0.0};
static GLfloat p2[3] = {-1.0, -6.0, 0.0};
static GLfloat p3[3] = {-0.0, -0.0, 0.0};
static GLfloat p4[3] = {1.0,   6.0, 0.0};
static GLfloat p5[3] = {5.0,   8.0, 0.0};
static GLfloat p6[3] = {9.0,   9.0, 0.0};

//Arrays of control points
static GLfloat ctrlpoints[4][3] = {
    {p0[0], p0[1], p0[2]}, 
    {p1[0], p1[1], p1[2]}, 
    {p2[0], p2[1], p2[2]}, 
    {p3[0], p3[1], p3[2]}};

static GLfloat ctrlpoints2[4][3] = {
    {p3[0], p3[1], p3[2]}, 
    {p4[0], p4[1], p4[2]}, 
    {p5[0], p5[1], p5[2]}, 
    {p6[0], p6[1], p6[2]}};

static float zoom = 10; //Zoom level
static bool left_button_down = false; //Whether the LMB is pressed

//Translation variables
static float xOffset = 0;
static float yOffset = 0;

//Rotaion variables
static float xRot = 0; 
static float yRot = 0;

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_MAP1_VERTEX_3);
}

//Cursor x, y, and z values
static GLdouble ox=0.0,oy=0.0,oz=0.0;

//Find the point closest to the cursor
int findClosestPoint(float x, float y, float z)
{
    int i;
    int closest;
    float dists[7];
    float closestDist = 10000;

    dists[0] = ((x - p0[0]) * (x - p0[0])) + ((y - p0[1]) * (y - p0[1])); //+ ((z - p0[2]) * (z - p0[2]));
    dists[1] = ((x - p1[0]) * (x - p1[0])) + ((y - p1[1]) * (y - p1[1])); //+ ((z - p1[2]) * (z - p1[2]));
    dists[2] = ((x - p2[0]) * (x - p2[0])) + ((y - p2[1]) * (y - p2[1])); //+ ((z - p2[2]) * (z - p2[2]));
    dists[3] = ((x - p3[0]) * (x - p3[0])) + ((y - p3[1]) * (y - p3[1])); //+ ((z - p3[2]) * (z - p3[2]));
    dists[4] = ((x - p4[0]) * (x - p4[0])) + ((y - p4[1]) * (y - p4[1])); //+ ((z - p4[2]) * (z - p4[2]));
    dists[5] = ((x - p5[0]) * (x - p5[0])) + ((y - p5[1]) * (y - p5[1])); //+ ((z - p5[2]) * (z - p5[2]));
    dists[6] = ((x - p6[0]) * (x - p6[0])) + ((y - p6[1]) * (y - p6[1])); //+ ((z - p6[2]) * (z - p6[2]));

    for (i = 0; i < 7; i++)
    {
        if(dists[i] < closestDist)
        {
            closestDist = dists[i]; 
            closest = i;
        }
    }

    return closest;
}

//Check which mouse button was pressed
void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        left_button_down = true;
    }
    else
    {
        left_button_down = false;
    }
}

//Reset cursor
void passiveMouseMotion(int x, int y)
{
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    glutPostRedisplay();
}

//Check mouse movements, moving points as necessary
void mouseMotion(int x,int y) 
{

    if(!left_button_down)
    {
        return;
    }

    glutSetCursor(GLUT_CURSOR_NONE); 

    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;
    
    int closest;
    int i;
    int j;

    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
    
    oz += 5;
    ox -= xOffset;
    oy -= yOffset;
    closest = findClosestPoint(ox, oy, oz);

    //Move the closest point, enforcing collinearity for P2, P3, and P4
    switch (closest)
    {
        case 0:
            p0[0] = ox;
            p0[1] = oy;
            break;
        case 1:
            p1[0] = ox;
            p1[1] = oy;
            break;
        case 2:
            p4[0] = p4[0] + (p2[0] - ox); 
            p4[1] = p4[1] + (p2[1] - oy);
            p2[0] = ox;
            p2[1] = oy;
            break;
        case 3:
            p2[0] = p2[0] - (p3[0] - ox);
            p2[1] = p2[1] - (p3[1] - oy);
            p4[0] = p4[0] - (p3[0] - ox);
            p4[1] = p4[1] - (p3[1] - oy);
            p3[0] = ox;
            p3[1] = oy;
            break;
        case 4:
            p2[0] =p2[0] + (p4[0] - ox);
            p2[1] =p2[1] + (p4[1] - oy);
            p4[0] = ox;
            p4[1] = oy;
            break;
        case 5:
            p5[0] = ox;
            p5[1] = oy;
            break;    
        case 6:
            p6[0] = ox;
            p6[1] = oy;
            break;
        default:
            break;
    }

    //Temp control points, to be copied over regular control points
    GLfloat tmpctrlpoints[4][3] = 
    {
        {p0[0], p0[1], p0[2]}, 
        {p1[0], p1[1], p1[2]}, 
        {p2[0], p2[1], p2[2]}, 
        {p3[0], p3[1], p3[2]}
    };


    GLfloat tmpctrlpoints2[4][3] = 
    {
        {p3[0], p3[1], p3[2]}, 
        {p4[0], p4[1], p4[2]}, 
        {p5[0], p5[1], p5[2]}, 
        {p6[0], p6[1], p6[2]}
    };

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 3; j++)
        {
            ctrlpoints[i][j] = tmpctrlpoints[i][j];
            ctrlpoints2[i][j] = tmpctrlpoints2[i][j];
        }
    }

    glutPostRedisplay();
}

void display(void)
{
   int i;
   int numPoints = 100;

   glPushMatrix();

   //Translate/rotate
   glTranslatef(xOffset, yOffset, 0);
   glRotatef(xRot, 1, 0, 0);
   glRotatef(yRot, 0, 1, 0);

   //Draw first curve
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <= numPoints; i++) 
         glEvalCoord1f((GLfloat) i/numPoints);
   glEnd();

   /* The following code displays the control points as dots. */
   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   
   //Draw second curve
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints2[0][0]);

   glColor3f(0.0, 1.0, 1.0);
   glBegin(GL_LINE_STRIP);
      for (i = 0; i <=numPoints; i++) 
         glEvalCoord1f((GLfloat) i/numPoints);
   glEnd();
   glPointSize(5.0);
   glColor3f(0.0, 1.0, 1.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints2[i][0]);
   glEnd();

   glPopMatrix();
   glutSwapBuffers();

}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-zoom, zoom, -zoom*(GLfloat)h/(GLfloat)w, 
               zoom*(GLfloat)h/(GLfloat)w, -25.0, 25.0);
   else
      glOrtho(-zoom*(GLfloat)w/(GLfloat)h, 
               zoom*(GLfloat)w/(GLfloat)h, -zoom, zoom, -25.0, 25.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void menu(int value)
{
    switch(value)
    {
        case 'z':
            zoom++;
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            glutPostRedisplay();
            break;
        case 'Z':
            zoom--;
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
            glutPostRedisplay();
            break;
        case 'x':
            xRot += 2;
            if(xRot > 90)
                xRot = 90;
            glutPostRedisplay();
            break;
        case 'X':
            xRot -= 2;
            if(xRot < 0)
                xRot = 0;
            glutPostRedisplay();
            break; 
        case 'y':
            yRot += 2;
            if(yRot > 90)
                yRot = 90;
            glutPostRedisplay();
            break;
        case 'Y':
            yRot -= 2;
            if(yRot < 0)
                yRot = 0;
            glutPostRedisplay();
            break;
        case 'd':
            xOffset++;
            glutPostRedisplay();
            break;
        case 'a':
            xOffset--;
            glutPostRedisplay();
            break;
        case 's':
            yOffset--;
            glutPostRedisplay();
            break;
        case 'w':
            yOffset++;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
    }
}

void createMenu(void)
{
    glutCreateMenu(menu);

    glutAddMenuEntry("z: Zoom out", 'z');
    glutAddMenuEntry("Z: Zoom in", 'Z');
    glutAddMenuEntry("x: Rotate x", 'x');
    glutAddMenuEntry("X: Rotate x", 'X');
    glutAddMenuEntry("y: Rotate y", 'y');
    glutAddMenuEntry("Y: Rotate y", 'Y');
    glutAddMenuEntry("w: Pan up", 'w');
    glutAddMenuEntry("s: Pan down", 's');
    glutAddMenuEntry("a: Pan left", 'a');
    glutAddMenuEntry("d: Pan right", 'd');
    glutAddMenuEntry("esc: Quit", 27);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y)
{
   menu(key);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   createMenu();
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc (keyboard);
   glutMouseFunc(mouse);
   glutMotionFunc(mouseMotion); 
   glutPassiveMotionFunc(passiveMouseMotion); 
   glutMainLoop();
   return 0;
}
