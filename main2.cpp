/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>dq
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static int slices = 16;
static int stacks = 16;
static float posZ = -100;
static float posY = 0;
static float posX = 0;
static float rotX = 0;
static float rotY = 0;
static float rotZ = 0;
static int mouseX = 0;
static int mouseY = 0;
static int mouseOldX = 0;
static int mouseOldY = 0;
static float val;
static bool ctrlpress;


typedef struct kaps {
    float posX;
    float posY;
    float posZ;
    float rotX;
    float rotY;
    float rotZ;
    struct kap * next;
} kap_t;

kap_t active;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{

    kap_t * fkap = NULL;
    fkap = malloc(sizeof(kap_t));
    fkap->posX = 0;
    fkap->posY = 0;
    fkap->posZ = 0;
    fkap->rotX = 0;
    fkap->rotY = 0;
    fkap->rotZ = 0;
    fkap->next = NULL;


    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    newKap(fkap);


    glutSwapBuffers();
}

static void newKap(kap_t kap) {
    glPushMatrix();

    glTranslatef(kap->posX, kap->posY, kap->posZ);
    glRotatef(kap->rotX, 1, 0, 0);
    glRotatef(kap->rotY, 0, 1, 0);
    glRotatef(kap->rotZ, 0, 0, 1);
    glScalef(15,3,1);
    glutSolidCube(1);

    glPopMatrix();

}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            posX--;
            break;

        case 'd':
            posX++;
            break;

        case 'z':
            posY++;
            break;

        case 's':
            posY--;
            break;

        case 'r':
            posZ++;
            break;
        case 'f':
            posZ--;
            break;

        case 'a':
            rotY-=3;
            break;
        case 'e':
            rotY+=3;
            break;
        case 'o':
            rotX-=3;
            break;
        case 'l':
            rotX+=3;
            break;
        case 'k':
            rotZ-=3;
            break;
        case 'm':
            rotZ+=3;
            break;
        case 'b':
            ctrlpress = true;
            break;

    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void cursor(int param1, int param2) {

    mouseOldX = param1;
    mouseOldY = param2;

}

static void test(int param1, int param2) {

    float pmX = param1 - mouseOldX;
    float pmY = param2 - mouseOldY;

    if (ctrlpress) {
        posX -= (pmX*(posZ/500))/10;
        posY += (pmY*(posZ/500))/10;
    } else {
        posX -= pmX*(posZ/500);
        posY += pmY*(posZ/500);
    }

}

static void rotateMouse (int param1, int param2) {

    float pmX = param1 - mouseOldX;
    float pmY = param2 - mouseOldY;

    if (ctrlpress) {
        rotY += pmX/10;
        rotZ -= pmY/10;
    } else {
        rotY += pmX;
        rotZ -= pmY;
    }

}

static void lastRotateMouse (int param1, int param2) {

    float pmX = param1 - mouseOldX;

    if (ctrlpress) {
        rotX += pmX/10;
    } else {
        rotX -= pmX;
    }

}

static void onClick(int button, int state, int param1, int param2) {

    mouseOldX = param1;
    mouseOldY = param2;



    if (ctrlpress) {
        if (button == 3) {
            posZ += 0.1;
        } else if (button == 4) {
            posZ -= 0.1;
        }
    } else {
        if (button == 3) {
            posZ ++;
        } else if (button == 4) {
            posZ--;
        }
    }

    if (button == 0) {
        glutMotionFunc(*test);
    } else if (button == 2) {
        glutMotionFunc(*rotateMouse);
    } else if (button == 1) {
        glutMotionFunc(*lastRotateMouse);
    }



}

static void keyup(unsigned char key, int x, int y) {
    ctrlpress = false;
}


const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyup);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);

    glutMouseFunc(*onClick);


    glutMainLoop();

    return EXIT_SUCCESS;
}
