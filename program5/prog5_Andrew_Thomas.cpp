/*
    Andrew Thomas
    CSCE 4230
    3/3/2016
    Program 3

    A simple simulation showing 5 planets, each with unique animations (double buffered)

    COMPILATION AND RUNNING: g++ space_Andrew_Thomas.cpp -o space_Andrew_Thomas -lGL -lGLU -lglut && ./space_Andrew_Thomas

    CONTROLS: 1,2,3 = change speed
              y = toggle spause
              esc = quit
 */
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

static int year = 0;
static int speedScale = 1;
static bool paused = true; //Start off paused

static float k = 50;
static float xrot;
static float yrot;
static float zoom = 10;

void init(void)
{
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_FLAT);
}

//Draw 5 planets, each doing something different

float bivariate(float x, float y)
{
        .5*exp(-.04*sqrt(pow(80 * x -40, 2) + pow(90 * y-45, 2))) * cos(0.15*sqrt(pow(80 * x-40, 2) + pow(90 * y-45, 2)));
}

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT);
        glColor3f (1.0, 1.0, 1.0);

        glPushMatrix();

        glRotatef (yrot, 0.0, 1.0, 0.0);
        glRotatef (xrot, 1.0, 0.0, 0.0);

        int x;
        int y;

        for(x = 0; x <= k; x++)
        {
                for(y = 0; y <= k; y++)
                {
                        glBegin(GL_POINTS);
                        glVertex3f((x / k) - .5, (y / k) - .5, bivariate((x / k) - .5, (y / k) - .5));
                        glEnd();
                }
        }

        /*glTranslatef (5.0, -4.0, 0.0);
           glRotatef ((GLfloat) -year * speedScale, 1.0, 1.0, 1.0);
           glutWireSphere(.5, 20, 16); //Spinning planet
           glRotatef ((GLfloat) year * speedScale, 1.0, 1.0, 1.0);
           glTranslatef (-5.0, 4.0, 0.0);


           glRotatef ((GLfloat) -year * speedScale, 1.0, 1.0, 1.0);
           glTranslatef (-5.0, -2.0, 0.0);
           glutWireSphere(.5, 20, 16); //3D axis planet
           glTranslatef (5.0, 2.0, 0.0);
           glRotatef ((GLfloat) year * speedScale, 1.0, 1.0, 1.0);


           glTranslatef (3.0, 2.0, 0.0);
           glRotatef ((GLfloat) (year / 2) * speedScale, 0.0, 0.0, 1.0);
           glutWireSphere(.5, 20, 16); //Draw sun


           glTranslatef (3.0, 0.0, 0.0);
           glutWireSphere(.3, 20, 16); //Draw planet

           glRotatef ((GLfloat) (year / 2) * speedScale, 0.0, 0.0, 1.0);

           glTranslatef (.7, 0.0, 0.0);


           glutWireSphere(0.1, 10, 8); //Draw moon*/

        glPopMatrix();
        glutSwapBuffers(); //Animate scene
}


void reshape (int w, int h)
{
        if(h > w)
        {
                glViewport(0,(h-w)/2,w,w);
        }
        else
        {
                glViewport((w-h)/2,0,h,h);
        }

        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(zoom, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt (0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
//Menu callbacks
void menu(int value){
        switch (value) {
        case 'z':
                zoom += 1;
                reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
                glutPostRedisplay();
                break;
        case 'Z':
                zoom -= 1;
                reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
                glutPostRedisplay();
                break;
        case 'x':
                xrot += 5;
                glutPostRedisplay();
                break;
        case 'X':
                xrot -= 5;
                glutPostRedisplay();
                break;
        case 'y':
                yrot += 5;
                glutPostRedisplay();
                break;
        case 'Y':
                yrot -= 5;
                glutPostRedisplay();
                break;
        case 27:
                exit(0);
                break;
        default:
                break;
        }

}

//Create the menu outside of main() to keep things a little cleaner
void createMenu(void)
{
        glutCreateMenu(menu);

        // Add sub menu entry
        glutAddMenuEntry("x: increase xrot", 'x');
        glutAddMenuEntry("X: decrease xrot", 'X');
        glutAddMenuEntry("y: increase yrot", 'y');
        glutAddMenuEntry("Y: decrease yrot", 'Y');
        glutAddMenuEntry("esc: Quit", 27);

        //Open the menu on right-click
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard (unsigned char key, int x, int y)
{
        menu(key);
}

//This is my idle callback
//Increment year to forward animation
void rotateYear()
{
        year = (year + 1) % (720 * speedScale);
        glutPostRedisplay();
}


int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (100, 100);
        glutCreateWindow (argv[0]);

        //Create the menu
        createMenu();

        init ();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
        return 0;
}
