#include "Renderer.h"

#include <iostream>

/* ������ ������ üũ�մϴ�. ���� �߰� �� �ٷ� break */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* ������ �߻��ϸ� �̸� ����ϰ� ���α׷��� ����ϴ�. */
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
    // �� ��Ҹ� ���ε��Ѵ�.
    shader.Bind();
    va.Bind();
    ib.Bind();

    // �پ��� �ε��� Ÿ�Կ� ������ �� �־���Ѵ�. (���� ���ɿ� ���� ��� ���� �ʿ��ϴ�.)
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
