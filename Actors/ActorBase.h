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

	virtual void Init(QOpenGLFunctions_3_3_Core* core_) = 0;
	virtual void Render() = 0;

	inline void SetOpenglCore(QOpenGLFunctions_3_3_Core* core_) { core = core_; }
};

