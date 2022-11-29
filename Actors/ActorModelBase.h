#pragma once

#include "ActorBase.h"
#include <QColor>

class ActorModelBase :
	public ActorBase {

public:
	ActorModelBase() {}
	virtual ~ActorModelBase() {}

	void init(QOpenGLFunctions_3_3_Core* core_) override;
	void init(QOpenGLFunctions_3_3_Core* core_, Camera* camera_ = nullptr, Light* light_ = nullptr); //请在opengl的环境下调用，否则没用

	virtual void load_model(const std::string& path) = 0;
	virtual void save_model(const std::string& path) = 0;

	inline void set_light(Light* light_) { light = light_; }
	inline void set_camera(Camera* camera_) { camera = camera_; }
	inline void set_normal_mode(Normal_Mode mode) { material.normal_mode = mode; }

	inline void show_model() { flag_show_model = true;  need_update_gl = true; }
	inline void hide_model() { flag_show_model = false;  need_update_gl = true; }
	inline void toggle_model() { flag_show_model = !flag_show_model;  need_update_gl = true; } //是否显示模型

	inline void show_lines() { flag_show_lines = true; need_update_gl = true; } //显示线框
	inline void hide_lines() { flag_show_lines = false; need_update_gl = true; } //隐藏线框
	inline void toggle_model_lines() { flag_show_lines = !flag_show_lines;  need_update_gl = true; } //切换线框显示模式

	inline void show_model_points() { flag_show_points = true; need_update_gl = true; } //显示顶点
	inline void hide_model_points() { flag_show_points = false; need_update_gl = true; } //隐藏顶点
	inline void toggle_model_points() { flag_show_points = !flag_show_points;  need_update_gl = true; } //顶点显示模式

	inline void show_face_normal_lines() { flag_face_normal_lines = true; need_update_gl = true; } //显示面法线
	inline void hide_face_normal_lines() { flag_face_normal_lines = false; need_update_gl = true; } //隐藏面法线
	inline void toggle_face_normal_lines() { flag_face_normal_lines = !flag_face_normal_lines;  need_update_gl = true; } //面法线显示模式

	inline void show_vertex_normal_lines() { flag_vertex_normal_lines = true; need_update_gl = true; } //显示顶点法线
	inline void hide_vertex_normal_lines() { flag_vertex_normal_lines = false; need_update_gl = true; } //隐藏顶点法线
	inline void toggle_vertex_normal_lines() { flag_vertex_normal_lines = !flag_vertex_normal_lines;  need_update_gl = true; } //顶点法线显示模式


	inline void set_lines_color(QColor color) { lines_color = color;  need_update_gl = true; } //线框颜色
	inline void set_points_color(QColor color) { point_color = color;  need_update_gl = true; } //顶点颜色
	inline void set_face_normal_lines_color(QColor color) { face_normal_lines_color = color;  need_update_gl = true; } //面法线颜色
	inline void set_vertex_normal_lines_color(QColor color) { vertex_normal_lines_color = color;  need_update_gl = true; } //顶点法线颜色

	inline void set_slice_rate_x(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_x = rate; need_update_gl = true; } //设置切片比例
	inline void set_slice_rate_y(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_y = rate; need_update_gl = true; } //设置切片比例
	inline void set_slice_rate_z(double rate) { assert(rate >= 0 && rate <= 100); slice_rate_z = rate; need_update_gl = true; } //设置切片比例
	inline void set_slice_inverse_x(bool is_inverse) { slice_inverse_x = is_inverse; need_update_gl = true; } //设置是否反转切片方向
	inline void set_slice_inverse_y(bool is_inverse) { slice_inverse_y = is_inverse; need_update_gl = true; } //设置是否反转切片方向
	inline void set_slice_inverse_z(bool is_inverse) { slice_inverse_z = is_inverse; need_update_gl = true; } //设置是否反转切片方向

	inline void get_slice_status(double& xrate, double& yrate, double& zrate, bool& xinv, bool& yinv, bool& zinv) {
		xrate = slice_rate_x; yrate = slice_rate_y; zrate = slice_rate_z;
		xinv = slice_inverse_x; yinv = slice_inverse_y; zinv = slice_inverse_z;
	}

	inline std::string get_filename() const { return filename; }
	inline std::string get_filetype()const { return filetype; }
	inline std::string get_filepath()const { return filepath; }
	inline std::string get_full_filepath() const { return filepath + filename + "." + filetype; }

	inline bool is_ready() { return camera != nullptr && light != nullptr && core != nullptr; };

protected:
	std::string filepath, filename, filetype;

	bool flag_show_model = true;
	M_BlinnPhong material; //网格

	bool need_update_gl = true; //刷新顶点数据，刷新完自动被置false，防止重复刷新

	QColor lines_color = { 0,0,0 };
	bool flag_show_lines = true;
	M_Lines material_line; //线框

	QColor point_color = { 255,0,0 };
	bool flag_show_points = false;
	M_Points material_point; //顶点

	double normal_lines_length = 0.05; //法线长度

	QColor face_normal_lines_color = { 0,0,255 };
	bool flag_face_normal_lines = false;
	M_Lines material_face_normal_lines; //面法线

	QColor vertex_normal_lines_color = { 0,255,0 };
	bool flag_vertex_normal_lines = false;
	M_Lines material_vertex_normal_lines; //顶点法线

	double slice_rate_x = 0, slice_rate_y = 0, slice_rate_z = 0; //切片百分比
	bool slice_inverse_x = false, slice_inverse_y = false, slice_inverse_z = false; //是否反转切片方向

	Camera* camera = nullptr;
	Light* light = nullptr;
};
