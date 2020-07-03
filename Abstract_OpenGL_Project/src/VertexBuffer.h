#pragma once

// 버텍스 버퍼 클래스입니다.
class VertexBuffer
{
private : 
	// 버퍼의 아이디입니다. (내부의 렌더러 아이디입니다.)
	unsigned int m_RendererID;

public : 
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};