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

    void LoadShaders() override;
	// 顶点坐标+法线+颜色
    void SetShadersData(const std::vector<GLfloat>& verts);
    void SetShadersData(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
    void SetShadersData(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void Render(Camera& camera, Light& light);

private:


};

