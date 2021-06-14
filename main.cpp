#include <bits/stdc++.h>
#include <windows.h>
#include "BmpLoader.h"
#include "wheel.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include<string>
#define fileRead freopen("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\input.txt","r",stdin);
#define fileWrite freopen("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\input.txt","w",stdout);
using namespace std;
unsigned int ID;
static float dd=.3;
static float lookX = -3.5;
static float lookZ = -81;
static float camera = 0;
static float score = 0;
static float final_score = 0;
const double PI = 3.14159265389;
static bool collision = false, hLight = false, light1 = false,light3=false;;
static float eyeX = 1, eyeY = 1, eyeZ = 8 ,rot =0;;
static float carX[8] = {-6.8, -6.7, -4.2, -4.5, 4.2, 2.5, 3, 4.2}, carZ[8] = {-100.8, -280.7, -400.2, -180.5, -34.2, -150.5, -55, -4.2};
static float carSpeed[8] = {-1.2, -.75, -0.35, +0.75, +0.25, -0.2, +0.25, 1.3};
static float obstLeftX[12], obstRightX[12], obstLeftZ[12], obstRightZ[12];
static float coinx[10],coiny[10],coinz[10];
static double point = 0;
static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},
    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7},
    {2,0,1,3},
    {7,5,4,6},
    {2,3,7,6},
    {1,0,4,5},
    {6,4,0,2}
};

static void on_resize(int width, int height)
{
    const float rat = 1.0*width /  height;
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-rat, rat, -1.0, 1.0, 2.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}
void drawCube(float colorRed, float colorGreen, float colorBlue, bool e=false)
{
    GLfloat no_mat[] = {0, 0, 0, 1.0};
    GLfloat mat_emission[] = {colorRed, colorGreen, colorBlue,1};
    GLfloat mat_ambient[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_diffuse[] = {colorRed,colorGreen,colorBlue,1};
    GLfloat mat_specular[] = {1,1,1,1};
    GLfloat mat_shininess[] = {60};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, e?mat_emission:no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);



    glBegin(GL_QUADS);
    for(GLint i = 0; i<6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

//        for(GLint j = 0; j<4; j++){
//            glVertex3fv(&v_cube[c_ind[i][j]][0]);
//        }

        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);

    }
    glEnd();
}

void LoadTexture(const char*filename, int rep = 1)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}



void axis(void)
{

    glBegin(GL_LINES);
    glColor3f (1.0, 0.0, 0.0); ///red is X
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(50.0, 0.0, 0.0);

    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 50.0, 0.0); /// green is Y

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 50.0); ///blue is Z
    glEnd();

}

