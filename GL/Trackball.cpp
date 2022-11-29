#include "Trackball.h"

#include <iostream>
#include <cmath>

void Traceball::ResetTrace()
{
	is_pressed = false;
	last_click_2d = QVector2D(0, 0);
	last_click_3d = QVector3D(0, 0, 0);
}

void Traceball::TrackballToRotations(const QVector2D& p, QVector3D& axis, double& angle, bool& valid)
{
	QVector3D pos3d = TrackballToSphere(p);
	if (is_pressed == false) {
		valid = false;
		is_pressed = true;
	}
	else {
		valid = true;
		TrackballToRotations(last_click_3d, pos3d, axis, angle);
	}
	last_click_2d = p;
	last_click_3d = pos3d;
}

QVector3D Traceball::TrackballToSphere(const QVector2D& p)
{
	double x = (2 * p.x() - width) / width;
	double y = -(2 * p.y() - height) / height;
	double n2 = x * x + y * y;
	double r2 = radius * radius;
	return QVector3D(x, y, (n2 < 0.5 * r2) ? sqrt(r2 - n2) : 0.5 * r2 / sqrt(n2));
}

void Traceball::TrackballToRotations(const QVector3D& p0, const QVector3D& p1, QVector3D& axis, double& angle)
{
	axis = QVector3D::crossProduct(p0, p1);
	if (axis.length() < 1e-7) axis = QVector3D(1, 0, 0);
	axis.normalize();
	double dot_ = QVector3D::dotProduct(p0.normalized(), p1.normalized());
	dot_ = std::max(-1.0, std::min(1.0, dot_));
	angle = acos(dot_) * 180.0 / M_PI;
}
