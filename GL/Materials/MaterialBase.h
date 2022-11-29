#pragma once


#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>
#include <Eigen/Core>
#include "Camera.h"
#include "MaterialEnum.h"

class MaterialBase
{
public:
	MaterialBase() {}
	MaterialBase(QOpenGLFunctions_3_3_Core* core_) : core(core_) {}
	virtual ~MaterialBase();

	inline	void SetOpenglCore	(QOpenGLFunctions_3_3_Core* core_) { core = core_; }
	virtual void LoadShaders() = 0;
	virtual	void SetShadersData	(std::vector<GLfloat>& verts, int vertex_size, std::vector<int> offset_in_one_vertex);
	virtual	void Render		(Camera& camera);
	virtual	void Render(QMatrix4x4& M, QMatrix4x4& V, QMatrix4x4& P);
	virtual	void Render		();

protected:
	void ClearShadersData();

	QOpenGLFunctions_3_3_Core* core = nullptr;
	QOpenGLShaderProgram shader;
	std::vector<GLuint> VAOs = {0};
	std::vector<GLuint> VBOs = {0};
	GLuint drawable_size = 0;	//可绘制单元数量
};

