/*
    Andrew Thomas
    CSCE 4230
    2/4/2016
    COMPILATION AND RUNNING: g++ chaos.cpp -o chaos -lGL -lGLU -lglut && ./chaos

*/
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream> //cout
#include <unistd.h> //usleep()

using namespace std;

//Counters
int x = 0;
int n = 60000;

//Corners of the main triangle
float v1[3] = {.25,.25,0};
float v2[3] = {.5,.75,0};
float v3[3] = {.75,.25,0};

//The previously used coordinate
float vPrevious[3] = {0,0,0};

//Enables pausing execution (spacebar)
bool paused = false;

void display(void)
{
	x++;
        if(x >= n)
                paused = true;  //Stop execution
        if (!paused)
        {
                float randomNum = (rand() % 3); //Randomly pick coordinate

                //Change color of point based on coordinate chosen (to make it look cooler)
                if(randomNum == 0)
                        glColor3f (1,0,0);
                if(randomNum == 1)
                        glColor3f (0,1,0);
                if(randomNum == 2)
                        glColor3f (0,0,1);

                //Draw our points at the midpoint between the new point and the old point
                //Also saves these coordinates in vPrevious
                glBegin(GL_POINTS);
                if(randomNum == 0)
                {
                        glVertex3f ((v1[0] + vPrevious[0]) / 2, (v1[1] + vPrevious[1]) / 2, 0.0);
                        vPrevious[0] = (v1[0] + vPrevious[0]) / 2;
                        vPrevious[1] = (v1[1] + vPrevious[1]) / 2;

                }
                else if(randomNum == 1)
                {
                        glVertex3f ((v2[0] + vPrevious[0]) / 2, (v2[1] + vPrevious[1]) / 2, 0.0);
                        vPrevious[0] = (v2[0] + vPrevious[0]) / 2;
                        vPrevious[1] = (v2[1] + vPrevious[1]) / 2;
                }
                else if(randomNum == 2)
                {
                        glVertex3f ((v3[0] + vPrevious[0]) / 2, (v3[1] + vPrevious[1]) / 2, 0.0);
                        vPrevious[0] = (v3[0] + vPrevious[0]) / 2;
                        vPrevious[1] = (v3[1] + vPrevious[1]) / 2;
                }
                glEnd();

/* don't wait!
 * start processing buffered OpenGL routines
 */
                glFlush ();
                glutPostRedisplay();
        }
}

void init (void)
{
/* select clearing color        */
        glClearColor (0.0, 0.0, 0.0, 0.0);

/* initialize viewing values  */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); //This happens in reshape function now
}


//Allows pausing/termination of the program
void handleKeypress(unsigned char key, //The key that was pressed
                    int x, int y) {    //The current mouse coordinates (unused for now)
        switch (key)
        {
        case 27:                //Escape key
                exit(0);        //Exit the program
                break;
        case 32:                //Spacebar
                paused = !paused;
		glutPostRedisplay();
                break;
        }
}

//Keep the viewport square when window size changes
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
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

}

/*
 * Declare initial window size, position, and display mode

 * (single buffer and RGBA).  Open window with "Chaos - Andrew Thomas"
 * in its title bar.  Call initialization routines.
 * Register callback function to display graphics, handle reshapes, and handle keypresses
 * Enter main loop and process events.
 */
int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize (500,500);
        glutInitWindowPosition (100, 100);
        glutCreateWindow ("Chaos - Andrew Thomas");
        init ();
        glutDisplayFunc(display);
        glutKeyboardFunc(handleKeypress);
        glutReshapeFunc(reshape);
        glutMainLoop();
        return 0; /* ANSI C requires main to return int. */
}
