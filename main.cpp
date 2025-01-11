/*
-------------------------------- Library --------------------------------
*/
#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
using namespace std;
struct Movement
{
    float rotate;
} cube;

bool isMoving = true;
bool is2DMode = false;
float lightPos[] = {0.0f, 10.0f, 0.0f, 0.0f};
float aspectRatio = 1.0f; // Menyimpan aspect ratio layar
float cameraYPos = 100;
float reduceYPos = 0.05;
float rotate = 0.0;

// Parameter Scene 2 (Lingkaran)
const int num_points = 8;      // Jumlah titik pada lingkaran
const float radius = 2.0f;     // Radius lingkaran
const float omega = 2.0f;      // Kecepatan rotasi
const float glow_speed = 4.0f; // Kecepatan glow

/*
-------------------------- Prototipe Function ---------------------------
*/
void init3D();
void display();
void loadingScreen();
void kotak(float x, float y, float z, float size);
void object2D();
void createMenu();
void carte();
void update(int value);
void processMenu(int option);
void drawConnectingLines(float time, float x_offset);
void drawPoint(float x, float y, float intensity);
void drawText(float x, float y, const std::string &text, void *font, float r, float g, float b);
/*
--------------------------- Rest Of Function -----------------------------
*/
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1366, 768);
    glutCreateWindow("Playstation 2");
    init3D();
    glutDisplayFunc(display);
    glutTimerFunc(1000 / 60, update, 0);
    glutMainLoop();
    return 0;
}

void carte()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(-1000.0, 0.0, 0.0);
	glVertex3f(1000.0, 0.0, 0.0);
	
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1000.0, 0.0);
	glVertex3f(0.0, 1000.0, 0.0);
	
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1000.0);
	glVertex3f(0.0, 0.0, 1000.0);
	glEnd();
}

void kotak(float x, float y, float z, float size)
{
	glPushMatrix();
	glTranslatef(x, y, z);
    glRotated(-40.0, 0.0, 1.0, 0.0);
    glColor3ub(255, 255, 255);
    glScalef(1.0f, size, 1.0f);
    glutSolidCube(5.0);
    glPopMatrix();
}

void ekor(float startX, float startY, float startZ, float length, int segments)
{
    float segmentLength = length / segments;
    for (int i = 0; i < segments; i++)
    {
        float alpha = 1.0f - (float)i / segments;
        float offsetX = (rand() % 100 - 50) / 100.0f; // Randomize x-offset
        float offsetY = (rand() % 100 - 50) / 100.0f; // Randomize y-offset
        float offsetZ = (rand() % 100 - 50) / 100.0f; // Randomize z-offset

        glColor4f(1.0f, 0.5f + 0.5f * alpha, 0.0f, alpha); // Firework-like color
        glPushMatrix();
        glTranslatef(startX + i * segmentLength * 0.8f + offsetX, startY + i * segmentLength * 0.5f + offsetY, startZ - i * segmentLength + offsetZ);
        glutSolidSphere(1.0f, 10, 10); // Use sphere for a firework particle effect
        glPopMatrix();
    }
}
// Fungsi untuk menggambar teks
void loadingScreen()
{
    glPushMatrix();
    /*
    -------------------------------- Cube --------------------------------
    */
	glRotated(rotate -= 0.03, 0, 1, 0);
    kotak(0, 0, 0, 5);

    kotak(-10, 0, 5, 10);
    kotak(10, 0, -5, 10);
    kotak(5, 0, 10, 10);
    kotak(-5, 0, -10, 10);

    kotak(-20, 0, 10, 10);
    kotak(20, 0, -10, 10);
    kotak(-15, 0, -15, 10);
    kotak(15, 0, 15, 10);
    
    kotak(-5, -10, 10, 10);
    kotak(5, 15, -15, 10);
    kotak(-10, -15, 5, 10);
    kotak(15, -10, -20, 10);
    kotak(5, -5, 20, 10);
    kotak(-20, 20, -15, 10);
    kotak(25, -15, 10, 10);
    kotak(-25, 5, -10, 10);
    
    glPushMatrix();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(0.0, 20.0, 0.0);
    glRotated(cube.rotate, 1.0, 1.0, 1.0);
    glColor4ub(128, 128, 128, 128);
    glutSolidCube(10.0);

    glDisable(GL_BLEND);
    glPopMatrix();
    
    glPopMatrix();
}

void object2D()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float x_offset = -2.0f;

    // Gambar garis penghubung
    drawConnectingLines(time, x_offset);

    // Gambar titik-titik
    for (int i = 0; i < num_points; ++i)
    {
        float theta = 2.0f * M_PI * i / num_points;
        float x = x_offset + radius * cos(theta + omega * time);
        float y = radius * sin(theta + omega * time);

        float intensity = 0.5f + 0.5f * sin(glow_speed * time + theta);

        drawPoint(x, y, intensity);
    }

    float text_x = 1.5f;
    drawText(text_x, 0.5f, "Browser", GLUT_BITMAP_HELVETICA_18, 0.6f, 0.8f, 1.0f);
    drawText(text_x, 0.0f, "System Configuration", GLUT_BITMAP_HELVETICA_12, 0.6f, 0.8f, 1.0f);

    glutPostRedisplay();

    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1.0, 1.0, 1000.0);
    if (cameraYPos <= 90)
    {
    	reduceYPos += 0.1;
	}
    gluLookAt(5.0, cameraYPos -= reduceYPos, 5.0, 0.0, -10.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    carte();
    if (!is2DMode && cameraYPos <= 10) 
    {
    	is2DMode = true;
	}
    if (is2DMode)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-5, 5, -5, 5);
        glMatrixMode(GL_MODELVIEW);

        object2D();
    }
    else
    {
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        loadingScreen();
    }

    glutSwapBuffers();
}

void init3D()
{
    /*-------------------------------- 3D Init --------------------------------*/

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    /*-------------------------------- Pencahayaan --------------------------------*/

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    /*-------------------------------- ViewPort --------------------------------*/

    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void processMenu(int option)
{
    switch (option)
    {
    case 1:
        is2DMode = false;
        init3D();
        break;
    case 2:
        is2DMode = true;
        break;
    }
    glutPostRedisplay();
}

void update(int value)
{
    if (!is2DMode)
    {
        if (isMoving)
        {
            cube.rotate += 1.2;
        }
        glutPostRedisplay();
    }
    glutTimerFunc(1000 / 60, update, 0);
}

void drawPoint(float x, float y, float intensity)
{
    glColor4f(0.2f + 0.8f * intensity, 0.4f + 0.6f * intensity, 1.0f, 1.0f);
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

// Fungsi untuk menggambar garis yang menghubungkan titik-titik
void drawConnectingLines(float time, float x_offset)
{
    glColor4f(0.2f, 0.4f, 1.0f, 0.5f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_points; ++i)
    {
        float theta = 2.0f * M_PI * i / num_points;
        float x = x_offset + radius * cos(theta + omega * time);
        float y = radius * sin(theta + omega * time);
        glVertex2f(x, y);
    }
    glEnd();
}

// Fungsi untuk menggambar teks
void drawText(float x, float y, const std::string &text, void *font, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(font, c);
    }
}
