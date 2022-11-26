#include "MeshRender.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QtWidgets/QDesktopWidget>
#include "ShaderUtils.h"



MeshRender::MeshRender(QWidget*parent)
	: QOpenGLWidget(parent)
{
	Init();
	//QSurfaceFormat surfaceFormat;
	//surfaceFormat.setSamples(16);//多重采样
	//setFormat(surfaceFormat); //setFormat是QOpenGLWidget的函数
}

MeshRender::~MeshRender()
{}

void MeshRender::Init(void)
{
	// qt stuff
	setAttribute(Qt::WA_NoSystemBackground, true);
	setFocusPolicy(Qt::StrongFocus);//选中窗口变色
	//setAcceptDrops(true); //拖拽功能
	//SetProjectionMode (projectionMode);

	light.light_pos = QVector3D(0, 0, 4 * camera.scene_radius);
	light.light_color = { 1,1,1 };
}

void MeshRender::initializeGL()
{
	// OpenGL state
	//initializeOpenGLFunctions ();

	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	core->glClearColor(0.3, 0.3, 0.3, 1.0);
	core->glClearDepth(1.0);
	core->glEnable(GL_DITHER);
	core->glEnable(GL_DEPTH_TEST);//开启深度测试
	//开启抗锯齿
	core->glEnable(GL_MULTISAMPLE);
	core->glEnable(GL_POINT_SMOOTH);
	core->glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);//GL_NICEST图形显示质量优先   GL_FASTEST 速度优先  GL_DONT_CARE系统默认
	core->glEnable(GL_BLEND);
	core->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	core->glEnable(GL_MULTISAMPLE);
	core->glEnable(GL_LINE_SMOOTH);
	core->glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	background.init(core);

}

void MeshRender::paintGL()
{
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawScene();
}

void MeshRender::DrawScene()
{
	DrawBackGround();
}

void MeshRender::DrawBackGround(void) {
	background.render();
}


void MeshRender::mousePressEvent(QMouseEvent* _event)
{

}

void MeshRender::mouseMoveEvent(QMouseEvent* _event)
{
	
}

void MeshRender::mouseReleaseEvent(QMouseEvent* _event)
{

}

void MeshRender::wheelEvent(QWheelEvent* _event)
{

}

void MeshRender::keyPressEvent(QKeyEvent* _event)
{

}

void MeshRender::keyReleaseEvent(QKeyEvent* _event)
{
	_event->ignore();
}

void MeshRender::resizeGL(int _w, int _h)
{
	camera.width = _w;
	camera.height = _h;
	update();
}
