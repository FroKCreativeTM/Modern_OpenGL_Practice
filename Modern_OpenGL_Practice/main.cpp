#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// ����� �� �� ������ �극��ŷ ����Ʈ�� �ɾ��ش�.
#define ASSERT(x) if (!(x)) __debugbreak();
// ������ ��ũ��
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* ���̴� �������� ������ �� ���̴� �ҽ� Ŭ�����Դϴ�. */
struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    // C++17 �����Դϴ�.
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function
            << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    // ���� �ҷ�����
    std::ifstream stream(filepath);

    // ���̴� Ÿ���� �����մϴ�.
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // ���� ���̴����� �����մϴ�.
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // �����׸�Ʈ ���̴����� �����մϴ�.
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            // ���ڿ� ��Ʈ���� �����Ѵ�.
            ss[(int)type] << line << '\n';
        }
    }
    
    // ���̴� ��ü�� ��ȯ�մϴ�.
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int shaderType, const std::string& source)
{
    // ���̴��� Ÿ�Կ� ���� ����ϴ�.
    unsigned int id = glCreateShader(shaderType);
    const char* src = source.c_str();               // &source[0]

    // ���̴� �ҽ��� id�� �����ϰ� �̸� �������մϴ�.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // ���� ó��
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // ���� �������� �����Ѵٸ� ���� �α׸� ��� �� 0�� ��ȯ�մϴ�.
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile "<< (shaderType == GL_VERTEX_SHADER  ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    // ���̴� ���̵��� ��ȯ�մϴ�.
    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // ���α׷��� �� ���̴��� ������ ��, ��ũ���ְ�, ����� ���ش�.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // ���� �� �� ���̴��� �����.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // ���α׷��� ��ȯ�Ѵ�.
    return program;
}

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

    // �� �̷��� ���� �뷮�� �����Ѵ�. (�̹� ������ �κ��� ��Ȱ������.)
    // float positions[] = {
    //     // 1st �ﰢ��
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f,
    // 
    //     // �ι�° �ﰢ��
    //     0.5f, 0.5f, 0.0f,
    //     -0.5f, 0.5f, 0.0f,
    //     -0.5f, -0.5f, 0.0f
    // };

    // �簢���� �������� �κ�
    float positions[] = 
    {
        -0.5f, -0.5f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,  // 1
        0.5f, 0.5f, 0.0f,   // 2
        -0.5f, 0.5f, 0.0f   // 3
    };

    // GPU�� ���� ������ ���̵� 
    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    // �ϳ��� ���۸� �����.
    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));       // ������ ��ü�� ������ ������
    // ������ ���� ���� ���ۿ� ���̵� �ش�. (���ε� ��Ų��.)
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    // ���ۿ� ������ �����͸� �����´�.
    // STREAM�� ���� �� �� ����
    // STATIC�̳� DINAMIC�� ���� ���δ�.
    // �� ���� �ﰢ���� ����� ���� ������ �����Ѵ�.
    GLCall(glBufferData(GL_ARRAY_BUFFER, 9 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    // 0��° ���۸� Ȱ��ȭ �Ѵ�.
    GLCall(glEnableVertexAttribArray(0));

    /*
    index : ���� ���̴��� layout(location=x)���� x�� �ش�
    (���̾ƿ��̶�? ���̴����� ����ϴ� �ε������� ���̴�. ��Ȯ���� attribute position)
    size : vertex attrib�� �����ϴ� ����� ����, ���� ��� vector position �Ӽ��� x,y,z,w �� homogeneous ��ǥ���� 4, x,y,z �� �����Ǹ� 3
    �� 1,2,3,4 �� �ϳ��� �����ϴ�.
    type : ���� attrib�� �����ϴ� ��� Ÿ��
    normalized : Ÿ���� float�� GL_HALF_FLOAT�� �ƴ� ���, unsigned�� ��� 0..1 signed�� ��� -1..1�� float�� �����Ѵ�.
    stribe pointer�� ����Ű�� �迭���� ���� ���� ��ұ����� ����Ʈ ��
    pointer : vertex attrib�� �迭 �ּ� const void* Ÿ������ ���� �����ϴ�.
    */
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));

    // �ε��� ���۸� �����.
    unsigned int indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    // ������ ���� ���� ���ۿ� ���̵� �ش�. (���ε� ��Ų��.)
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    // �� ���� �ﰢ���� ����� ���� ������ �����Ѵ�.
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    // ���̴� ������ �ҷ��ɴϴ�.
    ShaderProgramSource source = ParseShader("./Basic.shader");
    std::cout << source.vertexSource << std::endl;
    std::cout << source.fragmentSource << std::endl;

    // ���̴��� �����ϰ� ��� �����մϴ�.
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    GLCall(glUseProgram(shader));

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
        // 6���� ���� �����͸� �����´�.
        // glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

        // �ε��� ���۸� �̿��� �簢�� �׸��� 
        // �� �� GL_UNSIGNED_INT�� ������ ������ ���� �ʴ´�. �ֳĸ� unsigned int�� ���� ���۷� �Ǿ��ֱ� �����̴�.
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}