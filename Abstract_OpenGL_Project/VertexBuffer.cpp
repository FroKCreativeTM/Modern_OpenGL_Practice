#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* 이 정점 버퍼를 생성하는 생성자입니다. */

    // 일단 처음으로 이 렌더러 아이디를 지정합니다.
    GLCall(glGenBuffers(1, &m_RendererID));
    // 이 렌더러 아이디를 통해 배열 버퍼를 바인딩합니다.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // 이 버퍼에 데이터는 data이며, 데이터 사이즈는 size만큼 들어갑니다.
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}