#include "A_WorldCoordinate.h"

void A_WorldCoordinate::Init(QOpenGLFunctions_3_3_Core* core_)
{
	core = core_;
	material_mesh.SetOpenglCore(core);
	material_mesh.LoadShaders();
	material_mesh.width = 1.0; //线段宽度

	// 画平行x
	int limitx = abs(verts[0]);
	for (int i = -50; i <= 50; ++i) {
		if (i == 0) continue;
		verts.push_back(-limitx);
		verts.push_back(0);
		verts.push_back(i);
		verts.push_back(othercolor.redF());
		verts.push_back(othercolor.greenF());
		verts.push_back(othercolor.blueF());
		verts.push_back(limitx);
		verts.push_back(0);
		verts.push_back(i);
		verts.push_back(othercolor.redF());
		verts.push_back(othercolor.greenF());
		verts.push_back(othercolor.blueF());
	}
	// 画平行z
	for (int i = -50; i <= 50; ++i) {
		if (i == 0) continue;
		verts.push_back(i);
		verts.push_back(0);
		verts.push_back(-limitx);
		verts.push_back(othercolor.redF());
		verts.push_back(othercolor.greenF());
		verts.push_back(othercolor.blueF());
		verts.push_back(i);
		verts.push_back(0);
		verts.push_back(limitx);
		verts.push_back(othercolor.redF());
		verts.push_back(othercolor.greenF());
		verts.push_back(othercolor.blueF());
	}

	material_mesh.SetShadersData(verts);
}

void A_WorldCoordinate::Render()
{
	if (flag_show)
	{
		QMatrix4x4 M;
		material_mesh.Render(camera->model, camera->view, camera->projection);
	}
}

