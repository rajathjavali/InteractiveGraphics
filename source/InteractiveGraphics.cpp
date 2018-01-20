// InteractiveGraphics.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stdio.h>

#include "GL\freeglut.h"
#include "glutFunctions.h"

#define GL_SCREEN_WIDTH 1000
#define GL_SCREEN_HEIGHT 500

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	int height = glutGet(GLUT_SCREEN_HEIGHT);
	int width = glutGet(GLUT_SCREEN_WIDTH);
	if (!height || !width)
		glutInitWindowPosition(100, 100);
	else
		glutInitWindowPosition(width / 2 - GL_SCREEN_WIDTH /2, height / 2 - GL_SCREEN_HEIGHT /2);
	glutInitWindowSize(GL_SCREEN_WIDTH, GL_SCREEN_HEIGHT);
	glutCreateWindow("Rasterisation");
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
    return 0;
}

