#pragma once

#define _USE_MATH_DEFINES

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Camera.h"
#include "Materials.h"
#include "Light.h"
#include "A_Background.h"


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

	void DrawBackGround(void);
	/* 鼠标键盘事件 */
	virtual void mousePressEvent(QMouseEvent*) override;
	virtual void mouseMoveEvent(QMouseEvent*) override;
	virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void wheelEvent(QWheelEvent*) override;
	virtual void keyPressEvent(QKeyEvent*) override;
	virtual void keyReleaseEvent(QKeyEvent*) override;

protected:

	/* 鼠标按压模式 */
	Qt::MouseButton mouseMode;


	QOpenGLFunctions_3_3_Core* core;	// OpenGL核心态
	

	A_Background background;

	Camera camera;
	Light light;
	

};
