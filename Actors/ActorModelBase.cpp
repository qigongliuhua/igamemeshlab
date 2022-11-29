#include "ActorModelBase.h"

void ActorModelBase::Init(QOpenGLFunctions_3_3_Core* core_)
{
	assert(camera != nullptr && light != nullptr);
	core = core_;
	material_mesh.SetOpenglCore(core);
	material_mesh.LoadShaders();
	material_mesh_edge.SetOpenglCore(core);
	material_mesh_edge.LoadShaders();
	material_mesh_vertex.SetOpenglCore(core);
	material_mesh_vertex.LoadShaders();
	material_mesh_face_normal.SetOpenglCore(core);
	material_mesh_face_normal.LoadShaders();
	material_face_vertex_normal.SetOpenglCore(core);
	material_face_vertex_normal.LoadShaders();
}

void ActorModelBase::Init(QOpenGLFunctions_3_3_Core* core_, Camera* camera_, Light* light_)
{
	core = core_;
	camera = camera_;
	light = light_;
	material_mesh.SetOpenglCore(core);
	material_mesh.LoadShaders();
	material_mesh_edge.SetOpenglCore(core);
	material_mesh_edge.LoadShaders();
	material_mesh_vertex.SetOpenglCore(core);
	material_mesh_vertex.LoadShaders();
	material_mesh_face_normal.SetOpenglCore(core);
	material_mesh_face_normal.LoadShaders();
	material_face_vertex_normal.SetOpenglCore(core);
	material_face_vertex_normal.LoadShaders();
}
