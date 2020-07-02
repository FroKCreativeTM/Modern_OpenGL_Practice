#pragma once
class IndexBuffer
{
private:
	// 버퍼의 아이디입니다. (내부의 렌더러 아이디입니다.)
	unsigned int m_RendererID;
	// 얼마나 많은 인덱스가 들어가나 체크하는 변수입니다.
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

