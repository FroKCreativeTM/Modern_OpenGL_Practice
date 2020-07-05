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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    // 각 요소를 바인딩한다.
    shader.Bind();
    va.Bind();
    ib.Bind();

    // 다양한 인덱스 타입에 대응할 수 있어야한다. (또한 성능에 대한 고려 또한 필요하다.)
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
