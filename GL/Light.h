#pragma once

#include <QVector3D>

class Light
{
public:
	Light() {};
	Light(QVector3D pos): light_pos(pos) {};
	Light(QVector3D pos, QVector3D color): light_pos(pos), light_color(color) {};
	~Light() {};

	QVector3D light_pos = { 0,0,0 };
	QVector3D light_color = { 1,1,1 };
};
