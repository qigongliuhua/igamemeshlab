#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_iGameMeshLab.h"
#include "SceneManger.h"


class iGameMeshLab : public QMainWindow
{
    Q_OBJECT


public:
    iGameMeshLab(QWidget *parent = nullptr);
    ~iGameMeshLab();


	SceneManger* sceneManger;

public slots:
	void change_background_color();
	void change_model_line_color();
	void change_model_point_color();
	void reset_all_color();
	void change_camera_fov(int value);
	void change_camera_persp_mode(int value);
	void reset_camera();
	void set_normalmode_flat();
	void set_normalmode_smooth();
	void toggle_world_coor();

	void slice_x_inverse_slot(int value); //沿x轴切片反转
	void slice_y_inverse_slot(int value); //沿y轴切片反转
	void slice_z_inverse_slot(int value); //沿z轴切片反转
	void slice_x_rate_slot(int value); //沿x轴切片百分比
	void slice_y_rate_slot(int value); //沿y轴切片百分比
	void slice_z_rate_slot(int value); //沿z轴切片百分比


	void MenuClicked(QAction* action);
private:
    Ui::iGameMeshLab ui;
};
