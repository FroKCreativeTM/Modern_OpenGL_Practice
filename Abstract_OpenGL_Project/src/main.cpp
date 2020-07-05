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

/* ImGui header */
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* opengl 컨텍스트 버전 설정 */
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

    // 버퍼의 스왑 타임을 조절한다. 
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

        // 알파 값에 따른 블렌딩 기능 활성화
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // 버텍스 배열 생성
        VertexArray va;
        // 버텍스 버퍼 생성
        VertexBuffer vb(positions, 4 * 4 * sizeof(positions));  // 정점(2) + 텍스처(2)

        // 레이아웃 설정
        // 이 때 이 레이아웃은 2D이다.
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);      // 텍스처를 위한 레이아웃
        va.AddBuffer(vb, layout);

        // 인덱스 버퍼 생성
        IndexBuffer ib(indices, 6);

        // 보통 ortho를 이용해서 2D 공간 카메라를 표현하며
        // perspective를 이용해서 3D 공간을 표현한다.
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
        shader.SetUniform1i("u_Texture", 0); // 텍스처 슬롯이 0이니까 0을 넣는다.

        va.Unbind();
        shader.Unbind();           
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        Renderer renderer;

        /* imgui create and initialize */
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();

        /* imgui variable */
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);
            
            /* 렌더러 객체의 그리기 */
            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    
    // delete here
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}