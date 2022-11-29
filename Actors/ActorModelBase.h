#pragma once

#include "ActorBase.h"
#include <QColor>

class ActorModelBase :
	public ActorBase {

public:
	ActorModelBase() {}
	virtual ~ActorModelBase() {}

	void Init(QOpenGLFunctions_3_3_Core* core_) override;
	void Init(QOpenGLFunctions_3_3_Core* core_, Camera* camera_ = nullptr, Light* light_ = nullptr); //请在opengl的环境下调用，否则没用

	virtual void LoadMesh(const std::string& path) = 0;
	virtual void SaveMesh(const std::string& path) = 0;

	inline void SetLight(Light* light_) { light = light_; }
	inline void SetCamera(Camera* camera_) { camera = camera_; }
	inline void SetNormalMode(Normal_Mode mode) { material_mesh.normal_mode = mode; }

	inline void ShowMesh() { flag_show_mesh = true;  need_update_gl = true; }
	inline void HideMesh() { flag_show_mesh = false;  need_update_gl = true; }
	inline void ToggleMeshVisibility() { flag_show_mesh = !flag_show_mesh;  need_update_gl = true; } //是否显示模型

	inline void ShowMeshEdge() { flag_show_mesh_edge = true; need_update_gl = true; } //显示线框
	inline void HideMeshEdge() { flag_show_mesh_edge = false; need_update_gl = true; } //隐藏线框
	inline void ToggleMeshEdgeVisibility() { flag_show_mesh_edge = !flag_show_mesh_edge;  need_update_gl = true; } //切换线框显示模式

	inline void ShowMeshVertex() { flag_show_mesh_vertex = true; need_update_gl = true; } //显示顶点
	inline void HideMeshVertex() { flag_show_mesh_vertex = false; need_update_gl = true; } //隐藏顶点
	inline void ToggleMeshVertexVisibility() { flag_show_mesh_vertex = !flag_show_mesh_vertex;  need_update_gl = true; } //顶点显示模式

	inline void ShowMeshFaceNormal() { flag_show_mesh_face_normal = true; need_update_gl = true; } //显示面法线
	inline void HideMeshFaceNormal() { flag_show_mesh_face_normal = false; need_update_gl = true; } //隐藏面法线
	inline void ToggleMeshFaceNormalVisibility() { flag_show_mesh_face_normal = !flag_show_mesh_face_normal;  need_update_gl = true; } //面法线显示模式

	inline void ShowMeshVertexNormal() { flag_show_mesh_vertex_normal = true; need_update_gl = true; } //显示顶点法线
	inline void HideMeshVertexNormal() { flag_show_mesh_vertex_normal = false; need_update_gl = true; } //隐藏顶点法线
	inline void ToggleMeshVertexNormalVisibility() { flag_show_mesh_vertex_normal = !flag_show_mesh_vertex_normal;  need_update_gl = true; } //顶点法线显示模式


	inline void SetMeshEdgeDisplayColor(QColor color) { mesh_edge_color = color;  need_update_gl = true; } //线框颜色
	inline void SetMeshVertexDisplayColor(QColor color) { mesh_vertex_color = color;  need_update_gl = true; } //顶点颜色
	inline void SetMeshFaceNormalDisplayColor(QColor color) { mesh_face_normal_color = color;  need_update_gl = true; } //面法线颜色
	inline void SetMeshVertexNormalDisplayColor(QColor color) { mesh_vertex_normal_color = color;  need_update_gl = true; } //顶点法线颜色

	inline void SetSliceRateX(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_x = rate; need_update_gl = true; } //设置切片比例
	inline void SetSliceRateY(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_y = rate; need_update_gl = true; } //设置切片比例
	inline void SetSliceRateZ(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_z = rate; need_update_gl = true; } //设置切片比例
	inline void SetSliceInverseX(bool is_inverse) { slice_inverse_x = is_inverse; need_update_gl = true; } //设置是否反转切片方向
	inline void SetSliceInverseY(bool is_inverse) { slice_inverse_y = is_inverse; need_update_gl = true; } //设置是否反转切片方向
	inline void SetSliceInverseZ(bool is_inverse) { slice_inverse_z = is_inverse; need_update_gl = true; } //设置是否反转切片方向
	inline void GetCurSliceStatus(double& xrate, double& yrate, double& zrate, bool& xinv, bool& yinv, bool& zinv) {
		xrate = slice_rate_x; yrate = slice_rate_y; zrate = slice_rate_z;
		xinv = slice_inverse_x; yinv = slice_inverse_y; zinv = slice_inverse_z;
	}

	inline std::string GetMeshFileName() const { return filename; }
	inline std::string GetMeshFileType()const { return filetype; }
	inline std::string GetMeshFilePath()const { return filepath; }
	inline std::string GetMeshFileFullPath() const { return filepath + filename + "." + filetype; }

	inline bool IsRenderReady() { return camera != nullptr && light != nullptr && core != nullptr; };

protected:
	std::string filepath, filename, filetype;

	bool flag_show_mesh = true;
	M_BlinnPhong material_mesh; //网格

	bool need_update_gl = true; //刷新顶点数据，刷新完自动被置false，防止重复刷新

	QColor mesh_edge_color = { 0,0,0 };
	bool flag_show_mesh_edge = true;
	M_Lines material_mesh_edge; //线框

	QColor mesh_vertex_color = { 255,0,0 };
	bool flag_show_mesh_vertex = false;
	M_Points material_mesh_vertex; //顶点

	double normal_dislay_length_zoom = 0.05; //法线长度

	QColor mesh_face_normal_color = { 0,0,255 };
	bool flag_show_mesh_face_normal = false;
	M_Lines material_mesh_face_normal; //面法线

	QColor mesh_vertex_normal_color = { 0,255,0 };
	bool flag_show_mesh_vertex_normal = false;
	M_Lines material_face_vertex_normal; //顶点法线

	double slice_rate_x = 0, slice_rate_y = 0, slice_rate_z = 0; //切片百分比
	bool slice_inverse_x = false, slice_inverse_y = false, slice_inverse_z = false; //是否反转切片方向

	Camera* camera = nullptr;
	Light* light = nullptr;
};
