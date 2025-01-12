/*
-------------------------------- Library --------------------------------
*/
#include <iostream>
#include <GL/glut.h>
#include <math.h>

struct Movement
{
    float rotate;
} cube;

bool isMoving = true;
bool is2DMode = false;
bool hidden = false;
float lightPos[] = {0.0f, 10.0f, 0.0f, 0.0f};
float aspectRatio = 1.0f;
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
void myKeyboard(unsigned char key, int x, int y);

// Fungsi utama
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Playstation 2");

    init3D();

    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard); // Daftarkan fungsi keyboard
    glutTimerFunc(1000 / 60, update, 0);

    glutMainLoop();
    return 0;
}

// Fungsi untuk menggambar garis sumbu koordinat (cartesius)
void carte()
{
    glColor3f(1.0, 0.0, 0.0); // X-axis (merah)
    glBegin(GL_LINES);
    glVertex3f(-1000.0, 0.0, 0.0);
    glVertex3f(1000.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0); // Y-axis (hijau)
    glVertex3f(0.0, -1000.0, 0.0);
    glVertex3f(0.0, 1000.0, 0.0);

    glColor3f(0.0, 0.0, 1.0); // Z-axis (biru)
    glVertex3f(0.0, 0.0, -1000.0);
    glVertex3f(0.0, 0.0, 1000.0);
    glEnd();
}

// Fungsi keyboard untuk mengontrol program
void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'c': // Toggle menampilkan/menyembunyikan garis sumbu
        hidden = !hidden; // Ubah status hidden
        break;
    case 27: // Kode ASCII untuk tombol ESC
        exit(0); // Keluar dari program
        break;
    }
    glutPostRedisplay(); // Meminta tampilan untuk di-render ulang
}

// Fungsi utama untuk menggambar tampilan
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, aspectRatio, 1.0, 1000.0);

    if (cameraYPos <= 90)
    {
        reduceYPos += 0.1;
    }
    gluLookAt(5.0, cameraYPos -= reduceYPos, 5.0, 0.0, -10.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);

    if (!hidden) // Hanya tampilkan garis sumbu jika tidak disembunyikan
    {
        carte();
    }

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

// Fungsi untuk inisialisasi 3D
void init3D()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glClearColor(0.1, 0.1, 0.1, 1.0);
}

// Fungsi untuk memperbarui rotasi dan posisi
void update(int value)
{
    if (!is2DMode && isMoving)
    {
        cube.rotate += 1.2;
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, update, 0);
}

// Fungsi untuk menggambar loading screen
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

// Fungsi untuk menggambar objek 2D
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

// Fungsi untuk menggambar kotak
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