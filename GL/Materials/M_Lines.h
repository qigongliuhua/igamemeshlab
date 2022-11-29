#pragma once

#include "MaterialBase.h"
#include <QVector3D>

class M_Lines :
	public MaterialBase
{
public:
	M_Lines() {}
	M_Lines(QOpenGLFunctions_3_3_Core* core_) : MaterialBase(core_) {}
	~M_Lines() {};

	void LoadShaders() override;
	void SetShadersData(const std::vector<GLfloat>& verts_and_colors);
	void SetShadersData(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
	void SetShadersData(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void Render(Camera& camera) override;
	void Render(QMatrix4x4& M, QMatrix4x4& V, QMatrix4x4& P) override;

	GLfloat width = 1.0;;
};
