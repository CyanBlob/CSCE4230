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

static int year = 0;
static int speedScale = 1;
static bool paused = true; //Start off paused

void init(void)
{
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_FLAT);
}

//Draw 5 planets, each doing something different
void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT);
        glColor3f (1.0, 1.0, 1.0);

        glPushMatrix();

        glTranslatef (5.0, -4.0, 0.0);
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


        glutWireSphere(0.1, 10, 8); //Draw moon

        glPopMatrix();
        glutSwapBuffers(); //Animate scene
}


void reshape (int w, int h)
{
        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt (0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//This is my idle callback
//Increment year to forward animation
void rotateYear()
{
        year = (year + 1) % (720 * speedScale);
        glutPostRedisplay();
}

//Menu callbacks
void menu(int value){
        
        //Change execution speed to slow
        if(value == 1)
        {
                speedScale = 1;
        }
        //Change execution speed to medium
        else if(value ==2)
        {
                speedScale = 2;
        }
        //Change execution speed to fast
        else if(value == 3)
        {
                speedScale = 3;
        }

        //Exit
        else if(value == 4)
        {
                exit(0);
        }

        //Toggle pause
        else if(value == 5)
        {
            if(!paused)
            {
                paused = true;
                glutIdleFunc(NULL);
            }
            else if (paused)
            { 
                paused = false;
                glutIdleFunc(rotateYear);
        
            }
        }
}


//Create the menu outside of main() to keep things a little cleaner
void createMenu(void)
{
        glutCreateMenu(menu);

        // Add sub menu entry
        glutAddMenuEntry("y: Pause/Start", 5);
        glutAddMenuEntry("1: Slow", 1);
        glutAddMenuEntry("2: Medium", 2);
        glutAddMenuEntry("3: Fast", 3);
        glutAddMenuEntry("esc: Quit", 4);

        //Open the menu on right-click
        glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//Keyboard handler
void keyboard (unsigned char key, int x, int y)
{
        switch (key) {
        case '1':
                menu(1);
                break;
        case '2':
                menu(2);
                break;
        case '3':
                menu(3);
                break;
        case 'y':
                //Start/stop
                menu(5);
                break;
        case 27:
                exit(0);
                break;
        default:
                break;
        }
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
