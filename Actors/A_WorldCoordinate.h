#pragma once

#include "ActorBase.h"
#include <QColor>


class A_WorldCoordinate :
	public ActorBase
{
public:
	A_WorldCoordinate() {};
	~A_WorldCoordinate() {}

	void Init(QOpenGLFunctions_3_3_Core* core_) override;
	void Render() override;

	void SetCamera(Camera* camera_) { camera = camera_; }

	void ToggleVisibility() { flag_show = !flag_show; }
	void Show() { flag_show = true; }
	void Hide() { flag_show = false; }

private:
	bool flag_show = false;

	M_Lines material_mesh;
	Camera* camera = nullptr;

	QColor xcolor = { 255,0,0 };
	QColor ycolor = { 0,255,0 };
	QColor zcolor = { 0,0,255 };
	QColor othercolor = { 200,200,200 };

	// 渐变背景，其实是一个长方形面片，绘制在最底下
	std::vector<GLfloat> verts{
		// 顶点位置        
		-100000.0f, 0, 0, 1, 0, 0,   // -x
		100000.0f, 0, 0, 1, 0, 0,   // +x
		0, -100000.0f, 0, 0, 1, 0,   // -y
		0, 100000.0f, 0, 0, 1, 0,   // +y
		0, 0, -100000.0f, 0, 0, 1,   // -z
		0, 0, 100000.0f, 0, 0, 1,   // +z
	};
};

