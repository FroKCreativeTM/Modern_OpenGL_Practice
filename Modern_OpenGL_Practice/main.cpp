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

    // �ϳ��� ���۸� �����.
    unsigned int buffer;
    glGenBuffers(1, &buffer);       // ������ ��ü�� ������ ������
    // ������ ���� ���� ���ۿ� ���̵� �ش�. (���ε� ��Ų��.)
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // ���ۿ� ������ �����͸� �����´�.
    // STREAM�� ���� �� �� ����
    // STATIC�̳� DINAMIC�� ���� ���δ�.
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positions, GL_STATIC_DRAW);

    // 0��° ���۸� Ȱ��ȭ �Ѵ�.
    glEnableVertexAttribArray(0);                                               

    /*
    index : ���� ���̴��� layout(location=x)���� x�� �ش�
    size : vertex attrib�� �����ϴ� ����� ����, ���� ��� vector position �Ӽ��� x,y,z,w �� homogeneous ��ǥ���� 4, x,y,z �� �����Ǹ� 3
    �� 1,2,3,4 �� �ϳ��� �����ϴ�.
    type : ���� attrib�� �����ϴ� ��� Ÿ��
    normalized : Ÿ���� float�� GL_HALF_FLOAT�� �ƴ� ���, unsigned�� ��� 0..1 signed�� ��� -1..1�� float�� �����Ѵ�.
    stribe pointer�� ����Ű�� �迭���� ���� ���� ��ұ����� ����Ʈ ��
    pointer : vertex attrib�� �迭 �ּ�
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);      

    // �� �����͵��� ��� �׸� ���ΰ��� ���� ���� ���� �ٷ� shader�̴�.

    // ���� �����͸� �̷��� �������� ���� �ʴ´�.
    // glVertex3f(-0.5f, -0.5f, 0.0f);
    // glVertex3f(0.0f, 0.5f, 0.0f);
    // glVertex3f(0.5f, -0.5f, 0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // ���ۿ� �ִ� �����͸� �����´�.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}