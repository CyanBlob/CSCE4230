/*
    Andrew Thomas
    CSCE 4230
    2/4/2016
    COMPILATION AND RUNNING: g++ pendulum.cpp -o chaos -lGL -lGLU -lglut && ./pendulum
 */
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream> //cout
#include <unistd.h> //usleep()
#include <cmath>

using namespace std;

static GLfloat spin = 0.0;
static GLfloat spinScale = 0.0;
static int r = 1;
static float l = r * 8;
void init(void)
{
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_FLAT);
}

void display(void)
{
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();

        //Draw the anchor
        glBegin(GL_POLYGON);
        //glBegin(GL_POINTS);
        glVertex3f (-.5, -.5, 0.0);
        glVertex3f (.5, -.5, 0.0);
        glVertex3f (.5, .5, 0.0);
        glVertex3f (-.5, .5, 0.0);
        glEnd();

        //Spin the line and bob
        glRotatef(spin, 0.0, 0.0, 1.0);

        //Draw the line
        glLineWidth(2);
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(l,0,0);
        glEnd();


        glColor3f(1.0, 1.0, 1.0);

        //Without finding the y value of the middle of the hexagon, I can't move it into place
        float displacement = ((l + r*cos(0/6.0*2*M_PI)) + (l + r*cos(3/6.0*2*M_PI))) / 2.0;

        //Draw the bob
        glBegin(GL_POLYGON);
        for (int i = 0; i < 6; ++i)
        {
                //Use math to create the hexagon
                glVertex2d(l + r*sin(i/6.0*2*M_PI),
                           l + r*cos(i/6.0*2*M_PI) - displacement);
        }
        glEnd();
        glPopMatrix();
        glutSwapBuffers();
}

//Rotate CCW
void spinDisplay(void)
{

        spin = spin + 2.0 * spinScale;
        if (spin > 360.0)
                spin = spin - 360.0;
        glutPostRedisplay();
}

//Rotate CW
void reverseSpinDisplay(void)
{
        spin = spin - 2.0 * (spinScale * -1);
        if (spin < 0)
                spin = spin + 360.0;
        glutPostRedisplay();
}

//Force the viewport to be a square
void reshape(int w, int h)
{
        if(h > w)
        {
                glViewport(0,(h-w)/2,w,w);
        }
        else
        {
                glViewport((w-h)/2,0,h,h);
        }
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-50.0,50.0,-50.0,50.0,-1.0,1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

}

void menu(int value){
        if(value == 0)
        {
                spinScale = spinScale + 1.0;
                glutIdleFunc(spinDisplay);
        }
        else if(value == 1)
        {
                spinScale = spinScale - 1.0;
                glutIdleFunc(reverseSpinDisplay);
        }
        else if(value == 2)
        {
                spinScale = 0.0;
                glutIdleFunc(NULL);
        }
        else if(value == 3)
        {
                exit(0);
        }
}

//Create the menu outside of main() to keep things a little cleaner
void createMenu(void)
{
        glutCreateMenu(menu);

        // Add sub menu entry
        glutAddMenuEntry("+: Increase speed CCW", 0);
        glutAddMenuEntry("-: Increase speed CW", 1);
        glutAddMenuEntry("space: Pause", 2);
        glutAddMenuEntry("esc: Exit program", 3);

        //Open the menu on right-click
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void handleKeypress(unsigned char key, //The key that was pressed
                    int x, int y) {    //The current mouse coordinates (unused for now)
        switch (key)
        {
        case 27:                                  //Escape key
                exit(0);
                break;
        case 32:                                  //Spacebar
                spinScale = 0.0;
                glutIdleFunc(NULL);
                break;
        case 43:                                  //'+''' key
                spinScale = spinScale + 1.0;
                glutIdleFunc(spinDisplay);
                break;
        case 45:
                spinScale = spinScale - 1.0;     //'-' key
                glutIdleFunc(reverseSpinDisplay);
                break;
        }
}

void mouse(int button, int state, int x, int y)
{
        switch (button) {
        case GLUT_LEFT_BUTTON:
                if (state == GLUT_DOWN)
                {
                        spinScale = spinScale = 1.0;
                        glutIdleFunc(spinDisplay);
                }
                break;
        case GLUT_MIDDLE_BUTTON:
                if (state == GLUT_DOWN)
                        glutIdleFunc(NULL);
                break;

        default:
                break;
        }
}
/*
   *Request double buffer display mode.
   *Register mouse input callback functions
 */
int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (100, 100);
        glutCreateWindow (argv[0]);
        init ();

        //Create the menu
        createMenu();

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutKeyboardFunc(handleKeypress);
        glutMainLoop();
        return 0;
}
