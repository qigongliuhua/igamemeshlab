#pragma once
#include "ActorModelBase.h"
#include "cinolib/meshes/polyhedralmesh.h"

class A_Polyhedralmesh :
	public ActorModelBase
{
public:
	A_Polyhedralmesh() {};
	~A_Polyhedralmesh() {}

	void Render() override;

	void LoadMesh(const std::string& path) override;
	void SaveMesh(const std::string& path) override;

	std::string GetMeshFileName() const { return filename; }
	std::string GetMeshFileType()const { return filetype; }
	std::string GetMeshFilePath()const { return filepath; }
	std::string GetMeshFileFullPath() const { return filepath + filename + "." + filetype; }
private:
	cinolib::Polyhedralmesh<> mesh;
};


