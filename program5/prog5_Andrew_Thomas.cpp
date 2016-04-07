/*
    Andrew Thomas
    CSCE 4230
    4/7/2016
    Program 5

    A simple OpenGL program demoing lighting on the triangle mesh of a bivariate function

    COMPILATION AND RUNNING: g++ prog5_Andrew_Thomas.cpp -o prog5_Andrew_Thomas -lGL -lGLU -lglut && ./prog5_Andrew_Thomas

    CONTROLS: x, X = rotate around x axis
              y, Y = rotate around y axis
              z, Z = zoom in/out
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

static int k = 50;
static float xrot;
static float yrot;
static float zoom = 10;

void init(void)
{
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_SMOOTH);

        //Enable lighting for the front of the mesh
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        GLfloat lightColor0[] = {1, 1, 1, 0.0};
        GLfloat lightPos0[] = {5.0f, 0.0f, 5.0f, 1.0f}; //Positioned at (5, 0, 5)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glEnable(GL_DEPTH_TEST);
}

float bivariate(float x, float y)
{
        return (.5*exp(-.04*sqrt(pow(80 * x -40, 2) + pow(90 * y-45, 2))) * cos(0.15*sqrt(pow(80 * x-40, 2) + pow(90 * y-45, 2))));
}

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f (1.0, 1.0, 1.0);

        glPushMatrix();

        glRotatef (yrot, 0.0, 1.0, 0.0);
        glRotatef (xrot, 1.0, 0.0, 0.0);

        float v[(k + 1) * (k + 1)][3]; //Vertex coordinates
        int ltri[2 * (k * k)][3];      //List of triangles
        float vn[(k + 1) * (k + 1)][3]; //Vertex normals
        float tn[2 * (k * k)];         //Triangle normals
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
                //cout<<magnitude<<endl;
                magnitude = 1;
                vn[indv][0] = vn[indv][0] / magnitude;
                vn[indv][1] = vn[indv][1] / magnitude;
                vn[indv][2] = vn[indv][2] / magnitude;
        }

        //Add triangle normals to vertex normals
        for(indt = 0; indt < 2 * (k * k); indt++)
        {
                i1 = ltri[indt][0];
                i2 = ltri[indt][1];
                i3 = ltri[indt][2];

                tn[0] = (v[i2][1]-v[i1][1])*(v[i3][2]-v[i1][2])-
                        (v[i2][2]-v[i1][2])*(v[i3][1]-v[i1][1]);

                tn[1] = (v[i2][2]-v[i1][2])*(v[i3][0]-v[i1][0])-
                        (v[i2][0]-v[i1][0])*(v[i3][2]-v[i1][2]);

                tn[2] = (v[i2][0]-v[i1][0])*(v[i3][1]-v[i1][1])-
                        (v[i2][1]-v[i1][1])*(v[i3][0]-v[i1][0]);

                //Normalize tn
                float magnitude;

                magnitude = sqrt((tn[0] * tn[0]) + (tn[1] * tn[1]) + (tn[2] * tn[2]));
                //cout<<magnitude<<endl;

                tn[0] = tn[0] / magnitude;
                tn[1] = tn[1] / magnitude;
                tn[2] = tn[2] / magnitude;

                //cout<<vn[i1][0]<<endl;
                vn[i1][0] += tn[0];
                vn[i1][1] += tn[1];
                vn[i1][2] += tn[2];
                //cout<<vn[i1][0]<<endl<<endl;

                vn[i2][0] += tn[0];
                vn[i2][1] += tn[1];
                vn[i2][2] += tn[2];

                vn[i3][0] += tn[0];
                vn[i3][1] += tn[1];
                vn[i3][2] += tn[2];
        }

        //Draw triangles
        indt = 0;
        for(j = 1; j <= k; j++)
        {
                for(i = 1; i <= k; i++)
                {
                        indv = 1;

                        glBegin(GL_TRIANGLES);
                        glNormal3f(vn[ltri[indt][0]][0],vn[ltri[indt][0]][1],vn[ltri[indt][0]][2]);
                        glVertex3f(v[ltri[indt][0]][0], v[ltri[indt][0]][1], v[ltri[indt][0]][2]);

                        glNormal3f(vn[ltri[indt][1]][0],vn[ltri[indt][1]][1],vn[ltri[indt][1]][2]);
                        glVertex3f(v[ltri[indt][1]][0], v[ltri[indt][1]][1], v[ltri[indt][1]][2]);

                        glNormal3f(vn[ltri[indt][2]][0],vn[ltri[indt][2]][1],vn[ltri[indt][2]][2]);
                        glVertex3f(v[ltri[indt][2]][0], v[ltri[indt][2]][1], v[ltri[indt][2]][2]);

                        //Second pairs
                        glNormal3f(vn[ltri[indt][0]][0],vn[ltri[indt][0]][1],vn[ltri[indt][0]][2]);
                        glVertex3f(v[ltri[indt + 1][0]][0], v[ltri[indt + 1][0]][1], v[ltri[indt + 1][0]][2]);

                        glNormal3f(vn[ltri[indt + 1][1]][0],vn[ltri[indt + 1][1]][1],vn[ltri[indt + 1][1]][2]);
                        glVertex3f(v[ltri[indt + 1][1]][0], v[ltri[indt + 1][1]][1], v[ltri[indt + 1][1]][2]);

                        glNormal3f(vn[ltri[indt + 1][2]][0],vn[ltri[indt + 1][2]][1],vn[ltri[indt + 1][2]][2]);
                        glVertex3f(v[ltri[indt + 1][2]][0], v[ltri[indt + 1][2]][1], v[ltri[indt + 1][2]][2]);
                        glEnd();

                        indt = indt + 2;
                }
        }

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
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(zoom, 1, 1.0, 20.0);
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
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
        return 0;
}
