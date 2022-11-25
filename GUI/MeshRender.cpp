#include "MeshRender.h"
#include <iostream>
#include <QApplication>
#include <QMouseEvent>
#include <QtWidgets/QDesktopWidget>
#include "IO.h"
#include "ShaderUtils.h"


const double MeshRender::trackballradius = 0.6;

MeshRender::MeshRender(QWidget*parent)
	: QOpenGLWidget(parent)
{
	Init();
	//QSurfaceFormat surfaceFormat;
	//surfaceFormat.setSamples(16);//多重采样
	//setFormat(surfaceFormat); //setFormat是QOpenGLWidget的函数

	MeshKernel::IO io;
	int side_num;
	mesh = io.ReadObjFile("D:\\Projects\\test\\iGameMeshLab\\Model\\bunny.obj", side_num);
	mesh.genAllVerticesNormal();

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
}

QSize MeshRender::minimumSizeHint(void) const
{
	return QSize(10, 10);
}

QSize MeshRender::sizeHint(void) const
{
	QRect rect = QApplication::desktop()->screenGeometry();
	return QSize(int(rect.width() * 0.8), int(rect.height() * 1.0));
}

const double& MeshRender::Radius(void) const
{
	return radius;
}

const Eigen::Vector3d& MeshRender::Center(void) const
{
	return center;
}

const QMatrix4x4* MeshRender::GetModelviewMatrix1(void) const
{
	return &viewMatrix;
}

void MeshRender::ResetModelviewMatrix(void)
{
	viewMatrix.setToIdentity();
}

void MeshRender::CopyModelViewMatrix(void)
{
}

void MeshRender::LoadCopyModelViewMatrix(void)
{
}

const QMatrix4x4* MeshRender::GetProjectionMatrix1(void) const
{
	return &projectionMatrix;
}

void MeshRender::SetProjectionMode(const ProjectionMode& pm)
{
	projectionMode = pm;
	UpdateProjectionMatrix();
	ViewAll();
}

const MeshRender::ProjectionMode& MeshRender::GetProjectionMode(void) const
{
	return projectionMode;
}


void MeshRender::SetDrawMode(const DrawMode& dm)
{
	drawMode = dm;
	update();
}

void MeshRender::SetColorMode(const ColorMode& rm)
{
	colorMode = rm;
	update();
}

const MeshRender::DrawMode& MeshRender::GetDrawMode(void) const
{
	// TODO: 在此处插入 return 语句
	return drawMode;
}

const MeshRender::ColorMode& MeshRender::GetColorMode(void) const
{
	// TODO: 在此处插入 return 语句
	return colorMode;
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

	viewMatrix.setToIdentity();
	SetScenePosition(Eigen::Vector3d(0.0, 0.0, 0.0), 1.0);


	material.set_opengl(core);
	// 加载shader
	material.load_shaders();

	// 渐变背景，其实是一个长方形面片，绘制在最底下
	const std::vector<GLfloat> back_ground_vertices{
		// 顶点位置        
		1.0f, 1.0f, 0.0f,   // 右上角
		-1.0f, -1.0f, 0.0f, // 左下角
		1.0f, -1.0f, 0.0f,  // 右下角
		-1.0f, 1.0f, 0.0f,   // 左上角
		-1.0f, -1.0f, 0.0f, // 左下角
		1.0f, 1.0f, 0.0f,   // 右上角
	};

	material.set_shaders(back_ground_vertices, 1.0, 1.0, 1.0);

	//std::cout << "qwe" << std::endl;


	std::vector<GLfloat> vertex;
	mesh.genAllVerticesNormal();
	for (auto fp : mesh.allfaces()) {
		auto f = fp.second;
		for (int i = 0; i < 3; ++i) {
			MeshKernel::iGameVertex v = mesh.vertices(f.vh(i));

			vertex.push_back(v.x());
			vertex.push_back(v.y());
			vertex.push_back(v.z());
			vertex.push_back(v.getNormalX());
			vertex.push_back(v.getNormalY());
			vertex.push_back(v.getNormalZ());
			vertex.push_back(0.8);
			vertex.push_back(0.8);
			vertex.push_back(0.8);
		}
	}

	material2.set_opengl(core);

	material2.load_shaders();

	material2.set_shaders(vertex);
}

