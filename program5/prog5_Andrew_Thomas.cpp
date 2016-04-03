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
//static bool paused = true; //Start off paused

static int k = 100;
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
        return (.5*exp(-.04*sqrt(pow(80 * x -40, 2) + pow(90 * y-45, 2))) * cos(0.15*sqrt(pow(80 * x-40, 2) + pow(90 * y-45, 2))));
}

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT);
        glColor3f (1.0, 1.0, 1.0);

        glPushMatrix();

        glRotatef (yrot, 0.0, 1.0, 0.0);
        glRotatef (xrot, 1.0, 0.0, 0.0);

        float v[(k + 1) * (k + 1)][3];
        float ltri[(k + 1) * (k + 1)][3];
        float vn[(k + 1) * (k + 1)][3];
        float tn[2 * (k * k)];
        float h = 1.0/k;
        float j;
        float i;
        float x;
        float y;
        int i1 = 0;
        int i2 = 0;
        int i3 = 0;
        int indv = 0;
        int indt = 0;

        //Store verices
        for (j = 0; j <=k; j++)
        {
                y = j * h;
                for (i = 0; i <=k; i++)
                {
                        x = i*h;
                        v[indv][0] = x - .5;
                        v[indv][1] = y - .5;
                        v[indv][2] = bivariate(x,y);
                        indv = indv + 1;
                }
        }

        //Draw all vertices
        int w;
        /*for (w = 0; w <= (k + 1) * (k + 1); w++)
        {
                glBegin(GL_POINTS);
                glVertex3f(v[w][0], v[w][1], v[w][2]);
                glEnd();
        }*/

        //Store triangles
        for(j = 1; j <= k; j++)
        {
                for(i = 1; i <= k; i++)
                {
                        indv = j * (k + 1) + i;

                        ltri[indt][0] = indv - k - 2;
                        ltri[indt][1] = indv - k - 1;
                        ltri[indt][2] = indv;
                        ltri[indt+1][0] = indv - k - 2;
                        ltri[indt+1][1] = indv;
                        ltri[indt+1][2] = indv - 1;
                        indt = indt + 2;
                }
        }

        //Initialize normals
        for(indv = 0; indv <= (k + 1) * (k + 1) - 1; indv++)
        {
                vn[indv][0] = 0;
                vn[indv][1] = 0;
                vn[indv][2] = 0;
        }

        //Normalize
        for(indv = 0; indv <= (k + 1) * (k + 1) - 1; indv++)
        {
                float magnitude;

                magnitude = sqrt((vn[indv][0] * vn[indv][0]) + (vn[indv][1] * vn[indv][1]) + (vn[indv][2] * vn[indv][2]));

                vn[indv][0] = vn[indv][0] / magnitude;
                vn[indv][1] = vn[indv][1] / magnitude;
                vn[indv][2] = vn[indv][2] / magnitude;
        }

        //Add triangle normals to vertex normals
        for(indt = 0; indt <= 2 * (k * k); indt++)
        {
                i1 = ltri[indt][0];
                i2 = ltri[indt][1];
                i3 = ltri[indt][2];

                tn[0] = (v[i2][1]-v[i1][1])*(v[i3][2]-v[i1][2])-
                        (v[i2][2]-v[i1][2])*(v[i3][1]-v[i1][1]);

                tn[0] = (v[i2][2]-v[i1][2])*(v[i3][0]-v[i1][0])-
                        (v[i2][0]-v[i1][0])*(v[i3][2]-v[i1][2]);

                tn[0] = (v[i2][0]-v[i1][0])*(v[i3][1]-v[i1][1])-
                        (v[i2][1]-v[i1][1])*(v[i3][0]-v[i1][0]);

                //Normalize tn
                float magnitude;

                magnitude = sqrt((tn[0] * tn[0]) + (tn[1] * tn[1]) + (tn[2] * tn[2]));

                tn[0] = tn[0] / magnitude;
                tn[1] = tn[1] / magnitude;
                tn[2] = tn[2] / magnitude;

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

        //Store triangles
        /*for(w = 0; w <= k; w++)
        {
                glBegin(GL_TRIANGLES);
                glVertex3f(v[ltri[w][0]][0], v[ltri[w][0]][1], v[ltri[w][0]][2]);
                glVertex3f(v[ltri[w][1]][0], v[ltri[w][1]][1], v[ltri[w][1]][2]);
                glVertex3f(v[ltri[w][2]][0], v[ltri[w][2]][1], v[ltri[w][2]][2]);
                glEnd();
        }*/

        //int x;
        //int y;

        /*for(x = 0; x <= k; x++)
           {
                for(y = 0; y <= k; y++)
                {
                        glBegin(GL_POINTS);
                        glVertex3f((x / k) - .5, (y / k) - .5, bivariate((x / k) - .5, (y / k) - .5));
                        glEnd();
                }
           }*/

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
