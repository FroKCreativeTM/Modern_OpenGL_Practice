#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* opengl ���ؽ�Ʈ ���� ���� */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // The compatibility OpenGL profile makes VAO object 0 a default object. 
    // The core OpenGL profile makes VAO object 0 not an object at all. 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // ������ ���� Ÿ���� �����Ѵ�. 
    glfwSwapInterval(1);

    // init the glew library after making context
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[] =
        {
            -0.5f, -0.5f,   // 0
            0.5f, -0.5f,    // 1
            0.5f, 0.5f,     // 2
            -0.5f, 0.5f,    // 3
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // ���ؽ� �迭 ����
        VertexArray va;
        // ���ؽ� ���� ����
        VertexBuffer vb(positions, 4 * 2 * sizeof(positions));

        // ���̾ƿ� ����
        // �� �� �� ���̾ƿ��� 2D�̴�.
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // �ε��� ���� ����
        IndexBuffer ib(indices, 6);


        Shader shader("./shaderFile.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);

        va.Unbind();
        shader.Unbind();           
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    // delete here

    glfwTerminate();
    return 0;
}