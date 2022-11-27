#pragma once

#include "ActorBase.h"
#include <QColor>

class ActorModelBase :
	public ActorBase {

public:
	ActorModelBase(){}
	virtual ~ActorModelBase(){}

	void init(QOpenGLFunctions_3_3_Core* core_) override;
	void init(QOpenGLFunctions_3_3_Core* core_, Camera* camera_ = nullptr, Light* light_ = nullptr); //请在opengl的环境下调用，否则没用

	virtual void load_model(const std::string& path) = 0;
	virtual void save_model(const std::string& path) = 0;

	void set_light(Light* light_) { light = light_; }
	void set_camera(Camera* camera_) { camera = camera_; }
	void set_normal_mode(Normal_Mode mode) { material.normal_mode = mode; }

	void show_model(){ flag_show_model = true;  need_update_gl = true; }
	void hide_model(){ flag_show_model = false;  need_update_gl = true; }
	void toggle_model() { flag_show_model = !flag_show_model;  need_update_gl = true; } //是否显示模型

	void show_lines() { flag_show_lines = true; need_update_gl = true; } //显示线框
	void hide_lines() { flag_show_lines = false; need_update_gl = true; } //隐藏线框
	void toggle_model_lines() { flag_show_lines = !flag_show_lines;  need_update_gl = true; } //切换线框显示模式

	void show_model_points() { flag_show_points = true; need_update_gl = true; } //显示顶点
	void hide_model_points() { flag_show_points = false; need_update_gl = true; } //隐藏顶点
	void toggle_model_points() { flag_show_points = !flag_show_points;  need_update_gl = true; } //顶点显示模式

	void show_face_normal_lines() { flag_face_normal_lines = true; need_update_gl = true; } //显示面法线
	void hide_face_normal_lines() { flag_face_normal_lines = false; need_update_gl = true; } //隐藏面法线
	void toggle_face_normal_lines() { flag_face_normal_lines = !flag_face_normal_lines;  need_update_gl = true; } //面法线显示模式

	void show_vertex_normal_lines() { flag_vertex_normal_lines = true; need_update_gl = true; } //显示顶点法线
	void hide_vertex_normal_lines() { flag_vertex_normal_lines = false; need_update_gl = true; } //隐藏顶点法线
	void toggle_vertex_normal_lines() { flag_vertex_normal_lines = !flag_vertex_normal_lines;  need_update_gl = true; } //顶点法线显示模式


	void set_lines_color(QColor color) { lines_color = color;  need_update_gl = true; } //线框颜色
	void set_points_color(QColor color) { point_color = color;  need_update_gl = true; } //顶点颜色
	void set_face_normal_lines_color(QColor color) { face_normal_lines_color = color;  need_update_gl = true; } //面法线颜色
	void set_vertex_normal_lines_color(QColor color) { vertex_normal_lines_color = color;  need_update_gl = true; } //顶点法线颜色

	std::string get_filename() const { return filename; }
	std::string get_filetype()const { return filetype; }
	std::string get_filepath()const { return filepath; }
	std::string get_full_filepath() const { return filepath + filename + "." + filetype; }

	bool is_ready() { return camera != nullptr && light != nullptr && core != nullptr; };

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

	Camera* camera = nullptr;
	Light* light = nullptr;
};
