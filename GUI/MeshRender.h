#pragma once


#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Camera.h"
#include "Light.h"
#include "A_Background.h"
#include "A_Trimesh.h"
#include "A_WorldCoordinate.h"
#include "MouseState.h"
#include "Trackball.h"
#include "SceneManger.h"
#include <QTimer> 

class MeshRender  : public QOpenGLWidget
{
	Q_OBJECT

public:
	MeshRender(QWidget *parent);
	~MeshRender();

private:
	void Init(void);// OpenGL初始化

protected:

	/* OpenGL绘制重写函数 */
	void initializeGL(void) override;
	void resizeGL(int _w, int _h) override;
	void paintGL(void) override;
	/* 设置绘制样式 */
	virtual void DrawScene();
	/* 鼠标键盘事件 */
	virtual void mousePressEvent(QMouseEvent*) override;
	virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void wheelEvent(QWheelEvent*) override;
	virtual void keyPressEvent(QKeyEvent*) override;
	virtual void keyReleaseEvent(QKeyEvent*) override;

public:
	bool need_update_shaders = false;
	QColor background_color = { 1,1,1 };
	A_Background background;
	A_WorldCoordinate world_coor;
	SceneManger* sceneManger = nullptr;

	Camera camera;
	Light light;
protected:
	uint window_width = 600, window_height = 480;
	MouseState mouse_state; //鼠标状态机
	uint last_key = Qt::Key_unknown; //上次按下的按键，如果要用组合按键请先完成类KeyState

	QOpenGLFunctions_3_3_Core* core;	// OpenGL核心态
	
	Traceball trackball; //轨迹球
	QVector2D last_clicked_2d_for_translate = { -1,-1 };
	
	QTimer* updategl_timer;
};
