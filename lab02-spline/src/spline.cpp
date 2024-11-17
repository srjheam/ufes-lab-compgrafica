#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GLSR/window.hpp"

#include "srutil/macros.hpp"

#define TAMANHO_JANELA 500

#define ZOOM_MIN_SIZE 1.0f
#define ZOOM_MAX_SIZE 20.0f
#define ZOOM_FACTOR 1.2f

#define VIEW_MOVE_STEP 0.5f
float g_viewOffsetX = 0.0f;
float g_viewOffsetY = 0.0f;

Window &g_window = Window::getInstance();

int g_n = 30;

// Pontos de controle da Spline
GLfloat g_ctrlpoints[4][3] = {
    {-4.0, -4.0, 0.0}, {-2.0, 4.0, 0.0}, {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

int g_selectedPoint = -1;
bool g_mouseDown = false;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_MAP1_VERTEX_3);

    // Definicao do polinomio com os pontos de controle
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlpoints[0][0]);

    // Muda para a matriz de projecao (aulas futuras)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area/volume de visualizacao. Os objetos desenhados devem estar
    // dentro desta area
    glOrtho(-g_window.size(), g_window.size(), -g_window.size(),
            g_window.size(), -g_window.size(), g_window.size());
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Set up screen space coordinates
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, g_window.width(), 0, g_window.height());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw text at fixed screen position
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(10, 10); // Fixed pixel position

    const static char label[] = "n = ";
    for (size_t i = 0; i < STR_LEN(label); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[i]);

    char buffer[4] = {0};
    sprintf(buffer, "%d", g_n);
    for (size_t i = 0; buffer[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);

    const static char help[] = "; use i/k to change n, +/- to zoom in/out, "
                               "arrows to move perspective, r to reset";
    for (size_t i = 0; i < STR_LEN(help); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, help[i]);

    // Restore original matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    /* Desenha a curva aproximada por n+1 pontos. */
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= g_n; i++) {
        // Avaliacao do polinomio, retorna um vertice (equivalente a um
        // glVertex3fv)
        glEvalCoord1f((GLfloat)i / (GLfloat)g_n);
    }
    glEnd();

    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
        glVertex3fv(&g_ctrlpoints[i][0]);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    g_window.width(w);
    g_window.height(h);

    // Define a porcao visivel da janela
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    // Muda para a matriz de projecao (aulas futuras)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-g_window.effectiveSize() + g_viewOffsetX,
            g_window.effectiveSize() + g_viewOffsetX,
            -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            -g_window.size(), g_window.size());

    for (int i = 0; i < 4; i++) {
        if (g_ctrlpoints[i][0] > g_window.effectiveSize() + g_viewOffsetX)
            g_ctrlpoints[i][0] = g_window.effectiveSize() + g_viewOffsetX;
        else if (g_ctrlpoints[i][0] < -g_window.effectiveSize() + g_viewOffsetX)
            g_ctrlpoints[i][0] = -g_window.effectiveSize() + g_viewOffsetX;

        if (g_ctrlpoints[i][1] >
            g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY)
            g_ctrlpoints[i][1] =
                g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY;
        else if (g_ctrlpoints[i][1] <
                 -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY)
            g_ctrlpoints[i][1] =
                -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY;
    }
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlpoints[0][0]);

    // Muda para a matriz de trasformacoes (aulas futuras)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, UNUSED_PARAM(int x), UNUSED_PARAM(int y)) {
    switch (key) {
    case '=':
    case '+':
        g_window.size(fmax(ZOOM_MIN_SIZE, g_window.size() / ZOOM_FACTOR));
        break;

    case '-':
    case '_':
    case '\\':
        g_window.size(fmin(ZOOM_MAX_SIZE, g_window.size() * ZOOM_FACTOR));
        break;

    case 'r':
    case 'R':
        g_viewOffsetX = 0.0f;
        g_viewOffsetY = 0.0f;
        g_window.size(5.0f);

        break;

    case 'i':
    case 'I':
        if (g_n == 100)
            break;

        g_n++;
        glutPostRedisplay();

        return;

    case 'k':
    case 'K':
        if (g_n == 2)
            break;

        g_n--;
        glutPostRedisplay();

        return;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-g_window.effectiveSize() + g_viewOffsetX,
            g_window.effectiveSize() + g_viewOffsetX,
            -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            -g_window.size(), g_window.size());

    glutPostRedisplay();
}

void specialKeys(int key, UNUSED_PARAM(int x), UNUSED_PARAM(int y)) {
    switch (key) {
    case GLUT_KEY_LEFT:
        g_viewOffsetX -= VIEW_MOVE_STEP;
        break;
    case GLUT_KEY_RIGHT:
        g_viewOffsetX += VIEW_MOVE_STEP;
        break;
    case GLUT_KEY_UP:
        g_viewOffsetY += VIEW_MOVE_STEP;
        break;
    case GLUT_KEY_DOWN:
        g_viewOffsetY -= VIEW_MOVE_STEP;
        break;
    default:
        return;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-g_window.effectiveSize() + g_viewOffsetX,
            g_window.effectiveSize() + g_viewOffsetX,
            -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
            -g_window.size(), g_window.size());

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        g_mouseDown = true;

        float wx = ((float)x / g_window.width() * 2.0 - 1.0) *
                       g_window.effectiveSize() +
                   g_viewOffsetX;
        float wy = (1.0 - (float)y / g_window.height() * 2.0) *
                       (g_window.effectiveSize() / g_window.aspect()) +
                   g_viewOffsetY;

        // Check for point selection using transformed coordinates
        for (int i = 0; i < 4; i++) {
            float dx = wx - g_ctrlpoints[i][0];
            float dy = wy - g_ctrlpoints[i][1];
            if (sqrt(dx * dx + dy * dy) < 0.3) {
                g_selectedPoint = i;
                break;
            }
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        g_mouseDown = false;
        g_selectedPoint = -1;
    }
}

void motion(int x, int y) {
    if (!g_mouseDown || g_selectedPoint == -1)
        return;

    float wx =
        ((float)x / g_window.width() * 2.0 - 1.0) * g_window.effectiveSize() +
        g_viewOffsetX;
    float wy = (1.0 - (float)y / g_window.height() * 2.0) *
                   (g_window.effectiveSize() / g_window.aspect()) +
               g_viewOffsetY;

    wx = fmax(-g_window.effectiveSize() + g_viewOffsetX,
              fmin(g_window.effectiveSize() + g_viewOffsetX, wx));
    wy = fmax(
        -g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY,
        fmin(g_window.effectiveSize() / g_window.aspect() + g_viewOffsetY, wy));

    g_ctrlpoints[g_selectedPoint][0] = wx;
    g_ctrlpoints[g_selectedPoint][1] = wy;

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &g_ctrlpoints[0][0]);
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    g_window.width(TAMANHO_JANELA);
    g_window.height(TAMANHO_JANELA);
    g_window.size(5.0f);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
