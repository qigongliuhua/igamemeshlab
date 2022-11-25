#pragma once

#include "MaterialBase.h"

#include "Light.h"




class M_BlinnPhong :
    public MaterialBase
{

public:
    M_BlinnPhong() {}
    M_BlinnPhong(QOpenGLFunctions_3_3_Core* core_) : MaterialBase(core_) {}
    ~M_BlinnPhong() {};

    void load_shaders() override;
    void set_shaders(const std::vector<GLfloat>& verts);
    void set_shaders(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
    void set_shaders(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);

	void render(Camera& camera, Light& light);
};

