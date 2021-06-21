#include<iostream>
#include<glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        /* Loop until the user closes the window */
        /* Render here */
        glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glBegin(GL_TRIANGLES);
            
            glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.0f);     // bottom left 0
         //   glColor4f(0.0f, 1.0f, 0.0f, 1.0f);  
            glVertex3f(0.5f, -0.5f, 0.0f);      // bottom right 1
          //  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, 0.0f);       // top right 2
           
        glEnd();

        glBegin(GL_TRIANGLES);
            glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.0f);     // bottom left 0
            glVertex3f(0.5f, -0.5f, 0.0f);       // bottom right 1
            glVertex3f(-0.5f, 0.5f, 0.0f);      // top left 3
        glEnd();
        /*
        glBegin(GL_TRIANGLES);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f(0.5f, 0.5f, 0.0f);
        glVertex3f(-0.5f, 0.5f, 0.0f);
        glEnd();*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}