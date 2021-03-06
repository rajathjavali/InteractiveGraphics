// InteractiveGraphics.cpp : Defines the entry point for the console application.
// refer : http://ogldev.atspace.co.uk/

#include <stdlib.h>
#include <stdio.h>

#include "GL\glew.h"
#include "GL\gl.h"
#include "cyPoint.h"
#include "cyTriMesh.h"
#include "GL\freeglut.h"
#include "cyGL.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\matrix_transform.hpp"

#define GL_SCREEN_WIDTH 1000
#define GL_SCREEN_HEIGHT 500
#define PI 3.1415

GLuint VAO;
GLuint VBO[2];

enum VIEW {
	ORTHO,
	PERSP
};

void shaderinit(cyGLSLProgram *program, const char *vertexShader, const char *fragmentShader);
void display();
void idle();
void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void modelProjectionInit(VIEW view);

int totalSize;
int ProjectionModelviewMatrix_Loc;
cyGLSLProgram *program;



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
	program = new cyGLSLProgram();
	std::vector<float> vertices, normals;
	
	std::vector<float> cube = { -1.0f,-1.0f,-1.0f, -1.0f,-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f,-1.0f, 1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, 1.0f,-1.0f, 1.0f, -1.0f,-1.0f, 1.0f, -1.0f,-1.0f,-1.0f, -1.0f, 1.0f, 1.0f, -1.0f,-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f,-1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, 1.0f};
	std::vector<float> cubecolor;
	float r;
	for (int v = 0; v < 36; v++) {
		r = static_cast <float>(rand()) / static_cast <float> (RAND_MAX);
		cubecolor.push_back(r);
	}

	mesh->LoadFromFileObj(obj);

	int numVert = mesh->NV();

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
	totalSize = vertices.size() / 3;

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
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, cubecolor.size() * sizeof(float), &cubecolor[0], GL_STATIC_DRAW);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint testVBO;
	glGenBuffers(1, &testVBO);
	glBindBuffer(GL_ARRAY_BUFFER, testVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(oneTriangle), &oneTriangle[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(float), &cube[0], GL_STATIC_DRAW);
	totalSize = cube.size() / 3;

	

	/*glGenBuffers(2, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);*/
	
	shaderinit(program, "VertexShader", "FragmentShader");
	
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialKey);

	glutMainLoop();
    return 0;
}

void shaderinit(cyGLSLProgram *program, const char *vertexShader, const char *fragmentShader)
{
	if (program->BuildFiles(vertexShader, fragmentShader))
		printf("Successfull linking!!");
	glUseProgram(program->GetID());
	modelProjectionInit(PERSP);
}

void modelProjectionInit(VIEW view)
{

	glm::mat4 Projection;
	switch (view) {
	case ORTHO:
		Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
		break;
	case PERSP:
		Projection = glm::perspective(glm::radians(45.0f), (float)GL_SCREEN_WIDTH / (float)GL_SCREEN_HEIGHT, 0.1f, 100.0f);
		break;
	}

	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 mvp = Projection * View * Model;

	GLuint mvp_handle = glGetUniformLocation(program->GetID(), "MVP");
	glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, &mvp[0][0]);

}

// function which takes care of rendering the scene to the back buffer
// avoid having computation details. computation goes into the idle func
// always call all opengl commands on the same thread
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//glDrawArrays(GL_POINTS, 0, totalSize);
	glDrawArrays(GL_TRIANGLES, 0, totalSize);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
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

bool perspective = true;
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		//  exit the window on esc key
	case 27:
		exit(0);
		break;
	case 'P':
	case 'p':
		if (perspective)
		{
			modelProjectionInit(ORTHO);
			perspective = false;
			glutPostRedisplay();
		}
		else
		{
			modelProjectionInit(PERSP);
			perspective = true;
			glutPostRedisplay();
		}
		break;
	}
}

void specialKey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F6:
		shaderinit(program, "VertexShader", "FragmentShader");
		glUseProgram(program->GetID());
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



/*	void init()
	{
		//glm::vec3 cameraPosition, cameraTarget(0, 0, -1), upVector(0, 1, 0);

		//glm::mat4 CameraMatrix = glm::lookAt(
		//	cameraPosition, // the position of your camera, in world space
		//	cameraTarget,   // where you want to look at, in world space
		//	upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
		//);
		//glm::float32 FoV = PI/2;

		//// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
		//glm::mat4 projectionMatrix = glm::perspective(
		//	glm::radians(FoV), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		//	4.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		//	0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		//	100.0f             // Far clipping plane. Keep as little as possible.
		//);

		//glm::mat4 myMatrix;
		//glm::vec4 myVector;
		//glm::vec4 transformedVector = myMatrix * myVector;
		//glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
		//glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
		//glm::vec4 transformedVector = myMatrix * myVector; // guess the result
		//
		//glm::mat4 MVPmatrix = projection * view * model; // Remember : inverted !


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)GL_SCREEN_WIDTH / (float)GL_SCREEN_HEIGHT, 0.1f, 100.0f);

		// Or, for an ortho camera :
		//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

												   // Get a handle for our "MVP" uniform
												   // Only during the initialisation
												   //GLuint MatrixID = glGetUniformLocation(program_id, "MVP");

												   // Send our transformation to the currently bound shader, in the "MVP" uniform
												   // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
												   //glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, &mvp[0][0]);
	}*/