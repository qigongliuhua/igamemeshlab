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
	void change_camera_fov(int value);
	void change_camera_persp_mode(int value);
	void reset_camera();
	void set_normalmode_flat();
	void set_normalmode_smooth();
	void toggle_world_coor();

	void MenuClicked(QAction* action);
private:
    Ui::iGameMeshLab ui;
};
