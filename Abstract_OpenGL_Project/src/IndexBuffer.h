#pragma once
class IndexBuffer
{
private:
	// ������ ���̵��Դϴ�. (������ ������ ���̵��Դϴ�.)
	unsigned int m_RendererID;
	// �󸶳� ���� �ε����� ���� üũ�ϴ� �����Դϴ�.
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int size);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

