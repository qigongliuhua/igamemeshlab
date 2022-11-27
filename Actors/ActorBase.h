#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include "Materials.h"
#include "Camera.h"

class ActorBase
{
protected:
	QOpenGLFunctions_3_3_Core* core = nullptr;
public:
	ActorBase(){}
	virtual ~ActorBase(){}

	virtual void init(QOpenGLFunctions_3_3_Core* core_) = 0;
	virtual void render() = 0;

	void set_opengl(QOpenGLFunctions_3_3_Core* core_) { core = core_; }
};

