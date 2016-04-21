/*
    Andrew Thomas
    CSCE 4230
    4/21/2016
    Program 6

    A simple OpenGL program demoing lighting on the triangle mesh of a bivariate function

    COMPILATION AND RUNNING: g++ prog6_Andrew_Thomas.cpp -o prog6_Andrew_Thomas -lGL -lGLU -lglut && ./prog6_Andrew_Thomas

    CONTROLS: x, X = rotate around x axis
              y, Y = rotate around y axis
              z, Z = zoom in/out
              d, D = scale Z values
              f, F = toggle polygon mode
              b, B = toggle bounding box
              e, E = increase/decrease shininess
              r, R = restore defaults
              s, S = toggle lighting mode
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

#define K 50
using namespace std;

//Transform and property variables
static int k = K;
static float xrot;
static float yrot;
static float zoom = 10;
static float bivariateScale = 1; //z-axis scalar
static float shininess = 1; //Shininess scalar
static bool line = false; //Set polygonmode to line if true
static bool boundBox = false; //Draw bounding box if true
static float zMin; //Min z value (for bounding box)
static float zMax;//Max z value (for bounding box)
static bool flat = false; //Use flat shading if true

//Triangle drawing variables
static float tn[2 * (K * K)];         //Triangle normals
static int ltri[2 * (K * K)][3];      //List of triangles
static float v[(K + 1) * (K + 1)][3]; //Vertex coordinates
static float vn[(K + 1) * (K + 1)][3]; //Vertex normals
static float h = 1.0/K;
static float j;
static float i;
static float x;
static float y;
static int i1 = 0;
static int i2 = 0;
static int i3 = 0;
static int indv = 0;
static int indt = 0;


void init(void)
{
        //const GLint* i = 1;
        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel (GL_SMOOTH);

        //Enable lighting for the front of the mesh
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        GLfloat lightColor0[] = {1, 1, 1, 0.0};
        GLfloat lightPos0[] = {10.0f, 0.0f, 10.0f, 1.0f}; //Positioned at (5, 0, 5)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
        glEnable(GL_DEPTH_TEST);
}

float bivariate(float x, float y)
{
        return (.5*exp(-.04*sqrt(pow(80 * x -40, 2) + pow(90 * y-45, 2))) * cos(0.15*sqrt(pow(80 * x-40, 2) + pow(90 * y-45, 2))));
}

void restoreDefaults()
{
  xrot = 0;
  yrot = 0;
  zoom = 10;
  bivariateScale = 1;
  shininess = 1;
  line = false;
  boundBox = false;
  flat = false;
}

//Create and store triangles
void createTriangles()
{
        indt = 0;
        indv = 0;
        zMin = 10000;
        zMax = 0;
        //Store verices
        for (j = 0; j <=k; j++)
        {
                y = j * h;
                for (i = 0; i <=k; i++)
                {
                        x = i*h;
                        v[indv][0] = x - .5;
                        v[indv][1] = y - .5;
                        v[indv][2] = bivariateScale * bivariate(x,y);

                        if(v[indv][2] < zMin)
                        {
                                zMin = v[indv][2];
                        }
                        if(v[indv][2] > zMax)
                        {
                                zMax = v[indv][2];
                        }

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
}

void display(void)
{
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f (1.0, 1.0, 1.0);

        glPushMatrix();

        glRotatef (yrot, 0.0, 1.0, 0.0);
        glRotatef (xrot, 1.0, 0.0, 0.0);

        //Set polygon mode and shading mode
        if(line)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(flat)
                glShadeModel (GL_FLAT);
        else
                glShadeModel (GL_SMOOTH);


        //Enable material and set shininess
        GLfloat whiteSpecularMaterial[] = {.1f, .1f, .1f, .1f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glEnable ( GL_COLOR_MATERIAL );

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

        if(boundBox)
        {
                glDisable(GL_LIGHTING);
                glColor3f (0.0, 1.0, 1.0);
                glBegin(GL_LINE_LOOP);

                glVertex3f(-0.5, -0.5, zMin); // P1
                glVertex3f(-0.5,  0.5, zMin); // P2
                glVertex3f(0.5,  0.5, zMin); // P3
                glVertex3f(0.5, -0.5, zMin); // P4

                glEnd();
                glBegin(GL_LINE_LOOP);
                glVertex3f(-0.5, -0.5, zMax); // P1
                glVertex3f(-0.5,  0.5, zMax); // P2
                glVertex3f(0.5,  0.5, zMax); // P3
                glVertex3f(0.5, -0.5, zMax); // P4

                glEnd();
                glBegin(GL_LINES);

                glVertex3f(-0.5, -0.5, zMin); // P1
                glVertex3f(-0.5, -0.5, zMax); // P1
                glVertex3f(-0.5,  0.5, zMin); // P2
                glVertex3f(-0.5,  0.5, zMax); // P2
                glVertex3f(0.5,  0.5, zMin); // P3
                glVertex3f(0.5,  0.5, zMax); // P3
                glVertex3f(0.5, -0.5, zMin); // P4
                glVertex3f(0.5, -0.5, zMax); // P4

                glEnd();

                glEnable(GL_LIGHTING);
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
        case 'd':
                bivariateScale *= .5;
                createTriangles();
                glutPostRedisplay();
                break;
        case 'D':
                bivariateScale *= 2;
                createTriangles();
                glutPostRedisplay();
                break;
        case 'f':
        case 'F':
                if(line)
                        line = false;
                else
                        line = true;
                glutPostRedisplay();
                break;
        case 'b':
        case 'B':
                if(boundBox)
                        boundBox = false;
                else
                        boundBox = true;
                glutPostRedisplay();
                break;
        case 'e':
                shininess *= .5;
                if(shininess < 0)
                    shininess = 0;
                glutPostRedisplay();
                break;
        case 'E':
                shininess *= 2;
                if(shininess > 128)
                    shininess = 128;
                glutPostRedisplay();
                break;
        case 'r':
        case 'R':
                restoreDefaults();
                glutPostRedisplay();
                break;
        case 's':
        case 'S':
                if(flat)
                        flat = false;
                else
                        flat = true;
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
        glutAddMenuEntry("z: increase zoom", 'z');
        glutAddMenuEntry("Z: decrease zoom", 'Z');
        glutAddMenuEntry("d: decrease bivariate scale", 'd');
        glutAddMenuEntry("D: increase bivariate scale", 'D');
        glutAddMenuEntry("e: decrease shininess", 'e');
        glutAddMenuEntry("E: increase shininess", 'E');
        glutAddMenuEntry("f/F: toggle wireframe/polygon-fill", 'f');
        glutAddMenuEntry("s/S: toggle flat/smooth shading", 'S');
        glutAddMenuEntry("b/B: toggle bounding box", 'b');
        glutAddMenuEntry("r/R: restore defaults", 'r');
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
        createTriangles();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMainLoop();
        return 0;
}
