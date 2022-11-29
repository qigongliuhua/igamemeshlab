#pragma once

#include "MaterialBase.h"
#include <QVector3D>

class M_Points :
	public MaterialBase
{
public:
	M_Points() {}
	M_Points(QOpenGLFunctions_3_3_Core* core_) : MaterialBase(core_) {}
	~M_Points() {};

	void LoadShaders() override;
	void SetShadersData(const std::vector<GLfloat>& verts_and_colors);
	void SetShadersData(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
	void SetShadersData(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void Render(Camera& camera) override;
};
