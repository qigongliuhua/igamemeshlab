#pragma once

#include "ActorBase.h"



class A_Background:
	public ActorBase
{
public:
	A_Background() {};
	~A_Background() {}

	void Init(QOpenGLFunctions_3_3_Core* core_) override;
	void Render() override;


	void SetBackgroudColor(double r, double g, double b); //设置背景颜色
private:
	M_Flat material_mesh;
	Camera camera;

	// 渐变背景，其实是一个长方形面片，绘制在最底下
	const std::vector<GLfloat> background_vertices{
		// 顶点位置        
		1.0f, 1.0f, 0.9999f,   // 右上角
		-1.0f, -1.0f, 0.9999f, // 左下角
		1.0f, -1.0f, 0.9999f,  // 右下角
		-1.0f, 1.0f, 0.9999f,   // 左上角
		-1.0f, -1.0f, 0.9999f, // 左下角
		1.0f, 1.0f, 0.9999f,   // 右上角
	};
};

