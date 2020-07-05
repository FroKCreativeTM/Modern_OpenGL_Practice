/* OpenGL Graphics header */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* C/C++ runtime header */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/* addtional(class) header */
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

/* Graphics math header */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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
            100.5f, 100.5f, 0.0f, 0.0f,   // 0
            200.5f, 100.5f, 1.0f, 0.0f,    // 1
            200.5f, 200.5f, 1.0f, 1.0f,     // 2
            100.5f, 200.5f, 0.0f, 1.0f     // 3
        };

        unsigned int indices[] =
        {
            0, 1, 2,
            2, 3, 0
        };

        // ���� ���� ���� ���� ��� Ȱ��ȭ
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // ���ؽ� �迭 ����
        VertexArray va;
        // ���ؽ� ���� ����
        VertexBuffer vb(positions, 4 * 4 * sizeof(positions));  // ����(2) + �ؽ�ó(2)

        // ���̾ƿ� ����
        // �� �� �� ���̾ƿ��� 2D�̴�.
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);      // �ؽ�ó�� ���� ���̾ƿ�
        va.AddBuffer(vb, layout);

        // �ε��� ���� ����
        IndexBuffer ib(indices, 6);

        // ���� ortho�� �̿��ؼ� 2D ���� ī�޶� ǥ���ϸ�
        // perspective�� �̿��ؼ� 3D ������ ǥ���Ѵ�.
        // glm::mat4 projection = glm::ortho<float>(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 projection    = glm::ortho<float>(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);
        glm::mat4 view          = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));       
        glm::mat4 model         = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        // glm::vec4 vertexPosition(100.0f, 100.f, 0.0f, 1.0f);
        // glm::vec4 result = projection * vertexPosition;

        glm::mat4 mvp = projection * view * model;

        Shader shader("./Shader/shaderFile.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.0f, 1.0f, 1.0f, 1.0f);
        // shader.SetUniformMat4f("u_MVP", projection);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("Media/Texture/nill.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0); // �ؽ�ó ������ 0�̴ϱ� 0�� �ִ´�.

        va.Unbind();
        shader.Unbind();           
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);
            
            /* ������ ��ü�� �׸��� */
            renderer.Draw(va, ib, shader);

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