#include "ShaderUtils.h"
#include <iostream>
#include <fstream>
#include <QFile>



void ShaderUtils::LoadShaderString(const std::string& shader, std::string& vert_shader, std::string& frag_shader)
{
	std::stringstream ss(shader);
	std::string line;
	int mode = -1; // 0:顶点着色器 1:片元着色器
	std::stringstream vert, frag;
	bool dump = false;

	auto trim = [](std::string& s){
		std::string blanks("\f\v\r\t\n ");
		s.erase(s.find_last_not_of(blanks) + 1);
	};

	while (std::getline(ss, line)) {
		trim(line); //剔除空白字符

		if (line.empty()) continue;

		if (line == "#vertexshader") {
			mode = 0;
		}
		else if (line == "#fragmentshader") {
			mode = 1;
		}
		else {
			if (mode == 0) {
				vert << line << std::endl;
			}
			else if (mode == 1) {
				frag << line << std::endl;
			}
		}
	}
	vert_shader = vert.str();
	frag_shader = frag.str();

	if (vert_shader.empty() || frag_shader.empty()) {
		std::cout << "#error: shader is not complete, please check it. " << std::endl;
		dump = true;
	}

	if (dump) {
		std::cout << std::endl;
		std::cout << "##################dump shader file#######################" << std::endl;
		std::cout << shader << std::endl;
		std::cout << "#########################################################" << std::endl << std::endl;
	}
}

void ShaderUtils::LoadShaderString(const std::string& shader, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader)
{
	std::string vert_code, frag_code;
	LoadShaderString(shader, vert_code, frag_code);

	vert_shader.compileSourceCode(vert_code.c_str());
	frag_shader.compileSourceCode(frag_code.c_str());
}

void ShaderUtils::LoadShaderString(const std::string& shader, QOpenGLShaderProgram& shader_program)
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
	LoadShaderString(shader, vertexShader, fragmentShader);
	shader_program.addShader(&vertexShader);
	shader_program.addShader(&fragmentShader);
	shader_program.link();
}

void ShaderUtils::LoadShaderFileFromQrc(const std::string& qrc_url, std::string& vert_shader, std::string& frag_shader)
{
	QFile f(qrc_url.c_str());
	if (f.exists() && f.open(QIODevice::ReadOnly)) {
		LoadShaderString(f.readAll().toStdString(), vert_shader, frag_shader);
	}
	else {
		std::cout << "#error: can't open shader file : " << qrc_url << std::endl;
	}
}

void ShaderUtils::LoadShaderFileFromQrc(const std::string& qrc_url, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader)
{
	std::string vert_code, frag_code;
	LoadShaderFileFromQrc(qrc_url, vert_code, frag_code);

	vert_shader.compileSourceCode(vert_code.c_str());
	frag_shader.compileSourceCode(frag_code.c_str());
}

void ShaderUtils::LoadShaderFileFromQrc(const std::string& qrc_url, QOpenGLShaderProgram& shader_program)
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
	LoadShaderFileFromQrc(qrc_url, vertexShader, fragmentShader);
	shader_program.addShader(&vertexShader);
	shader_program.addShader(&fragmentShader);
	shader_program.link();
}

void ShaderUtils::LoadShaderFile(const std::string& path, std::string& vert_shader, std::string& frag_shader)
{
	std::fstream f;
	f.open(path.c_str(), std::ios::in);
	if (f.is_open()) {
		std::stringstream ss;
		std::string line;
		while (std::getline(f, line))
		{
			ss << line << std::endl;
		}
		LoadShaderString(ss.str(), vert_shader, frag_shader);
	}
	else {
		std::cout << "#error: can't open shader file : " << path << std::endl;
	}
}

void ShaderUtils::LoadShaderFile(const std::string& path, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader)
{
	std::string vert_code, frag_code;
	LoadShaderFile(path, vert_code, frag_code);

	vert_shader.compileSourceCode(vert_code.c_str());
	frag_shader.compileSourceCode(frag_code.c_str());
}

void ShaderUtils::LoadShaderFile(const std::string& path, QOpenGLShaderProgram& shader_program)
{
	QOpenGLShader vertexShader(QOpenGLShader::Vertex);
	QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
	LoadShaderFile(path, vertexShader, fragmentShader);
	shader_program.addShader(&vertexShader);
	shader_program.addShader(&fragmentShader);
	shader_program.link();
}



