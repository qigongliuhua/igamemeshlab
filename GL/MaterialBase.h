#pragma once


#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>
#include <Eigen/Core>
#include "Camera.h"

class MaterialBase
{
public:
	MaterialBase() {}
	MaterialBase(QOpenGLFunctions_3_3_Core* core_) : core(core_) {}
	virtual ~MaterialBase();

	inline	void set_opengl	(QOpenGLFunctions_3_3_Core* core_) { core = core_; }
	virtual void load_shaders() = 0;
	virtual	void set_shaders	(std::vector<GLfloat>& verts, int vertex_size, std::vector<int> offset_in_one_vertex);
	virtual	void render		(Camera& camera);
	virtual	void render		();

protected:
	void clear();

	QOpenGLFunctions_3_3_Core* core = nullptr;
	QOpenGLShaderProgram shader;
	std::vector<GLuint> VAOs = {0};
	std::vector<GLuint> VBOs = {0};
	GLuint drawable_size = 0;	//可绘制单元数量
};

