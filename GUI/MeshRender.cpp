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

	sceneManger = new SceneManger();

	light.light_pos = QVector3D(0, 0, 4 * camera.scene_radius);
	light.light_color = { 1,1,1 };

	world_coor.set_camera(&camera);

	updategl_timer = new QTimer(this);
	connect(updategl_timer, &QTimer::timeout, this, [&]() {
		update();
	});

	updategl_timer->setInterval(1000 / 30); //每秒30帧
	updategl_timer->start();
}

void MeshRender::initializeGL()
{
	// OpenGL state
	//initializeOpenGLFunctions ();

	core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

	core->glClearColor((GLfloat)0.3, (GLfloat)0.3, (GLfloat)0.3, (GLfloat)1.0);
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
	world_coor.init(core);
}

void MeshRender::paintGL()
{
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (need_update_shaders) {
		need_update_shaders = false;
		background.set_color(background_color.redF(), background_color.greenF(), background_color.blueF());
	}
	DrawScene();
}

void MeshRender::DrawScene()
{
	background.render();
	for (auto item : sceneManger->items) {
		if (!item->get_mesh()->is_ready()) item->get_mesh()->init(core, &camera, &light);
		item->get_mesh()->render();
	}
	world_coor.render();

	//释放需要删除的
	for (auto item : sceneManger->need_delete_items) {
		item->~SceneModelItem();
	}
	sceneManger->need_delete_items.clear();
}



void MeshRender::mousePressEvent(QMouseEvent* _event)
{
	mouse_state.press(_event->button());
	update();
}

void MeshRender::mouseMoveEvent(QMouseEvent* _event)
{
	//std::cout << _event->pos().x() << " " << _event->pos().y() << std::endl;
	if (mouse_state.status == MouseState::MouseStatus::Only_Press_Left_Buttom) { //旋转模型
		QVector2D pos2d(_event->pos().x(), _event->pos().y());
		QVector3D axis; double angle; bool valid;
		trackball.trackball_to_rotations(pos2d, axis, angle, valid);
		if (valid) camera.rotate(angle, axis);

		world_coor.show();
	}
	else if (mouse_state.status == MouseState::MouseStatus::Only_Press_Right_Buttom) { //平移模型
		QVector2D pos2d(_event->pos().x(), _event->pos().y());
		if (last_clicked_2d_for_translate.x() >=0 && last_clicked_2d_for_translate.y() >= 0) {
			QVector2D delta = last_clicked_2d_for_translate - pos2d;
			delta.normalize();
			delta *= camera.scene_radius * 0.015;
			camera.translate(QVector3D(-delta.x(), delta.y(), 0));
		}
		last_clicked_2d_for_translate = pos2d;

		world_coor.show();
	}

	update();
}

void MeshRender::mouseReleaseEvent(QMouseEvent* _event)
{
	mouse_state.release(_event->button());

	if (mouse_state.status != MouseState::MouseStatus::Only_Press_Left_Buttom) {
		trackball.reset_trace(); //停止旋转模型
	}
	if (mouse_state.status != MouseState::MouseStatus::Only_Press_Left_Buttom && 
		mouse_state.status != MouseState::MouseStatus::Only_Press_Right_Buttom) {
		world_coor.hide();
	}

	update();
}

void MeshRender::wheelEvent(QWheelEvent* _event)
{
	double d = -_event->delta() / 120.0 * 0.1 * camera.scene_radius;
	camera.zoom(d);
	update();
}

void MeshRender::keyPressEvent(QKeyEvent* _event)
{
	// Qt::Key_Control == _event->key()
	last_key = _event->key();
	_event->ignore();

	if (last_key == Qt::Key_Space) {
		camera.reset();
	}
	else if (last_key == Qt::Key_1) {
		camera.reset();
		camera.rotate_y(90);
	}
	else if (last_key == Qt::Key_2) {
		camera.reset();
		camera.rotate_y(180);
	}
	else if (last_key == Qt::Key_3) {
		camera.reset();
		camera.rotate_y(270);
	}
	else if (last_key == Qt::Key_4) {
		camera.reset();
		camera.rotate_x(90);
	}
	else if (last_key == Qt::Key_5) {
		camera.reset();
		camera.rotate_z(90);
	}
	update();
}

void MeshRender::keyReleaseEvent(QKeyEvent* _event)
{
	// Qt::Key_Control == _event->key()
	if (last_key == _event->key()) last_key = Qt::Key_unknown;
	_event->ignore();

	update();
}

void MeshRender::resizeGL(int _w, int _h)
{
	window_width = _w; window_height = _h;
	camera.width = _w; camera.height = _h;
	camera.reset_projection();

	trackball.width = _w; trackball.height = _h;
	update();
}
