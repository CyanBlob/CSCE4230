/*
   Andrew Thomas
   CSCE 4230
   Program 4
   3/8/2016

   A simple simulation of three segment arm, where each segment is individually controllable
   Compile and run with: g++ robot_Andrew_Thomas.cpp -o robot_Andrew_Thomas -lGL -lGLU -lglut && ./robot_Andrew_Thomas
 */

#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GL/glut.h>

//Define globals
static int shoulder = 0, elbow = 0, wrist = 0;
static float xrot = 0, yrot = 0;
static bool wire = true; //Bool for wireframe vs solid cubes
static bool ortho = true; //Bool for orthographic view vs frustum

void init(void)
{
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_FLAT);
}

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();

        glRotatef (yrot, 0.0, 1.0, 0.0);
        glRotatef (xrot, 1.0, 0.0, 0.0);

        //Draw shoulder
        glTranslatef (-1.0, 0.0, 0.0);
        glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
        glTranslatef (1.0, 0.0, 0.0);
        glPushMatrix();
        glScalef (5,1,1);
        glColor3d(0,0,1);
        (wire) ? glutWireCube (1.0) : glutSolidCube (1.0); //If wire, draw wireframe. Else, draw solid
        glPopMatrix();

        //Draw elbow
        glTranslatef (2.25, 0.0, 0.0);
        glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
        glTranslatef (2.25, 0.0, 0.0);
        glPushMatrix();
        glScalef (4,1,1);
        glColor3d(0,1,0);
        (wire) ? glutWireCube (1.0) : glutSolidCube (1.0); //If wire, draw wireframe. Else, draw solid
        glPopMatrix();

        //Draw wrist
        glTranslatef (1.5, 0.0, 0.0);
        glRotatef ((GLfloat) wrist, 0.0, 0.0, 1.0);
        glTranslatef (1.5, 0.0, 0.0);
        glPushMatrix();
        glScalef (2,1,1);
        glColor3d(1,0,0);
        (wire) ? glutWireCube (1.0) : glutSolidCube (1.0); //If wire, draw wireframe. Else, draw solid
        glPopMatrix();
        glPopMatrix();
        glutSwapBuffers();
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
        //glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        (ortho) ? glOrtho(-10,10,-10,10,50,-50) : glFrustum (-3, 3, -3, 3, 1.5, 50.0); //glOrtho or glFrustum
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef (0.0, 0.0, -5.0);
}

//Menu callbacks
void menu(int value){
        switch (value) {
        case 's':
                shoulder = (shoulder + 5) % 360;
                glutPostRedisplay();
                break;
        case 'S':
                shoulder = (shoulder - 5) % 360;
                glutPostRedisplay();
                break;
        case 'e':
                elbow = (elbow + 5) % 360;
                glutPostRedisplay();
                break;
        case 'E':
                elbow = (elbow - 5) % 360;
                glutPostRedisplay();
                break;
        case 'w':
                wrist = (wrist + 5) % 360;
                glutPostRedisplay();
                break;
        case 'W':
                wrist = (wrist - 5) % 360;
                glutPostRedisplay();
                break;
        case 'h':
                wire = !wire;
                glutPostRedisplay();
                break;
        case 'p':
                ortho = !ortho;
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
        glutAddMenuEntry("w: Move wrist", 'w');
        glutAddMenuEntry("W: Reverse wrist", 'W');
        glutAddMenuEntry("e: Move forearm", 'e');
        glutAddMenuEntry("E: Reverse forearm", 'E');
        glutAddMenuEntry("s: Move shoulder", 's');
        glutAddMenuEntry("S: Reverse shoulder", 'S');
        glutAddMenuEntry("h: Toggle wireframe", 'h');
        glutAddMenuEntry("p: Toggle viewmode", 'p');
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

int main(int argc, char** argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize (500, 500);
        glutInitWindowPosition (100, 100);
        glutCreateWindow (argv[0]);

        //Create the menu
        createMenu();

        init ();

        //Enable depth testing
        glDepthMask(GL_TRUE);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
        return 0;
}
