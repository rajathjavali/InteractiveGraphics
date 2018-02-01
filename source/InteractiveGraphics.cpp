// InteractiveGraphics.cpp : Defines the entry point for the console application.
// refer : http://ogldev.atspace.co.uk/

#include <stdlib.h>
#include <stdio.h>

#include "GL\glew.h"
#include "cyPoint.h"
#include "cyTriMesh.h"
#include "GL\freeglut.h"

#define GL_SCREEN_WIDTH 1000
#define GL_SCREEN_HEIGHT 500

GLuint VAO;
GLuint VBO[2];

void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

int main(int argc, char **argv)
{
	// needs an object as input
	if (argc != 2)
	{
		fprintf(stderr, "Pass an Obj name as command Line Parameter\n");
		return -1;
	}
	
	/* Variables */
	char* obj = argv[1];
	int height = 0, width = 0;
	cyTriMesh *mesh = new cyTriMesh();
	mesh->LoadFromFileObj(obj);

	int numVert = mesh->NV();

	std::vector<float> vertices, normals;
	for (int i = 0; i < numVert; i++)
	{
		cy::Point3f vertex = mesh->V(i);
		cy::Point3f normal = mesh->VN(i);

		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);

		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	
	height = glutGet(GLUT_SCREEN_HEIGHT);
	width = glutGet(GLUT_SCREEN_WIDTH);
	
	if (!height || !width)
		glutInitWindowPosition(100, 100);
	else
		glutInitWindowPosition(width / 2 - GL_SCREEN_WIDTH /2, height / 2 - GL_SCREEN_HEIGHT /2);
	glutInitWindowSize(GL_SCREEN_WIDTH, GL_SCREEN_HEIGHT);
	
	glutCreateWindow("Rasterisation");
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
		
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals[0], GL_STATIC_DRAW);
	
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
    return 0;
}

// function which takes care of rendering the scene to the back buffer
// avoid having computation details. computation goes into the idle func
// always call all opengl commands on the same thread
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
	
	glutSwapBuffers();
}

void idle()
{
	// changing the color of the output window
	float r = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
	float g = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
	float b = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
	float a = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
	//glClearColor(r, g, b, a);

	// tells glut that the scene has changed
	// glut then decides whether to re render or not
	//glutPostRedisplay();
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