void Floor(void)
{
    for(int i=0; i<15; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 5);
        glPushMatrix();
        glTranslatef(-100,-3,i*-100);
        glScaled(200,1,110);
        drawCube(.35,.75,.5);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
void Sky(void)
{
    for(int i=0; i<15; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 15);
        glPushMatrix();
        glTranslatef(100,100,50*100);
        glScaled(200,1,110);
        drawCube(.35,.75,.5);
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
void night(void)
{
   glClearColor(0.3, 0.3, 0.3, 1.0);
}
void footPath()
{
    for(int i=1; i<=300;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 3);
        ///Left FootPath
        glPushMatrix();
        glTranslatef(-17.5, -2.7, -5*i);
        glScalef(5.0, 1.0, 5.0);
        drawCube(1,1,1);
        glPopMatrix();

        ///Right FootPath
        glPushMatrix();
        glTranslatef(13.2, -2.7, -5*i);
        glScalef(5.0, 1.0, 5.0);
        drawCube(1,1,1);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}

void road()
{
    ///road
    for(int i=1; i<=150; i++){

        glPushMatrix();
        glTranslatef(-12,-2.9,-10*i);
        glScalef(25.0,1.0,10.0);
        drawCube(1,1,1);
        glPopMatrix();
    }
}
/*
void road(void)
{
    ///road
    for(int i=1; i<180; i++)
    {

       // glBindTexture(GL_TEXTURE_2D,1);
        //glEnable(GL_TEXTURE_2D);

        ///left
        glPushMatrix();
        glTranslatef(-12.5,-2.9,-10*i);
        //reff();
        glScaled(13.5,1,10);
        //drawCube(.6,.6,.6,1);
        glPopMatrix();

        ///right
        glPushMatrix();
        glTranslatef(2,-2.9,-10*i);
        //reff();
        glScaled(13.5,1,10);
        //drawCube(.6,.6,.6,1);
        glPopMatrix();

        ///line mid
        glPushMatrix();
        glTranslatef(1,-2.9,-10*i);
        //reff();
        glScaled(1,1,1);
        //drawCube(1,1,1);
        glPopMatrix();

        ///line left
        glPushMatrix();
        glTranslatef(-13.5,-2.9,-10*i);
        //reff();
        glScaled(1,1,1);
        drawCube(1,1,1);
        glPopMatrix();

        ///line right
        glPushMatrix();
        glTranslatef(15.5,-2.9,-10*i);
        //reff();
        glScaled(1,1,1);
        drawCube(1,1,1);
        glPopMatrix();

        //glDisable(GL_TEXTURE_2D);
    }
}
*/
void drawText(string str,float colorRed, float colorGreen, float colorBlue,int width=1, float val=0.5,int col=0)
{
    glPushMatrix();
    glLineWidth(width);


    glScalef(.004f*val,.004f*val,20);

    glPushMatrix();
    glScalef(0,0,20);
    drawCube(1, colorGreen, colorBlue);
    glPopMatrix();

    for (int c=0; c != str.size(); c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[c]);
    }
    glPopMatrix();

}

void light(float x, float y, float z)
{
    GLfloat no_light[] = {0, 0, 0, 1.0};
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1};
    GLfloat light_diffuse[] = {0.2, 0.2, 0.2, 1};
    GLfloat light_specular[] = {1, 1, 1, 1};
    GLfloat light_pos[] = { x+5, y, z, 1.0};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT, light1?light_ambient:no_light);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, light1?light_diffuse:no_light);
    glLightfv(GL_LIGHT0,GL_SPECULAR, light1?light_specular:no_light);
    glLightfv(GL_LIGHT0,GL_POSITION, light_pos);

}
GLfloat cut_off_x = 42.0;
void head_light(float x, float y, float z)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1, 1, 0, 1.0};
    GLfloat light_diffuse[]  = { 1, 1, 0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_pos[] = { x+5,y,z,1};

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, hLight?light_ambient:no_light);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, hLight?light_diffuse:no_light);
    glLightfv(GL_LIGHT1,GL_SPECULAR, hLight?light_specular:no_light);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos);

    GLfloat direction[] = {0, -1, -1, 1};

    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction );
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, cut_off_x );
}

void shop_light(float x, float y, float z)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1, 1, 1, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_pos[] = { x+5,y,z,1};

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light3?light_ambient:no_light);
    glLightfv(GL_LIGHT2,GL_DIFFUSE, light3?light_diffuse:no_light);
    glLightfv(GL_LIGHT2,GL_SPECULAR, light3?light_specular:no_light);
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos);

    GLfloat direction[] = {-1, -1, -1, 1};
    GLfloat cut_off = 180.0;
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction );
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, cut_off );
}


