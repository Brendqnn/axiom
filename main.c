#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;
    
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "ELMO IS EVIL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
   
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

		static float angle = 0.0f;
		angle += 0.01f;

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		
		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5, -0.5f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5f, -0.5);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.0f, 0.5f);
		glEnd();

		glPopMatrix();
		
        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

