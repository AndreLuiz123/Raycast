#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.14159265359
#define PI2 PI/2
#define PI3 3*PI/2
#define DR 0.0174533

float px, py, pdx, pdy, pa;

int mapX = 8, mapY = 8, mapSize = 64;

int mapVector[] =
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,1,0,0,0,1,
    1,1,1,1,1,1,1,1
};

float dist(float ax, float ay, float bx, float by, float ang){
    return (sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
}

void drawRay3D()
{

    int r, mx, my, mp, dof, distT;
    float rx, ry, ra, xo, yo;

    ra = pa - DR*30;
    if(ra>2*PI){ra-=2*PI;} if(ra<0){ra+=2*PI;}

    ///HORIZONTAL LINES
    for(r = 0; r<60; r++)
    {

        dof = 0;
        float aTan = -1/tan(ra);
        float distH = 100000, hx = px, hy = py;
        if(ra>PI){ry = (((int)py>>6)<<6) -0.0001;rx = (py - ry)*aTan + px;yo = -64;xo = -yo*aTan;}
        if(ra<PI){ry = (((int)py>>6)<<6) +64;rx = (py - ry)*aTan + px;yo = 64;xo = -yo*aTan;}
        if(ra==0 || ra == PI){rx = px;ry = py;dof = 8;}

        while(dof < 8)
        {
            mx = (int)rx>>6;
            my = (int)ry>>6;
            mp = my*mapX + mx;
            if(mp < mapX*mapY && mp > 0 && mapVector[mp]==1){hx = rx; hy = ry; distH = dist(px,py,hx,hy,ra);dof = 8;}
            else {rx+=xo;ry+=yo;dof+=1;}
        }

        /*glColor3f(0,1,0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(px,py);
        glVertex2f(rx,ry);
        glEnd();*/

        dof = 0;
        float nTan = -tan(ra);
        float distV = 100000, vx = px, vy = py;
        if(ra>PI2 && ra<PI3){rx = (((int)px>>6)<<6) -0.0001;ry = (px - rx)*nTan + py;xo = -64;yo = -xo*nTan;}
        if(ra<PI2 || ra>PI3){rx = (((int)px>>6)<<6) +64;ry = (px - rx)*nTan + py;xo = 64;yo = -xo*nTan;}
        if(ra==0 || ra == PI){rx = px;ry = py;dof = 8;}

        while(dof < 8)
        {
            mx = (int)rx>>6;
            my = (int)ry>>6;
            mp = my*mapX + mx;
            if(mp < mapX*mapY && mp > 0 && mapVector[mp]==1){vx = rx; vy = ry; distV = dist(px,py,vx,vy,ra); dof=8;}
            else {rx+=xo;ry+=yo;dof+=1;}
        }

        if(distH<distV){rx = hx; ry = hy;distT = distH;glColor3f(1,0,0);}
        else{rx = vx; ry = vy;distT = distV; glColor3f(0.5f,0,0);}

        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(px,py);
        glVertex2f(rx,ry);
        glEnd();

        ///DRAW THE 3D
        float ca = pa - ra;  if(ca>2*PI){ca-=2*PI;} if(ca<0){ca+=2*PI;} distT = distT*cos(ca);
        float lineH = (mapSize*320)/distT; if(lineH>320){lineH = 320;}
        float lineO = 160 - lineH/2;
        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2f(r*8 + 530,lineO);
        glVertex2f(r*8 + 530,lineO + lineH);
        glEnd();

        ra+=DR;
        if(ra>2*PI){ra-=2*PI;} if(ra<0){ra+=2*PI;}
    }

}

void draw2DMap()
{

    int x, y, x0, y0;

    for(y=0; y<mapY; y++)
        for(x=0; x<mapX; x++)
        {
            if(mapVector[y*mapX + x] == 1)
            {
                glColor3f(1,1,1);
            }
            else
            {
                glColor3f(0,0,0);
            }
            x0 = x*mapSize;
            y0 = y*mapSize;
            glBegin(GL_QUADS);
            glVertex2d(x0+1,y0+1);
            glVertex2d(x0+1,y0+mapSize-1);
            glVertex2d(x0+mapSize-1,y0+mapSize-1);
            glVertex2d(x0+mapSize-1,y0+1);
            glEnd();
        }

}

void drawPlayer()
{

    glColor3f(1.0f, 1.0f, 0.0f);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2f(px,py);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2d(px,py);
    glVertex2d(px+pdx*5,py+pdy*5);
    glEnd();
}

void buttons(unsigned char key, int x, int y)
{

    if(key == 'w')
    {
        px+=pdx;
        py+=pdy;
    }
    if(key == 's')
    {
        px-=pdx;
        py-=pdy;
    }
    if(key == 'a')
    {
        pa-=0.1f;
        if(pa<0)
        {
            pa+=2*PI;
        }
        pdx = cos(pa)*5;
        pdy = sin(pa)*5;
    }
    if(key == 'd')
    {
        pa+=0.1f;
        if(pa>2*PI)
        {
            pa-=2*PI;
        }
        pdx = cos(pa)*5;
        pdy = sin(pa)*5;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    gluOrtho2D(0,1024,512,0);
    px = 300;
    py = 300;
    pdx = cos(pa)*5;
    pdy = sin(pa)*5;
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw2DMap();
    drawPlayer();
    drawRay3D();

    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1024,512);
    glutCreateWindow("Basic Ray Cast");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutMainLoop();
    return 0;
}
