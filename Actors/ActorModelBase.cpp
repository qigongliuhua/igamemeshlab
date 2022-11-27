#include "ActorModelBase.h"

void ActorModelBase::init(QOpenGLFunctions_3_3_Core* core_)
{
	assert(camera != nullptr && light != nullptr);
	core = core_;
	material.set_opengl(core);
	material.load_shaders();
	material_line.set_opengl(core);
	material_line.load_shaders();
	material_point.set_opengl(core);
	material_point.load_shaders();
	material_face_normal_lines.set_opengl(core);
	material_face_normal_lines.load_shaders();
	material_vertex_normal_lines.set_opengl(core);
	material_vertex_normal_lines.load_shaders();
}

void ActorModelBase::init(QOpenGLFunctions_3_3_Core* core_, Camera* camera_, Light* light_)
{
	core = core_;
	camera = camera_;
	light = light_;
	material.set_opengl(core);
	material.load_shaders();
	material_line.set_opengl(core);
	material_line.load_shaders();
	material_point.set_opengl(core);
	material_point.load_shaders();
	material_face_normal_lines.set_opengl(core);
	material_face_normal_lines.load_shaders();
	material_vertex_normal_lines.set_opengl(core);
	material_vertex_normal_lines.load_shaders();
}
