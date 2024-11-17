#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#define TAMANHO_JANELA 500

const float g_squareOriginX = 0.25, g_squareOriginY = 0.25;
const float g_squareWidth = 0.5, g_squareHeight = 0.5;

float g_dX = 0.0, g_dY = 0.0;
bool g_isKeyPressed[256] = {0};
bool g_isSquareDrag = false;
float g_lastMouseX = 0.0, g_lastMouseY = 0.0;
float g_mouseX = 0.0, g_mouseY = 0.0;

void display(void) {
    /* Limpar todos os pixels  */
    glClear(GL_COLOR_BUFFER_BIT);

    const float squareOutlineWidth = 0.05;
    glColor3f(1.0, .5, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(g_squareOriginX + g_dX - squareOutlineWidth,
               g_squareOriginY + g_dY - squareOutlineWidth, 0.0); // D   C
    glVertex3f(g_squareOriginX + g_squareWidth + g_dX + squareOutlineWidth,
               g_squareOriginY + g_dY - squareOutlineWidth, 0.0); //
    glVertex3f(g_squareOriginX + g_squareWidth + g_dX + squareOutlineWidth,
               g_squareOriginY + g_squareHeight + g_dY + squareOutlineWidth,
               0.0); //
    glVertex3f(g_squareOriginX + g_dX - squareOutlineWidth,
               g_squareOriginY + g_squareHeight + g_dY + squareOutlineWidth,
               0.0); // A   B
    glEnd();

    /* Define cor dos vértices com os valores R, G e B variando de 0.0 a 1.0 */
    glColor3f(.33, .5, .25);
    /* Desenhar um polígono (retângulo) */
    glBegin(GL_POLYGON);
    glVertex3f(g_squareOriginX + g_dX, g_squareOriginY + g_dY, 0.0); // D   C
    glVertex3f(g_squareOriginX + g_squareWidth + g_dX, g_squareOriginY + g_dY,
               0.0); //
    glVertex3f(g_squareOriginX + g_squareWidth + g_dX,
               g_squareOriginY + g_squareHeight + g_dY, 0.0); //
    glVertex3f(g_squareOriginX + g_dX, g_squareOriginY + g_squareHeight + g_dY,
               0.0); // A   B
    glEnd();

    /* Desenhar no frame buffer! */
    glutSwapBuffers(); // Funcao apropriada para janela double buffer
}

void keyboard(unsigned char key, int x, int y) {
    g_isKeyPressed[key] = true;

    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    g_isKeyPressed[key] = false;

    glutPostRedisplay();
}

void idle() {
    const float step = 0.01;
    if (g_isKeyPressed['w']) {
        g_dY += step;
    }
    if (g_isKeyPressed['s']) {
        g_dY -= step;
    }
    if (g_isKeyPressed['a']) {
        g_dX -= step;
    }
    if (g_isKeyPressed['d']) {
        g_dX += step;
    }
    if (g_isKeyPressed['r']) {
        g_dX = 0.0;
        g_dY = 0.0;
    }

    if (g_isSquareDrag) {
        g_dX += g_mouseX - g_lastMouseX;
        g_dY += g_mouseY - g_lastMouseY;

        g_lastMouseX = g_mouseX;
        g_lastMouseY = g_mouseY;

        printf("g_dX: %f, g_dY: %f\n", g_dX, g_dY);
    }

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    float xNorm = (float)x / TAMANHO_JANELA;
    float yNorm = 1.0 - (float)y / TAMANHO_JANELA;

    if (button == GLUT_LEFT_BUTTON
        && state == GLUT_DOWN
        && xNorm >= g_squareOriginX + g_dX
        && xNorm <= g_squareOriginX + g_squareWidth + g_dX
        && yNorm >= g_squareOriginY + g_dY
        && yNorm <= g_squareOriginY + g_squareHeight + g_dY)
        g_isSquareDrag = true;
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        g_isSquareDrag = false;
}

void motion(int x, int y) {
    float xNorm = (float)x / TAMANHO_JANELA;
    float yNorm = 1.0 - (float)y / TAMANHO_JANELA;

    g_lastMouseX = g_mouseX;
    g_lastMouseY = g_mouseY;

    g_mouseX = xNorm;
    g_mouseY = yNorm;
}

void init(void) {
    /* selecionar cor de fundo (preto) */
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* inicializar sistema de visualizacao */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(TAMANHO_JANELA, TAMANHO_JANELA);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hello world");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);

    glutMainLoop();

    return 0;
}
