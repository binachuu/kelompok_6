#include <GL/glut.h>
#include <cmath>
#include <string>

// Parameter Umum
int currentScene = 1; // Scene yang sedang aktif (1 = Loading Screen, 2 = Browser)
float aspectRatio = 1.0f; // Menyimpan aspect ratio layar

// Parameter Scene 1 (Kotak-Kotak)
const int num_boxes = 6;
float box_positions[num_boxes][2];
float box_speeds[num_boxes][2];

// Parameter Scene 2 (Lingkaran)
const int num_points = 8; // Jumlah titik pada lingkaran
const float radius = 2.0f; // Radius lingkaran
const float omega = 2.0f; // Kecepatan rotasi
const float glow_speed = 4.0f; // Kecepatan glow

// Fungsi untuk menggambar kotak
void drawBox(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();
}

// Fungsi untuk menggambar satu titik dengan efek glow
void drawPoint(float x, float y, float intensity) {
    glColor4f(0.2f + 0.8f * intensity, 0.4f + 0.6f * intensity, 1.0f, 1.0f);
    glPointSize(12.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

// Fungsi untuk menggambar garis yang menghubungkan titik-titik
void drawConnectingLines(float time, float x_offset) {
    glColor4f(0.2f, 0.4f, 1.0f, 0.5f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_points; ++i) {
        float theta = 2.0f * M_PI * i / num_points;
        float x = x_offset + radius * cos(theta + omega * time);
        float y = radius * sin(theta + omega * time);
        glVertex2f(x, y);
    }
    glEnd();
}

// Fungsi untuk menggambar teks
void drawText(float x, float y, const std::string& text, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

// Scene 1: Loading Screen
void drawScene1() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Update posisi kotak
    for (int i = 0; i < num_boxes; ++i) {
        box_positions[i][0] += box_speeds[i][0];
        box_positions[i][1] += box_speeds[i][1];

        // Pantulan dari batas layar
        if (box_positions[i][0] > 4.5f || box_positions[i][0] < -4.5f) box_speeds[i][0] = -box_speeds[i][0];
        if (box_positions[i][1] > 4.5f || box_positions[i][1] < -4.5f) box_speeds[i][1] = -box_speeds[i][1];

        // Gambar kotak
        drawBox(box_positions[i][0], box_positions[i][1], 1.0f, 0.2f, 0.6f, 1.0f);
    }

    glutSwapBuffers();
}

// Scene 2: Browser
void drawScene2() {
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float x_offset = -2.0f;

    // Gambar garis penghubung
    drawConnectingLines(time, x_offset);

    // Gambar titik-titik
    for (int i = 0; i < num_points; ++i) {
        float theta = 2.0f * M_PI * i / num_points;
        float x = x_offset + radius * cos(theta + omega * time);
        float y = radius * sin(theta + omega * time);

        // Efek glow
        float intensity = 0.5f + 0.5f * sin(glow_speed * time + theta);

        drawPoint(x, y, intensity);
    }

    // Gambar teks
    float text_x = 1.5f; // Disesuaikan agar lebih ke kanan
    drawText(text_x, 0.5f, "Browser", GLUT_BITMAP_HELVETICA_18, 0.6f, 0.8f, 1.0f);
    drawText(text_x, 0.0f, "System Configuration", GLUT_BITMAP_HELVETICA_12, 0.6f, 0.8f, 1.0f);

    glutSwapBuffers();
}

// Callback display
void display() {
    if (currentScene == 1) {
        drawScene1();
    } else if (currentScene == 2) {
        drawScene2();
    }
}

// Callback untuk keyboard
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        // Pindah scene saat spasi ditekan
        currentScene = (currentScene == 1) ? 2 : 1;
    } else if (key == 27) {
        // Keluar program saat tombol ESC ditekan
        exit(0);
    }
}

// Callback untuk menyesuaikan ukuran jendela (aspect ratio)
void reshape(int w, int h) {
    aspectRatio = (float)w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (aspectRatio >= 1.0f) {
        // Layar lebih lebar, sesuaikan horizontal
        gluOrtho2D(-5.0f * aspectRatio, 5.0f * aspectRatio, -5.0f, 5.0f);
    } else {
        // Layar lebih tinggi, sesuaikan vertikal
        gluOrtho2D(-5.0f, 5.0f, -5.0f / aspectRatio, 5.0f / aspectRatio);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Inisialisasi
void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Inisialisasi posisi dan kecepatan kotak untuk Scene 1
    for (int i = 0; i < num_boxes; ++i) {
        box_positions[i][0] = -4.0f + i * 1.5f;
        box_positions[i][1] = -4.0f + i * 0.8f;
        box_speeds[i][0] = 0.03f + i * 0.01f;
        box_speeds[i][1] = 0.02f + i * 0.01f;
    }
}

// Fungsi idle untuk animasi
void idle() {
    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("PS2 Loading & Browser Simulation");

    // Set fullscreen mode
    glutFullScreen();

    initOpenGL();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}