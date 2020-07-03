#pragma once

#include <string>
#include <unordered_map>	// 해시맵

/* 셰이더 종류마다 저장할 각 셰이더 소스 클래스입니다. */
struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private : 
	std::string m_fileName;
	unsigned int m_RendererID;
	// caching for uniforms
	std::unordered_map<std::string, int> m_uniforLocationCache;

public : 
	Shader(const std::string filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private : 
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int shaderType, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string fragmentShader);
	int GetUniformLocation(const std::string& name);
};

