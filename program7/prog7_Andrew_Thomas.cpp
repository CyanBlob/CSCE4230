/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  bezcurve.c			
 *  This program uses evaluators to draw a Bezier curve.
 */
#include <GL/glut.h>
#include <stdlib.h>

static GLfloat p0[3] = {-9.0, -9.0, 0.0};
static GLfloat p1[3] = {-5.0, -8.0, 0.0};
static GLfloat p2[3] = {-1.0, -6.0, 0.0};
static GLfloat p3[3] = {-0.0, -0.0, 0.0};
static GLfloat p4[3] = {1.0,   6.0, 0.0};
static GLfloat p5[3] = {5.0,   8.0, 0.0};
static GLfloat p6[3] = {9.0,   9.0, 0.0};

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

static float zoom = 10;

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 8, &ctrlpoints[0][0]);
   //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints2[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
   int i;
   int numPoints = 100;

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
   
   //Draw second curve (hopefully)
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

   glFlush();

}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-zoom, zoom, -zoom*(GLfloat)h/(GLfloat)w, 
               zoom*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-zoom*(GLfloat)w/(GLfloat)h, 
               zoom*(GLfloat)w/(GLfloat)h, -zoom, zoom, -5.0, 5.0);
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
    }
}

void createMenu(void)
{
    glutCreateMenu(menu);

    glutAddMenuEntry("z: Zoom in",'z');
    glutAddMenuEntry("z: Zoom out",'Z');

}

void keyboard(unsigned char key, int x, int y)
{
   //Shut up, Clang
   x++;
   y++;
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}