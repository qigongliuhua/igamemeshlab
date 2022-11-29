#pragma once
#include "ActorModelBase.h"
#include "cinolib/meshes/polygonmesh.h"

class A_Polygonmesh :
	public ActorModelBase
{
public:
	A_Polygonmesh() {};
	~A_Polygonmesh() {}

	void render() override;

	void load_model(const std::string& path) override;
	void save_model(const std::string& path) override;

	std::string get_filename() const { return filename; }
	std::string get_filetype()const { return filetype; }
	std::string get_filepath()const { return filepath; }
	std::string get_full_filepath() const { return filepath + filename + "." + filetype; }
private:
	cinolib::Polygonmesh<> mesh;
};


