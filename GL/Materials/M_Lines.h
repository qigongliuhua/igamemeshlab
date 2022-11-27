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

	void load_shaders() override;
	void set_shaders(const std::vector<GLfloat>& verts_and_colors);
	void set_shaders(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
	void set_shaders(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void render(Camera& camera) override;
	void render(QMatrix4x4& M, QMatrix4x4& V, QMatrix4x4& P) override;

	GLfloat width = 1.0;;
};
