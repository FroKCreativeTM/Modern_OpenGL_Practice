#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* �� ���� ���۸� �����ϴ� �������Դϴ�. */

    // �ϴ� ó������ �� ������ ���̵� �����մϴ�.
    GLCall(glGenBuffers(1, &m_RendererID));
    // �� ������ ���̵� ���� �迭 ���۸� ���ε��մϴ�.
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // �� ���ۿ� �����ʹ� data�̸�, ������ ������� size��ŭ ���ϴ�.
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