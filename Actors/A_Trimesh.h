#pragma once
#include "ActorBase.h"
class A_Trimesh :
    public ActorBase
{
public:
	A_Trimesh() {};
	~A_Trimesh() {}

	void init(QOpenGLFunctions_3_3_Core* core_) override;
	void render() override;

private:
	M_BlinnPhong material;
	Camera camera;
};

