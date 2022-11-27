#pragma once

#include "MaterialBase.h"

#include "Light.h"




class M_BlinnPhong :
    public MaterialBase
{

public:
	Normal_Mode normal_mode = Normal_Mode::Flat;
	
    M_BlinnPhong() {}
    M_BlinnPhong(QOpenGLFunctions_3_3_Core* core_) : MaterialBase(core_) {}
    ~M_BlinnPhong() {};

    void load_shaders() override;
	// 顶点坐标+法线+颜色
    void set_shaders(const std::vector<GLfloat>& verts);
    void set_shaders(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
    void set_shaders(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void render(Camera& camera, Light& light);

private:


};

