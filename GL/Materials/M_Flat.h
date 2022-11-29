#pragma once
#include "MaterialBase.h"


class M_Flat :
    public MaterialBase
{
public:
    M_Flat() {}
    M_Flat(QOpenGLFunctions_3_3_Core* core_) : MaterialBase(core_) {}
    ~M_Flat() {};

    void LoadShaders() override;
    void SetShadersData(const std::vector<GLfloat>& verts_and_colors);
    void SetShadersData(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors);
    void SetShadersData(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b);
};

