#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

class ShaderUtils {

public:
	ShaderUtils() {};
	~ShaderUtils() {};



	static void LoadShaderString(const std::string& shader, std::string& vert_shader, std::string& frag_shader);
	static void LoadShaderString(const std::string& shader, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader);
	static void LoadShaderString(const std::string& shader, QOpenGLShaderProgram& shader_program);

	static void LoadShaderFileFromQrc(const std::string& qrc_url, std::string& vert_shader, std::string& frag_shader);
	static void LoadShaderFileFromQrc(const std::string& qrc_url, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader);
	static void LoadShaderFileFromQrc(const std::string& qrc_url, QOpenGLShaderProgram& shader_program);

	static void LoadShaderFile(const std::string& path, std::string& vert_shader, std::string& frag_shader);
	static void LoadShaderFile(const std::string& path, QOpenGLShader& vert_shader, QOpenGLShader& frag_shader);
	static void LoadShaderFile(const std::string& path, QOpenGLShaderProgram& shader_program);

};
