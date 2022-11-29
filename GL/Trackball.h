#pragma once


#include <QVector3D>
#include <QVector2D>

class Traceball
{
public:
	Traceball(){}
	Traceball(int width_, int height_, double radius_ = 0.5) : radius(radius_), width(width_), height(height_) {}
	~Traceball() {}

	void ResetTrace(); //停止追踪
	void TrackballToRotations(const QVector2D& p, QVector3D& axis, double& angle, bool& valid); //第一次调用valid为false，之后都是true；angle为角度制

	double radius = 0.5; //轨迹球半径
	int width = 600, height = 480; //窗口尺寸

private:
	QVector3D TrackballToSphere(const QVector2D& p); //2D坐标映射到球面上的3D坐标
	void TrackballToRotations(const QVector3D& p0, const QVector3D& p1, QVector3D& axis, double& angle); //计算p0到p1的旋转轴和角度（角度制）

	bool is_pressed = false; //其否启动追踪
	QVector2D last_click_2d = { 0,0 };
	QVector3D last_click_3d = { 0,0,0 };
};

