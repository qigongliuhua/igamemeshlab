#include "A_Background.h"

void A_Background::init(QOpenGLFunctions_3_3_Core* core_)
{
	core = core_;
	material.set_opengl(core);
	material.load_shaders();
	material.set_shaders(back_ground_vertices, 1.0, 1.0, 1.0);
}

void A_Background::render()
{
	material.render();
}

void A_Background::set_color(double r, double g, double b)
{
	material.set_shaders(back_ground_vertices, r, g, b);
}