void building(void)
{
    int height1,height2,height3,height4;

    for(int i = 1; i<40; i++)
    {
        srand(i);
        height1 = (rand() % 10) + 10;
        srand(i+1);
        height2 = (rand() % 10) + 10;
        srand(i+2);
        height3 = (rand() % 10) + 10;
        srand(i+3);
        height4 = (rand() % 10) + 10;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 11);

        ///right side buildings
        glPushMatrix();
        glTranslatef(25,-4,-i*100);
        glScaled(10,height1*2,10);
        drawCube(1,1,1);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 12);

        glPushMatrix();
        glTranslatef(25,-4,(-i*100)+25);
        glScaled(10,height2*3,10);
        drawCube(.3,.8,.7);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        /*
        glPushMatrix();
        glTranslatef(15,-4,(-i*100)+50);
        glScaled(10,height4,10);
        drawCube(0,0,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(15,-4,(-i*100)+75);
        glScaled(10,height3,10);
        drawCube(1,1,0);
        glPopMatrix();

        */
        ///left side buildings

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 12);

        glPushMatrix();
        glTranslatef(-35,-4,-i*100);
        glScaled(10,height2*2,10);
        drawCube(.3,.8,.7);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 12);
        glPushMatrix();
        glTranslatef(-35,-4,(-i*100)+25);
        glScaled(10,height4*2,10);
        drawCube(1,1,1);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
        /*
        glPushMatrix();
        glTranslatef(-25,-4,(-i*100)+50);
        glScaled(10,height3,10);
        drawCube(0,0,1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-25,-4,(-i*100)+75);
        glScaled(10,height1,10);
        drawCube(1,1,0);
        glPopMatrix();
        */

    }
}
void obstacle()
{

    ///Left
    for(int i=1; i<=10;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i&1)?14:15);
        glPushMatrix();
        obstLeftX[i] = (i&1)?-7:-5;
        obstLeftZ[i] = -130*i;
        glTranslatef(obstLeftX[i], -2, obstLeftZ[i]);
        glScalef(1.5, 1.5, 0.7);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

    }
    ///Right
    for(int i=1;i<=10;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i&1)?15:14);
        glPushMatrix();
        obstRightX[i] = (i&1)?4:5;
        obstRightZ[i] = -100*i;
        glTranslatef(obstRightX[i], -2, obstRightZ[i]);
        glScalef(1.5, 1.5, 0.7);
        drawCube(1,1,1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}
typedef struct
{
float x;
float y;
}CIRCLE;

CIRCLE circle;

void createcircle (int k, int r, int h) {
    glBegin(GL_LINES);
    for (int i = 0; i < 180; i++)
    {
    circle.x = r * cos(i) - h;
    circle.y = r * sin(i) + k;
    glVertex3f(circle.x + k,circle.y - h,0);

    circle.x = r * cos(i + 0.1) - h;
    circle.y = r * sin(i + 0.1) + k;
    glVertex3f(circle.x + k,circle.y - h,0);
    }
    glEnd();
}
void CoinCall()
{

    ///Left
    for(int i=1; i<=8;i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, (i&1)?4:5);
        glPushMatrix();
        coinx[i] = (i&1)?-5:7;
        coinz[i] = -130*i;
        glTranslatef(coinx[i], -2, coinz[i]);
        glScalef(1.5, 1.5, 0.7);

        createcircle(3,3,3);
        //glutSolidSphere(4,4,4);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}
static float rotation =0;
void car(void)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 16);
    glPushMatrix();
    glRotated(rotation,1,0,0);
    ///Front Left Wheel
    glPushMatrix();
    glTranslatef(-0.6, -0.8, -1);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Front Right Wheel
    glPushMatrix();
    glTranslatef(1.4, -0.8, -1);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Back Left Wheel
    glPushMatrix();
    glTranslatef(-0.6, -0.8, 2);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Back Right Wheel
    glPushMatrix();
    glTranslatef(1.4, -0.8, 2);
    glRotatef(rot, 1, 0, 0);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    head_light(0,2,-3);
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);

    ///Front Left Light
    glPushMatrix();
    glTranslatef(0.2, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, hLight?true:false);
    glPopMatrix();

    ///Front Right Light
    glPushMatrix();
    glTranslatef(2-0.5, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, hLight?true:false);
    glPopMatrix();

    ///lowerPart
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,17);
    glPushMatrix();
    glTranslatef(0, -0.8, 0 - 2);
    glScalef(2, 0.8, 5);
    drawCube(1, 1, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///upperPart
    glPushMatrix();
    glTranslatef(0, 0, 0 - 0.4);
    glScalef(2, 0.55, 2);
    drawCube(1, 1, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 18);
    ///backGlass

    glPushMatrix();
    glTranslatef(0, 0, 0 + 1.95);
    glRotatef(-35,1,0,0);
    glScalef(2, 0.65, 0.01);
    drawCube(1, 1, 1);
    glPopMatrix();
    ///FrontGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 - 1);
    glRotatef(45, 1,0,0);
    glScalef(2,0.8, 0.01);
    drawCube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    ///Signal Light Left
    glPushMatrix();
    glTranslatef(0.65, -0.7, 0 + 3.1);
    glScalef(.8, .05, .01);
    drawCube(1, 0, 0, hLight?true:false);
    glPopMatrix();

    ///

    glPushMatrix();
    glTranslatef(0.65, -0.5, 0 + 3.1);
    glScalef(.8, .05, .01);
    drawCube(1, 0, 0, hLight?true:false);
    glPopMatrix();

    glPopMatrix();


    glPopMatrix();
}
void car_temp(void)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 16);

    ///Front Left Wheel
    glPushMatrix();
    glTranslatef(-0.6, -0.8, -1);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Front Right Wheel
    glPushMatrix();
    glTranslatef(1.4, -0.8, -1);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Back Left Wheel
    glPushMatrix();
    glTranslatef(-0.6, -0.8, 2);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();

    ///Back Right Wheel
    glPushMatrix();
    glTranslatef(1.4, -0.8, 2);
    glRotatef(rot, 1, 0, 0);
    glScalef(0.1,0.15,0.15);
    wheel();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    //head_light(0,2,-3);
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,16);

    ///Front Left Light
    glPushMatrix();
    glTranslatef(0.2, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, 0);
    glPopMatrix();

    ///Front Right Light
    glPushMatrix();
    glTranslatef(2-0.5, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, 0);
    glPopMatrix();

    ///lowerPart
    glPushMatrix();
    glTranslatef(0, -0.8, 0 - 2);
    glScalef(2, 0.8, 5);
    drawCube(1, 1, 1);
    glPopMatrix();

    ///upperPart
    glPushMatrix();
    glTranslatef(0, 0, 0 - 0.4);
    glScalef(2, 0.55, 2);
    drawCube(1, 1, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 16);
    ///backGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 + 1.95);
    glRotatef(-35,1,0,0);
    glScalef(2, 0.65, 0.01);
    drawCube(1, 1, 1);
    glPopMatrix();
    ///FrontGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 - 1);
    glRotatef(45, 1,0,0);
    glScalef(2,0.8, 0.01);
    drawCube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


    ///Signal Light Left
    glPushMatrix();
    glTranslatef(0, -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0, 0);
    glPopMatrix();

    ///Signal Light Right
    glPushMatrix();
    glTranslatef(0 + 2 - 0.3 , -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0, 0);
    glPopMatrix();

    glPopMatrix();
}

