#include <OpenGL/gl.h>
#include <GLUT/glut.h>

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("main");
	glutHideWindow();

	glutDisplayFunc
	(
		[] -> void
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.f, 0.f, 0.f, 1.f);
		}
	);

	glutShowWindow();
	
	glutMainLoop();

	return 0;
}