void MeshRender::paintGL()
{
	core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawScene();
}

void MeshRender::DrawScene()
{
	DrawBackGround();

	Camera cam(width(), height());
	cam.print();
	Light light;
	light.light_pos = -QVector3D(0,0, 4*cam.scene_radius);
	light.light_color = { 1,1,1 };
	material2.render(cam, light);
}

void MeshRender::DrawBackGround(void) {
	material.render();
	
}


void MeshRender::mousePressEvent(QMouseEvent* _event)
{
	lastpoint2 = _event->pos();
	lastpointok = MapToSphere(lastpoint2, lastpoint3);
	mouseMode = _event->button();
}

void MeshRender::mouseMoveEvent(QMouseEvent* _event)
{
	QPoint newPoint2D = _event->pos();
	if (lastpointok) {
		switch (mouseMode) {
		case Qt::LeftButton:
			Rotation(newPoint2D);
			break;
		case Qt::RightButton:
			Translation(newPoint2D);
			break;
		default:
			break;
		}
	} // end of if

	// remember this point
	lastpoint2 = newPoint2D;
	lastpointok = MapToSphere(lastpoint2, lastpoint3);
	// trigger redraw
	update();
}

void MeshRender::mouseReleaseEvent(QMouseEvent* _event)
{
	mouseMode = Qt::NoButton;
	lastpointok = false;
}

void MeshRender::wheelEvent(QWheelEvent* _event)
{
	double d = -_event->delta() / 120.0 * 0.1 * radius;
	Translate(Eigen::Vector3d(0.0, 0.0, d));
	update();
}

void MeshRender::keyPressEvent(QKeyEvent* _event)
{
	switch (_event->key()) {
	case Qt::Key_Escape:
		qApp->quit();
		break;
	}
	_event->ignore();
}

void MeshRender::keyReleaseEvent(QKeyEvent* _event)
{
	_event->ignore();
}

void MeshRender::Translation(const QPoint& p)
{
	double z = -(viewMatrix(2, 3) / viewMatrix(2, 2));		// 右键拖动的距离与模型离视角的远近进行绑定（使拖动更流畅）

	double w = width(); double h = height();
	double aspect = w / h;
	double near_plane = 0.01 * radius;
	double top = tan(45.0 / 2.0f * M_PI / 180.0f) * near_plane;
	double right = aspect * top;

	double dx = p.x() - lastpoint2.x();
	double dy = p.y() - lastpoint2.y();

	Translate(Eigen::Vector3d(2.0 * dx / w * right / near_plane * z,
		-2.0 * dy / h * top / near_plane * z,
		0.0f));
}

void MeshRender::Rotation(const QPoint& p)
{
	Eigen::Vector3d  newPoint3D;
	bool newPoint_hitSphere = MapToSphere(p, newPoint3D);
	if (newPoint_hitSphere) {
		Eigen::Vector3d axis = lastpoint3.cross(newPoint3D);// corss product
		if (axis.norm() < 1e-7) {
			axis = Eigen::Vector3d(1, 0, 0);
		}
		else {
			axis.normalize();
		}
		// find the amount of rotation
		Eigen::Vector3d d = lastpoint3 - newPoint3D;
		double t = 0.5 * d.norm() / trackballradius;
		if (t < -1.0) t = -1.0;
		else if (t > 1.0) t = 1.0;
		double phi = 2.0 * asin(t);
		double  angle = phi * 180.0 / M_PI;
		Rotate(axis, angle);
	}
}

