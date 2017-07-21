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
#include <math.h>


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
    int color[3];
    struct kaps *next;
    struct kaps *prev;
} kap_t;

kap_t *active;

kap_t *fkap = NULL;

kap_t *lastkap;



/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void drawAxes (kap_t * kap) {

    double roll = kap->rotX;
    double teta = kap->rotY;
    double phi = kap->rotZ;

    glPushMatrix();
    glTranslatef((kap->posX), kap->posY, kap->posZ);
    glColor3d(1,0,0);
    glRotatef(roll, 1, 0, 0);
    glRotatef(phi, 0, 0, 1);
    glRotatef(teta, 0, 1, 0);
    glScalef(20,1,1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(kap->posX, kap->posY, kap->posZ);
    glColor3d(0,1,0);
    glRotatef(roll, 1, 0, 0);
    glRotatef(phi, 0, 0, 1);
    glRotatef(teta, 0, 1, 0);
    glScalef(1,20,1);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(kap->posX, kap->posY, (kap->posZ));
    glColor3d(0,0,1);
    glRotatef(roll, 1, 0, 0);
    glRotatef(phi, 0, 0, 1);
    glRotatef(teta, 0, 1, 0);
    glScalef(1,1,20);
    glutSolidCube(1);
    glPopMatrix();

}

static void newKap(kap_t * kap) {


   /* quat MyQuaternion;

    vec3 EulerAngles(kap->rotX, kap->rotY, kap->rotZ);
    MyQuaternion = quat(EulerAngles);
    MyQuaternion = gtx::quaternion::angleAxis(degrees(RotationAngle), RotationAxis);*/


    double pitch = kap->rotX;
    double teta = kap->rotY;
    double phi = kap->rotZ;

    double y = cos(teta); // Y
    double x1 = sin(teta);   //X
    double z = cos(phi);   //Z
    double x2 = -sin(phi);   //X
    double x = x1+x2;

    //drawAxes(kap);
    glPushMatrix();

    glColor3d(kap->color[0],kap->color[1],kap->color[2]);
    glTranslatef(kap->posX, kap->posY, kap->posZ);
    //glRotatef(kap->rotX, x, y, z);
    glRotatef(pitch, 1, 0, 0);
    glRotatef(teta, -sin(rotZ), cos(rotZ), 0);
    glRotatef(phi, 0, 0, 1);





    glScalef(15,3,1);
    glutSolidCube(1);

    glPopMatrix();

}

static void display(void)
{


    if (fkap == NULL) {
        fkap = (kap_t *)malloc(sizeof(kap_t));
        fkap->posX = 0;
        fkap->posY = 0;
        fkap->posZ = -60;
        fkap->rotX = 0;
        fkap->rotY = 0;
        fkap->rotZ = 0;
        fkap->color[0] = 1;
        fkap->color[1] = 1;
        fkap->next = fkap;
        fkap->prev = fkap;
        active = fkap;
        lastkap = fkap;
    }



    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glColor3d(1,0,0);

    kap_t *kdisplay = fkap;

    do {
        newKap(kdisplay);
        kdisplay = kdisplay->next;
    } while (kdisplay != fkap);

    glutSwapBuffers();
}




static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;

        // TRANSLATION
        case 'q':
            active->posX--;
            break;

        case 'd':
            active->posX++;
            break;

        case 'z':
            active->posY++;
            break;

        case 's':
            active->posY--;
            break;

        case 'a':
            active->posZ++;
            break;
        case 'e':
            active->posZ--;
            break;

        // ROTATION

        case 'u':
            active->rotY-=3;
            break;
        case 'j':
            active->rotY+=3;
            break;
        case 'o':
            active->rotX-=3;
            break;
        case 'l':
            active->rotX+=3;
            break;
        case 'k':
            active->rotZ-=3;
            break;
        case 'm':
            active->rotZ+=3;
            break;

        //NOUVELLE PIECE
        case '+':
            lastkap->next = (kap_t *)malloc(sizeof(kap_t));
            lastkap->next->posX = 10;
            lastkap->next->posY = 10;
            lastkap->next->posZ = -100;
            lastkap->next->rotX = 0;
            lastkap->next->rotY = 0;
            lastkap->next->rotZ = 0;
            lastkap->next->color[0] = 1;
            lastkap->next->next = fkap;
            lastkap->next->prev = lastkap;
            fkap->prev = lastkap->next;
            lastkap = lastkap->next;
            active->color[1] = 0;
            active = lastkap;
            active->color[1] = 1;
            break;

        //COPIE KAPLA

        case '*':
            lastkap->next = (kap_t *)malloc(sizeof(kap_t));
            lastkap->next->posX = active->posX;
            lastkap->next->posY = active->posY;
            lastkap->next->posZ = active->posZ;
            lastkap->next->rotX = active->rotX;
            lastkap->next->rotY = active->rotY;
            lastkap->next->rotZ = active->rotZ;
            lastkap->next->color[0] = 1;
            lastkap->next->next = fkap;
            lastkap->next->prev = lastkap;
            fkap->prev = lastkap->next;
            lastkap = lastkap->next;
            active->color[1] = 0;
            active = lastkap;

            active->color[1] = 1;
            break;


        //PREMIERE PIECE
        case 'w':
            active->color[1] = 0;
            active = fkap;
            active->color[1] = 1;
            break;
        //PIECE SUIVANTE
        case '6':
            active->color[1] = 0;
            active=active->next;
            /*if (active->next) {

            }
            else {
                active=fkap;
            }*/
            active->color[1] = 1;

        //SMOOTHIER CONTROL
        case 'b':
            ctrlpress = true;
            break;
        //PREV
        case '4':
            active->color[1] = 0;
            active=active->prev;
            /*if (active->next) {

            }
            else {
                active=fkap;
            }*/
            active->color[1] = 1;
            break;
        //SUPPR
        case '-':
            if (active == fkap) {
                if (active == lastkap) {
                    break;
                }

                fkap = active->next;
            }
            if (active == lastkap) {
                lastkap = active->prev;
            }
            active->next->prev = active->prev;
            active->prev->next = active->next;
            kap_t *temp = active;
            active = active->next;
            free(temp);
            active->color[1] = 1;
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
        active->posX -= (pmX*(active->posZ/500))/10;
        active->posY += (pmY*(active->posZ/500))/10;
    } else {
        active->posX -= pmX*(active->posZ/500);
        active->posY += pmY*(active->posZ/500);
    }

    mouseOldX = param1;
    mouseOldY = param2;

}

static void rotateMouse (int param1, int param2) {

    float pmX = param1 - mouseOldX;
    float pmY = param2 - mouseOldY;

    if (ctrlpress) {
        active->rotY += pmX/10;
        active->rotZ -= pmY/10;
    } else {
        active->rotY += pmX/1;
        active->rotZ -= pmY/1;
    }

    mouseOldX = param1;
    mouseOldY = param2;

}

static void lastRotateMouse (int param1, int param2) {

    float pmX = param1 - mouseOldX;

    if (ctrlpress) {
        active->rotX += pmX/10;
    } else {
        active->rotX -= pmX;
    }

    mouseOldX = param1;
    mouseOldY = param2;
}

static void onClick(int button, int state, int param1, int param2) {

    mouseOldX = param1;
    mouseOldY = param2;



    if (ctrlpress) {
        if (button == 3) {
            active->posZ += 0.1;
        } else if (button == 4) {
            active->posZ -= 0.1;
        }
    } else {
        if (button == 3) {
            active->posZ ++;
        } else if (button == 4) {
            active->posZ--;
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
