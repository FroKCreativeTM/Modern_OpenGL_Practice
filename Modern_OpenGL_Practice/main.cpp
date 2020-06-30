#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

    // init the glew library after making context
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[9] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // 하나의 버퍼를 만든다.
    unsigned int buffer;
    glGenBuffers(1, &buffer);       // 갯수와 객체를 저장할 포인터
    // 위에서 만든 만든 버퍼에 아이디를 준다. (바인딩 시킨다.)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // 버퍼에 보관할 데이터를 가져온다.
    // STREAM은 보통 잘 안 쓰고
    // STATIC이나 DINAMIC이 자주 쓰인다.
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positions, GL_STATIC_DRAW);

    // 0번째 버퍼를 활성화 한다.
    glEnableVertexAttribArray(0);                                               

    /*
    index : 정점 쉐이더의 layout(location=x)에서 x에 해당
    size : vertex attrib를 구성하는 멤버의 갯수, 예로 들어 vector position 속성이 x,y,z,w 즉 homogeneous 좌표계라면 4, x,y,z 로 구성되면 3
    즉 1,2,3,4 중 하나만 가능하다.
    type : 정점 attrib를 구성하는 멤버 타입
    normalized : 타입이 float나 GL_HALF_FLOAT가 아닌 경우, unsigned인 경우 0..1 signed인 경우 -1..1의 float로 변경한다.
    stribe pointer가 가리키는 배열에서 다음 구성 요소까지의 바이트 수
    pointer : vertex attrib의 배열 주소
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);      

    // 이 데이터들을 어떻게 그릴 것인가에 대해 쓰는 것이 바로 shader이다.

    // 보통 데이터를 이렇게 직빵으로 주지 않는다.
    // glVertex3f(-0.5f, -0.5f, 0.0f);
    // glVertex3f(0.0f, 0.5f, 0.0f);
    // glVertex3f(0.5f, -0.5f, 0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // 버퍼에 있는 데이터를 가져온다.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}