bool MeshRender::MapToSphere(const QPoint& _v2D, Eigen::Vector3d& _v3D)
{
	// This is actually doing the Sphere/Hyperbolic sheet hybrid thing,
	// based on Ken Shoemake's ArcBall in Graphics Gems IV, 1993.
	double x = (2.0 * _v2D.x() - width()) / width();
	double y = -(2.0 * _v2D.y() - height()) / height();
	double xval = x;
	double yval = y;
	double x2y2 = xval * xval + yval * yval;

	const double rsqr = trackballradius * trackballradius;
	_v3D[0] = xval;
	_v3D[1] = yval;
	if (x2y2 < 0.5 * rsqr) {
		_v3D[2] = sqrt(rsqr - x2y2);
	}
	else {
		_v3D[2] = 0.5 * rsqr / sqrt(x2y2);
	}

	return true;
}


void MeshRender::resizeGL(int _w, int _h)
{
	core->glViewport(0, 0, _w, _h);
	UpdateProjectionMatrix();
	update();
}

void MeshRender::SetScenePosition(const Eigen::Vector3d& _center, const double& _radius)
{
	center = _center;
	radius = _radius;
	viewMatrix.setToIdentity();
	UpdateProjectionMatrix();
	ViewAll();
}

void MeshRender::ViewAll(void)
{
	viewMatrix.translate(-center[0], -center[1], -(center[2] + 2.0 * radius));
}

void MeshRender::UpdateProjectionMatrix(void)
{
	projectionMatrix.setToIdentity();

	if (PERSPECTIVE == projectionMode) {
		projectionMatrix.frustum(-0.01 * radius * (sqrt(2.0) - 1) * width() / height(),
			0.01 * radius * (sqrt(2.0) - 1) * width() / height(),
			-0.01 * radius * (sqrt(2.0) - 1),
			0.01 * radius * (sqrt(2.0) - 1),
			0.01 * radius,
			100.0 * radius);
	}
	else if (ORTHOGRAPHIC == projectionMode) //not work for
	{
		projectionMatrix.ortho(windowleft, windowright, windowbottom, windowtop, -1, 1);
	}
}

void MeshRender::ViewRotate(const QPoint& p)
{
	Eigen::Vector3d  newPoint3D;
	bool newPoint_hitSphere = MapToSphere(p, newPoint3D);
	if (newPoint_hitSphere) {
		Eigen::Vector3d axis = lastpoint3.cross(newPoint3D);// corss product
		if (axis.norm() < 1e-7) {
			axis = Eigen::Vector3d(1, 0, 0);
		}
		else {
			axis.normalize();
		}
		// find the amount of rotation
		Eigen::Vector3d d = lastpoint3 - newPoint3D;
		double t = 0.5 * d.norm() / trackballradius;
		if (t < -1.0) t = -1.0;
		else if (t > 1.0) t = 1.0;
		double phi = 2.0 * asin(t);
		double  angle = phi * 180.0 / M_PI;

		QMatrix4x4 tempmatrix;
		//tempmatrix.translate(0, 0, -2.0 * radius);	// 由于许多模型的初始center并不在坐标原点，需要把center先移到坐标原点再旋转
		tempmatrix.rotate(angle, axis[0], axis[1], axis[2]);
		//viewRotateMatrix.rotate(angle, axis[0], axis[1], axis[2]);
		//tempmatrix.translate(0, 0, 2.0 * radius);
		//viewMatrix = tempmatrix * viewMatrix;		// 左乘的方式，使动态欧拉角改成静态欧拉角，解决方向旋转的问题
		viewRotateMatrix = tempmatrix * viewRotateMatrix;
	}
}

void MeshRender::Translate(const Eigen::Vector3d& trans)
{
}

void MeshRender::Rotate(const Eigen::Vector3d& axis, const double& angle)
{
}

void MeshRender::ModelScale(const Eigen::Vector3d& trans)
{
}

void MeshRender::PickItemTranslation(const QPoint& p)
{
}
