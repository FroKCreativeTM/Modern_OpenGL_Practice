#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



/* 셰이더 종류마다 저장할 각 셰이더 소스 클래스입니다. */
struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

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
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int shaderType, const std::string& source)
{
    // 셰이더를 타입에 따라서 만듭니다.
    unsigned int id = glCreateShader(shaderType);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    // 사각형이 지나가는 부분
    float positions[] =
    {
        -0.5f, -0.5f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,  // 1
        0.5f, 0.5f, 0.0f,   // 2
        -0.5f, 0.5f, 0.0f   // 3
    };

    // GPU에 보낼 저점의 아이디를 
    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    /* 코어 버전을 사용하기 위해서는 이 vertexArrayObject(VAO)가 필요하다. */
    unsigned int vertexArrayObject;
    GLCall(glGenVertexArrays(1, &vertexArrayObject));
    GLCall(glBindVertexArray(vertexArrayObject));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    // 인덱스가 0이면, 지금 바인딩이 된 버퍼의 내용을 vertexArrayObject에 링킹한다.
    // 하지만 특정의 로케이션이나 레이아웃에 링킹하고 싶을 경우도 있을 것이다.
    // 이런 경우 정점 배열 버퍼를 무시하는 경우도 있다.
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0)); 

    unsigned int indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("./Uniform.shader");
    std::cout << source.vertexSource << std::endl;
    std::cout << source.fragmentSource << std::endl;

    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    GLCall(glUseProgram(shader));

    int location = glGetUniformLocation(shader, "u_Color");    
    ASSERT(location != -1);                                    
    GLCall(glUniform4f(location, 0.0f, 1.0f, 1.0f, 1.0f));     

    /* unbound everything */
    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* 실시간으로 바인딩해서 사용하는 것 또한 가능하다. */
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.0f, 0.5f, 1.0f));

        // vertex array를 사용하기 때문에 필요없는 부분이다.
        // GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        // GLCall(glEnableVertexAttribArray(0));
        // GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));

        // 이 정점 배열 오브젝트에 링킹이 된다. (위의 코드 참조)
        GLCall(glBindVertexArray(vertexArrayObject));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));

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

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}