#include "glutFunctions.h"
#include "GL\freeglut.h"

// function which takes care of rendering the scene to the back buffer
// avoid having computation details. computation goes into the idle func
// always call all opengl commands on the same thread
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	/* All rendering code */
	glColor3f(0.4f, 0.3f, 0.5f);

	/* ------------------ */
	glutSwapBuffers();
}

void idle()
{
	glColor3f(0.8f, 0.6f, 0.8f);

	// tells glut that the scene has changed
	// glut then decides whether to re render or not
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	//  exit the window on esc key
	case 27:
		exit(0);
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_DOWN)
	{
		switch (button) {

		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;

		}
	}
	else
	{

	}
}