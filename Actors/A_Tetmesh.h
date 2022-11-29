#pragma once
#include "ActorModelBase.h"
#include "cinolib/meshes/tetmesh.h"

class A_Tetmesh :
	public ActorModelBase
{
public:
	A_Tetmesh() {};
	~A_Tetmesh() {}

	void Render() override;

	void LoadMesh(const std::string& path) override;
	void SaveMesh(const std::string& path) override;

	std::string GetMeshFileName() const { return filename; }
	std::string GetMeshFileType()const { return filetype; }
	std::string GetMeshFilePath()const { return filepath; }
	std::string GetMeshFileFullPath() const { return filepath + filename + "." + filetype; }
private:
	cinolib::Tetmesh<> mesh;
};


