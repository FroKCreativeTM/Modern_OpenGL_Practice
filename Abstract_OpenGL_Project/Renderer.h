#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public : 
    // 그리는 것은 다양하다. 
    // 하지만 그리기 위해서는 우리는 보통 정점 배열, 인덱스 배열, 셰이더가 필요하다.
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};