/*
void car(void)
{
    ///CAR
    /*
    glPushMatrix();
    glTranslatef(-1+lookX,-2,-2+lookZ);
    glScaled(1.5,.5,4.0);
    drawCube(26.1/255,82.1/255,118.1/255);
    glPopMatrix();

    glPushMatrix();
    glTranslated(-1+lookX+.01,-1+.17,.1+lookZ);
    glRotated(65,.6,0,0);
    glScaled(1.5,1,1.1);
    drawCube(26.1/255,82.1/255,118.1/255);///0.847059 , 0.74902 , 0.847059
    glPopMatrix();
    */

    /*

    ///signal light
    glPushMatrix();
    glTranslatef(-1+lookX,-1.25,-1.9+lookZ);
    glScaled(.4,.2,4);
    drawCube(1.000, 0.271, 0.000);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.6+lookX,-1.25,-1.9+lookZ);
    glScaled(.4,.2,4);
    drawCube(1.000, 0.271, 0.000);
    glPopMatrix();




    ///numberplate
    glPushMatrix();
    glTranslatef(lookX - .3 , -1.6 , lookZ-1.9);
    glScaled(.5,.3,4);
    drawCube(0.902, 0.902, 0.980);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-1+lookX,-1.8,-1.9+lookZ);
    glScaled(2,.1,4);
    drawCube(0.902, 0.902, 0.980);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-1+.05+lookX,-1,-1.1+lookZ);
    glScaled(1.9,.7,2);
    drawCube(0.902, 0.902, 0.980);
    glPopMatrix();
    */



    /*

    glPushMatrix();
    glTranslatef(-1+lookX,-1.8,-1.9+lookZ);
    glScaled(1.5,.1,4);
    drawCube(0.902, 0.902, 0.980);
    glPopMatrix();




    ///back glass
    glPushMatrix();
    glTranslated(-1+lookX,-1+.07,-1.1+lookZ);
    glScaled(1.5,.6,2.1);
    drawCube(0.184, 0.310, 0.31);
    glPopMatrix();





    head_light(0,2,-3);
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);

    ///Front Left Light
    glPushMatrix();
    glTranslatef(0.2, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, hLight?true:false);
    glPopMatrix();

    ///Front Right Light
    glPushMatrix();
    glTranslatef(2-0.5, -0.4, -2.02);
    glScalef(0.3, 0.25, 0.1);
    drawCube(0.9, 0.5, 0.5, hLight?true:false);
    glPopMatrix();

    ///lowerPart
    glPushMatrix();
    glTranslatef(0, -0.8, 0 - 2);
    glScalef(2, 0.8, 5);
    drawCube(1, 1, 1);
    glPopMatrix();

    ///upperPart
    glPushMatrix();
    glTranslatef(0, 0, 0 - 0.4);
    glScalef(2, 0.55, 2);
    drawCube(1, 1, 1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 11);
    ///backGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 + 1.95);
    glRotatef(-35,1,0,0);
    glScalef(2, 0.65, 0.01);
    drawCube(1, 1, 1);
    glPopMatrix();
    ///FrontGlass
    glPushMatrix();
    glTranslatef(0, 0, 0 - 1);
    glRotatef(45, 1,0,0);
    glScalef(2,0.8, 0.01);
    drawCube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


    ///Signal Light Left
    glPushMatrix();
    glTranslatef(0, -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0, hLight?true:false);
    glPopMatrix();

    ///Signal Light Right
    glPushMatrix();
    glTranslatef(0 + 2 - 0.3 , -0.4, 0 + 3.1);
    glScalef(.3, .15, .01);
    drawCube(1, 0, 0, hLight?true:false);
    glPopMatrix();

    glPopMatrix();

}
*/
#include<string>
double bestScore()
{
    ifstream myfile;
    myfile.open ("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\input.txt");

    string word;
    myfile >> word;
    double lol = atof(word.c_str());
    myfile.close();
    return lol;
/*
    string x;
    myfile>>x;
    float num_float = stod(str);
    return num_float;

    int val=0;
    int len=x.length();
    for(int i=0;i<len;i++)
    {
        val=val*10+x[i]-48;
    }
    myfile.close();
    return val;
    */
}

