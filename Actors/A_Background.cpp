#include "A_Background.h"

void A_Background::Init(QOpenGLFunctions_3_3_Core* core_)
{
	core = core_;
	material_mesh.SetOpenglCore(core);
	material_mesh.LoadShaders();
	material_mesh.SetShadersData(background_vertices, 1.0, 1.0, 1.0);
}

void A_Background::Render()
{
	material_mesh.Render();
}

void A_Background::SetBackgroudColor(double r, double g, double b)
{
	material_mesh.SetShadersData(background_vertices, r, g, b);
}
