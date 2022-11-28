#include "SceneModelItem.h"
#include <QPainter>
#include <QMouseEvent>
#include <iostream>

SceneModelItem::SceneModelItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CreateMenu();
	
	connect(ui.flag_show_model, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model(int)));
	connect(ui.flag_show_model_lines, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model_line(int)));
	connect(ui.flag_show_model_vertex, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model_vertex(int)));
	connect(ui.flag_show_face_normal, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_face_normal(int)));
	connect(ui.flag_show_vertex_normal, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_vertex_normal(int)));
}

SceneModelItem::~SceneModelItem()
{
	//mesh->~ActorModelBase(); //关闭窗口时因为opengl上下文被销毁所以释放VBO VAO会报错，暂时不会修改
}

void SceneModelItem::clear_mesh()
{
	switch (meshtype)
	{
	case SceneModelItem::None:
		break;
	case SceneModelItem::TriMesh:
		get_trimesh()->~A_Trimesh();
		break;
	case SceneModelItem::TetMesh:
		get_tetmesh()->~A_Tetmesh();
		break;
	case SceneModelItem::HexMesh:
		get_hexmesh()->~A_Hexmesh();
		break;
	default:
		break;
	}
	mesh = nullptr;
	meshtype = SceneModelItem::None;
}

void SceneModelItem::load_trimesh(const std::string& path)
{
	A_Trimesh* trimesh = new A_Trimesh();
	trimesh->load_model(path.c_str());
	mesh = dynamic_cast<ActorModelBase*>(trimesh);
	meshtype = MeshType::TriMesh;

	ui.model_name->setText(tr(trimesh->get_filename().c_str()));
}

void SceneModelItem::load_trimesh(A_Trimesh* trimesh)
{
	mesh = dynamic_cast<ActorModelBase*>(trimesh);
	meshtype = MeshType::TriMesh;

	ui.model_name->setText(tr(trimesh->get_filename().c_str()));
}

void SceneModelItem::load_tetmesh(const std::string& path)
{
	A_Tetmesh* tetmesh = new A_Tetmesh();
	tetmesh->load_model(path.c_str());
	mesh = dynamic_cast<ActorModelBase*>(tetmesh);
	meshtype = MeshType::TetMesh;

	ui.model_name->setText(tr(tetmesh->get_filename().c_str()));
}

void SceneModelItem::load_tetmesh(A_Tetmesh* tetmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(tetmesh);
	meshtype = MeshType::TetMesh;

	ui.model_name->setText(tr(tetmesh->get_filename().c_str()));
}

void SceneModelItem::load_hexmesh(const std::string& path)
{
	A_Hexmesh* hexmesh = new A_Hexmesh();
	hexmesh->load_model(path.c_str());
	mesh = dynamic_cast<ActorModelBase*>(hexmesh);
	meshtype = MeshType::HexMesh;

	ui.model_name->setText(tr(hexmesh->get_filename().c_str()));
}

void SceneModelItem::load_hexmesh(A_Hexmesh* hexmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(hexmesh);
	meshtype = MeshType::HexMesh;

	ui.model_name->setText(tr(hexmesh->get_filename().c_str()));
}

void SceneModelItem::toggle_show_model(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->show_model();
	else mesh->hide_model();
}

void SceneModelItem::toggle_show_model_line(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->show_lines();
	else mesh->hide_lines();
}

void SceneModelItem::toggle_show_model_vertex(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->show_model_points();
	else mesh->hide_model_points();
}

void SceneModelItem::toggle_show_face_normal(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->show_face_normal_lines();
	else mesh->hide_face_normal_lines();
}

void SceneModelItem::toggle_show_vertex_normal(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->show_vertex_normal_lines();
	else mesh->hide_vertex_normal_lines();
}

void SceneModelItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		m_pMenuRD->exec(QCursor::pos());
	}

	press_listener(this);
}

void SceneModelItem::mouseMoveEvent(QMouseEvent* event)
{
}

void SceneModelItem::mouseReleaseEvent(QMouseEvent* event)
{
}

void SceneModelItem::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setBrush(QColor(9, 151, 247, 1));//painter区域全部的背景色
	painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(QPoint(0, 0), QPoint(width(), 0));
	painter.drawLine(QPoint(0, height()), QPoint(width(), height()));

	if (is_focus) {
		painter.setBrush(QColor(9, 151, 247, 1));//painter区域全部的背景色
		painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawRect(rect());
	}

}

void SceneModelItem::enterEvent(QEvent* event)
{
	setPalette(QPalette(Qt::white));
	setAutoFillBackground(true);
}

void SceneModelItem::leaveEvent(QEvent* event)
{
	setPalette(QPalette(QColor(9, 151, 247, 1)));
	setAutoFillBackground(true);
}

void SceneModelItem::CreateMenu()
{
	m_pMenuRD = new QMenu(this);
	m_pActionAdd = m_pMenuRD->addAction(tr("Add"));
	m_pActionDelete = m_pMenuRD->addAction(tr("Delete"));
	m_pActionEdit = m_pMenuRD->addAction(tr("Edit"));
	m_pMenuRD->addSeparator();

	m_pMenuTest = new QMenu(this);
	m_pMenuTest->setTitle(tr("Test"));
	m_pActionTest1 = m_pMenuTest->addAction(tr("Test1"));
	m_pActionTest2 = m_pMenuTest->addAction(tr("Test2"));
	m_pMenuRD->addMenu(m_pMenuTest);

	connect(m_pMenuRD, SIGNAL(triggered(QAction*)), this, SLOT(SlotMenuClicked(QAction*)));
}

void SceneModelItem::SlotMenuClicked(QAction* action)
{
	if (action == m_pActionAdd)
	{
		std::cout << "click add" << std::endl;
	}
	else if (action == m_pActionDelete)
	{
		delete_listener(this);
	}
	else if (action == m_pActionTest1)
	{
		std::cout << "click test1" << std::endl;
	}
}
