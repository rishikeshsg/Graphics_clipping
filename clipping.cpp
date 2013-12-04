/*-------------------------------------------------------
-	Rishikesh Ghewari & Arpit Agarwal					-
-	Roll No. 10010121 & 10010113						-
-	Graphics Lab										-
-	Cohen Sutherland clipping							-
-------------------------------------------------------*/

#include "clipping.h"
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

// Global Variables
double rotate_x=0;
double rotate_y=0;
double zoom = 0.6;
float bottom = 10.0;

/* Initialize OpenGL Graphics */
void initGL() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);				// Set background color to white and opaque
	glClearDepth(1.0f);									// Set background depth to farthest
	glEnable(GL_DEPTH_TEST);							// Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);								// Set the type of depth-test
	glShadeModel(GL_SMOOTH);							// Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections 
}

//Called when the window is resized.
void reshape(GLsizei width, GLsizei height) {
	//Compute aspect ratio of the new window
	//Avoid division by zero
	if (height == 0) 
		height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

     windowToView(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();             
    perspective(45.0, aspect, 0.1f, 100.0f);
}

//Callback Function called when some keys are pressed.
void specialKeys( int key, int x, int y ) {
	if (key == GLUT_KEY_RIGHT)
		rotate_y += 4;
	else if (key == GLUT_KEY_LEFT)
		rotate_y -= 4;
	else if (key == GLUT_KEY_UP)
		rotate_x -= 4;
	else if (key == GLUT_KEY_DOWN)
		rotate_x += 4;
	else if (key == GLUT_KEY_INSERT)
		zoom *= (1.0 + 0.01);
	else if (key == GLUT_KEY_END)
		zoom *= (1.0 - 0.01);

	//Update display
	glutPostRedisplay();
}

void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
		case 'w':
			bottom += 3.0;
			break;
		case 's':
			bottom -= 3.0;
			break;
    }
    glutPostRedisplay( );
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers required for Hidden surface removal
	glMatrixMode(GL_MODELVIEW);    

	glLoadIdentity();

	 worldToEye(0, 0, -20, 0, 0, 0, 0, 1, 0);

	rotate(rotate_x, 1.0, 0.0, 0.0);
	rotate(rotate_y, 0.0, 1.0, 0.0);
	scale(zoom, zoom, zoom);
	translate( 3.0f, -3.0f, -30.0f);

	//Draw the external cube(the view volume)
	window win;
	win.assign(0,0,10,10);
	int glBeginType = GL_LINE_LOOP;
	drawCube(5,5,5,10,glBeginType,0.0,0.0,0.0);

	//Cubes of size one for each color cuboid.
	for (int i=0;i<=6;i+=3)
	{
		float red = 0.0, blue = 0.0, green = 0.0;
		if(i<3)
			green = 1.0;
		else if(i>=3 && i<6)
			blue = 1.0;
		else
			red = 1.0;
		clipping(5,bottom+i+1,5,5,bottom+i,5,win,red,green,blue,1);
		clipping(5,bottom+i+2,5,5,bottom+i+1,5,win,red,green,blue,1);
		clipping(5,bottom+i+3,5,5,bottom+i+2,5,win,red,green,blue,1);
	}

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //Enable double buffered mode
	glutInitWindowSize(640, 480);							//Set the window's initial width & height
	glutInitWindowPosition(50, 50);							//Position the window's initial top-left corner
	glutCreateWindow("Cohen Sutherland Clipping");          //Create window with the given title
	glutDisplayFunc(display);       
	glutReshapeFunc(reshape);      
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboardFunc);
	initGL();												//openGL initialization function
	glutMainLoop();                 
	return 0;
}