void writeBest(double x)
{
    ofstream myfile;
    myfile.open ("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\input.txt");
    myfile <<x;
    myfile.close();
    return;
}
static void display(void)
{
    //freopen(stdin)




    glClearColor(dd, dd, dd, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    axis();

    if(lookZ<-850)
        lookZ = -80;

    if(lookZ>-80){
        lookZ=-850;
    }
    gluLookAt(lookX + eyeX, eyeY, eyeZ + lookZ, lookX, camera, lookZ-5, 0, 1, 0);
    //gluLookAt(lookX, 1, 8 + lookZ, lookX, camera, -10 + lookZ, 0, 1, 0);

    ///Start text Section

    stringstream ss,ss2;

    string s1 = ss.str();
    string s2 = ss2.str();


    ss.str(string());
    ss2.str(string());

    double bt=bestScore();
    ss2<<bt;
    ss<<point;
    if(point>=bt)
        bt=point;
    //ss<<lookZ;
    s1 ="Point:" + ss.str();
    s2="Best Score:"+ss2.str();
    //s1+=lookZ;

    glPushMatrix();
    glTranslatef(lookX, 3.3, lookZ);
    glRotatef(-2, 0, 0, 1);
    drawText(s1, 0, 0, 1, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(lookX, 3.7, lookZ);
    glRotatef(-2, 0, 0, 1);
    drawText(s2, 0, 0, 1, 3);
    glPopMatrix();
    ///End text section


    glPushMatrix();
    glTranslatef(lookX-2.3 , 10 , lookZ-5);
    glScalef(1,1,200);
    light(0,3,0);
    glPopMatrix();

    //glPushMatrix();
    //shop_light(lookX,3,lookZ -20);
    //glPopMatrix();

    glPushMatrix();
    glTranslatef(lookX-2.3 , 10 , lookZ-5);
    glScalef(1,1,200);
    light(0,3,0);
    glPopMatrix();

    //glPushMatrix();
    //shop_light(lookX,3,lookZ -20);
    //glPopMatrix();

    footPath();
    //Sky();
    Floor();
    //road();
    building();
    obstacle();

    for(int i=0;i<8;i++){
        glPushMatrix();
        glTranslatef(carX[i], -0.5, carZ[i]);
        car_temp();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(lookX, 0, lookZ);
    car();
    glPopMatrix();
    glPushMatrix();
    CoinCall();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,1);
        road();
    glDisable(GL_TEXTURE_2D);


    glutSwapBuffers();

}

int cnt_illegal=0;
bool carOn=false;
int cameraZoom = 0;
float increase_speed=.05;
static bool running = false, start = false;
static int cc=0;
static bool flagRotate=false;
static int tt_cnt=0;
static bool other_car=true;
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'e':
        carOn=!carOn;
        running = !running;
        start = true;
        lookZ = lookZ - 1;
        flagRotate=false;
        rotation=0;
        //lookZ = lookZ - 1;
        break;
    /*case 's':
        start = true;
        running = false;
        lookZ = lookZ + 0.3;
        increase_speed-=increase_speed+.05;
        //lookZ = lookZ + 0.1;
        break;
        */
    case 'i':
        increase_speed+=.05;
        break;
    case 'd':
        increase_speed-=.05;
        break;
    case 'c':
        cameraZoom++;
        cameraZoom%=3;
        cc++;
        cc%=2;
        if(cameraZoom==0)
            camera=0;
        else if(cameraZoom==1)
            camera=3;
        else
            camera=6;
            //eyeZ-=.5;
            if(cc==1)
                lookZ-=.5;
            else
                lookZ+=.5;
        break;
    case 'f':
        if ( lookX < 10.5)
        {
            lookX = lookX + 0.2;
            cnt_illegal=0;
        }
        else
        {
            lookX = lookX;
            cnt_illegal++;
            if(cnt_illegal==5)
            {
                carOn=false;
                lookX = 1.5;
                lookZ = -10;
                cnt_illegal=0;
            }
        }
        break;
    case 't':
        tt_cnt++;
        tt_cnt%=2;
        if(tt_cnt==1)
        {
            dd=.99;
            cut_off_x=360;
            light1=true;
            light3=true;
        }
        else
        {
            dd=.35;
            cut_off_x=42;
            light1=false;
            light3=false;
        }
        break;

    case 'a':
        if ( lookX > -11.5)
        {
            cnt_illegal=0;
            lookX = lookX - 0.2;
        }
        else
        {
            lookX = lookX;
            cnt_illegal++;
            if(cnt_illegal==5)
            {
                carOn=false;
                lookX = 1.5;
                lookZ = -10;
                cnt_illegal=0;
            }
        }
        break;

    case '1':
        if(tt_cnt==0)
        light1 = !light1;
        break;
    case '2':
        hLight = !hLight;
        break;
    case '3':
        light3 = !light3;
        break;
    case 'z':
        eyeZ-=0.5;
        break;
    case 'Z':
        eyeZ+=0.5;
        break;
    case 'x':
        eyeX-=0.5;
        break;
    case 'X':
        eyeX+=0.5;
        break;
    case 'y':
        eyeY-=0.5;
        break;
    case 'Y':
        eyeY+=0.5;
        break;
    }

    glutPostRedisplay();
}
static void idle(void)
{
    /*if (carOn == true)
	{
		lookZ = lookZ - increase_speed;
	}*/
	if(lookZ<-1300)
        lookZ = -80;
    else if(lookZ>-80){
        lookZ=-1300-1;
    }
    if(rot>=360) rot = 0;
    if(running)lookZ-=increase_speed , point+=abs(increase_speed-0), rot += 0.2;
    if(other_car){
        for(int i=0;i<8;i++){
            if(carZ[i]<-850)
                carZ[i] = -80;

            if(carZ[i]>-80){
                carZ[i]=-850;
            }
            if(carZ[i]<-1300)
                carZ[i] = -80 + i*5;
            else{
                carZ[i] -= carSpeed[i];
            }
        }
    }
    ///Collision detection
     for(int i=0;i<8;i++){
        if((carX[i]<=lookX+2 && carX[i]+2 >= lookX+2)&&(carZ[i]<=lookZ && carZ[i]+5>=lookZ)){
            lookX = lookX-0.3;
            lookZ = lookZ+1;
            collision = true;
        }
        else if((carX[i]<=lookX && carX[i]+2>=lookX) &&(carZ[i]<=lookZ && carZ[i]+5>=lookZ)){
            lookX = lookX+0.3;
            lookZ = lookZ+1;
            collision = true;
        }
        else if((carX[i]<=lookX && carX[i]+2>=lookX) &&(carZ[i]<=lookZ+5 && carZ[i]+5>=lookZ+5)){
            lookX = lookX+0.3;
            lookZ = lookZ - 1;
            collision = true;
        }
        else if((carX[i]>=lookX+2 && carX[i]+2<=lookX+2) &&(carZ[i]<=lookZ+5 && carZ[i]+5>=lookZ+5)){
            lookX = lookX-3;
            lookZ = lookZ - 1;
            collision = true;
        }
    }
    for(int i=1; i<=10; i++){
        if(((obstLeftX[i] <= lookX && obstLeftX[i]+1.5 >= lookX) || (obstLeftX[i] <= lookX+2 && obstLeftX[i]+1.5 >= lookX+2 )) && (obstLeftZ[i] <= lookZ && obstLeftZ[i]+0.7 >= lookZ)){
            lookX = lookX+0.3;
            lookZ = lookZ+2.5;
            collision = true;
        }
        else if(((obstLeftX[i] <= lookX && obstLeftX[i]+1.5 >= lookX)||(obstLeftX[i] <= lookX+2 && obstLeftX[i]+1.5 >= lookX+2 )) && (obstLeftZ[i] <= lookZ+5 && obstLeftZ[i]+0.7 >= lookZ+5)){
            lookX = lookX+0.3;
            lookZ = lookZ-1;
            collision = true;
        }
    }

    for(int i=1;i<=10;i++){
        if(((obstRightX[i] <= lookX && obstRightX[i]+1.5 >= lookX)||(obstRightX[i] <= lookX+2 && obstRightX[i]+1.5 >= lookX+2 )) && (obstRightZ[i] <= lookZ && obstRightZ[i]+0.7 >= lookZ)){
            lookX = lookX-0.3;
            lookZ = lookZ+2.5;
            collision = true;
        }
        else if(((obstRightX[i] <= lookX && obstRightX[i]+1.5 >= lookX)||(obstRightX[i] <= lookX+2 && obstRightX[i]+1.5 >= lookX+2 )) && (obstRightZ[i] <= lookZ+5 && obstRightZ[i]+0.7 >= lookZ+5)){
            lookX = lookX-0.3;
            lookZ = lookZ-1;
            collision = true;
        }
    }
    int bt=bestScore();
    if(point>=bt)
        writeBest(point);

    if(collision){
        point=0;
        //rotation+=50;
        flagRotate=true;
        running = false;
    }
    if(flagRotate)
    {
        rotation+=3;
        if(rotation>=360)
        {
            rotation=0;
            collision=false;
            flagRotate=false;
            running=true;
        }
    }
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(620,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition (100, 100);


    glutCreateWindow("Car Ride");

    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\rrr.bmp",1);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\tin.bmp",2);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\path.bmp",3);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\pool.bmp",4);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\field1.bmp",5);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\wall.bmp",6);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\shopFront.bmp",7);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\shopFront2.bmp",8);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\houseWall.bmp",9);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\gateDoor.bmp",10);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\build1.bmp",11);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\build2.bmp",12);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\build3.bmp",13);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\rock.bmp",14);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\sky.bmp",15);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\car_back.bmp",16);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\back.bmp",17);
    LoadTexture("F:\\Last\\Graphics Lab\\Project\\Mine\\Practice1\\images\\dog.bmp",18);

    glutReshapeFunc(on_resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);


    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);


    glutMainLoop();

    return EXIT_SUCCESS;
}
