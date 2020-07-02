#include "Renderer.h"

#include <iostream>

/* 에러가 없는지 체크합니다. 에러 발견 시 바로 break */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* 에러가 발생하면 이를 출력하고 프로그램을 멈춥니다. */
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function
            << " " << file << " : " << line << std::endl;
        return false;
    }
    return true;
}