#include "A_WorldCoordinate.h"

void A_WorldCoordinate::init(QOpenGLFunctions_3_3_Core* core_)
{
	core = core_;
	material.set_opengl(core);
	material.load_shaders();
	material.width = 1.0; //线段宽度

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

	material.set_shaders(verts);
}

void A_WorldCoordinate::render()
{
	if (flag_show)
	{
		QMatrix4x4 M;
		material.render(camera->model, camera->view, camera->projection);
	}
}

