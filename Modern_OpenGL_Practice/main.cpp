#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    // 파일 불러오기
    std::ifstream stream(filepath);

    // 셰이더 타입을 저장합니다.
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
                // 정점 셰이더임을 저장합니다.
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // 프레그먼트 셰이더임을 저장합니다.
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            // 문자열 스트림에 저장한다.
            ss[(int)type] << line << '\n';
        }
    }
    
    // 셰이더 객체를 반환합니다.
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int shaderType, const std::string& source)
{
    // 셰이더를 타입에 따라서 만듭니다.
    unsigned int id = glCreateShader(shaderType);
    const char* src = source.c_str();               // &source[0]

    // 셰이더 소스를 id에 적용하고 이를 컴파일합니다.
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // 에러 처리
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // 만약 컴파일이 실패한다면 에러 로그를 출력 후 0을 반환합니다.
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

    // 셰이더 아이디값을 반환합니다.
    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // 프로그램에 각 쉐이더를 적용한 뒤, 링크해주고, 기능을 켜준다.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // 쓰고 난 뒤 쉐이더는 지운다.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // 프로그램을 반환한다.
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
    (레이아웃이란? 쉐이더에서 사용하는 인덱스같은 놈이다. 정확히는 attribute position)
    size : vertex attrib를 구성하는 멤버의 갯수, 예로 들어 vector position 속성이 x,y,z,w 즉 homogeneous 좌표계라면 4, x,y,z 로 구성되면 3
    즉 1,2,3,4 중 하나만 가능하다.
    type : 정점 attrib를 구성하는 멤버 타입
    normalized : 타입이 float나 GL_HALF_FLOAT가 아닌 경우, unsigned인 경우 0..1 signed인 경우 -1..1의 float로 변경한다.
    stribe pointer가 가리키는 배열에서 다음 구성 요소까지의 바이트 수
    pointer : vertex attrib의 배열 주소 const void* 타입으로 지정 가능하다.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // 셰이더 파일을 불러옵니다.
    ShaderProgramSource source = ParseShader("./Basic.shader");
    std::cout << source.vertexSource << std::endl;
    std::cout << source.fragmentSource << std::endl;

    // 셰이더를 적용하고 사용 선언합니다.
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

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