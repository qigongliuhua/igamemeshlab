#include "SceneModelItem.h"
#include <QPainter>
#include <QMouseEvent>
#include <iostream>

SceneModelItem::SceneModelItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CreateMenu();
	
	connect(ui.flag_show_mesh, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model(int)));
	connect(ui.flag_show_mesh_edge, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model_line(int)));
	connect(ui.flag_show_mesh_vertex, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_model_vertex(int)));
	connect(ui.flag_show_mesh_face_normal, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_face_normal(int)));
	connect(ui.flag_show_mesh_vertex_normal, SIGNAL(stateChanged(int)), this, SLOT(toggle_show_vertex_normal(int)));
}

SceneModelItem::~SceneModelItem()
{
	//mesh->~ActorModelBase(); //关闭窗口时因为opengl上下文被销毁所以释放VBO VAO会报错，暂时不会修改
}

void SceneModelItem::ClearMesh()
{
	switch (meshtype)
	{
	case SceneModelItem::None:
		break;
	case SceneModelItem::Trimesh:
		GetTrimesh()->~A_Trimesh();
		break;
	case SceneModelItem::Quadmesh:
		GetQuadmesh()->~A_Quadmesh();
		break;
	case SceneModelItem::Polygonmesh:
		GetPolygonmesh()->~A_Polygonmesh();
		break;
	case SceneModelItem::Tetmesh:
		GetTetmesh()->~A_Tetmesh();
		break;
	case SceneModelItem::Hexmesh:
		GetHexmesh()->~A_Hexmesh();
		break;
	case SceneModelItem::Polyhedralmesh:
		GetPolyhedralmesh()->~A_Polyhedralmesh();
		break;
	default:
		break;
	}
	mesh = nullptr;
	meshtype = SceneModelItem::None;
}

void SceneModelItem::LoadTrimesh(const std::string& path)
{
	A_Trimesh* trimesh = new A_Trimesh();
	trimesh->LoadMesh(path.c_str());
	LoadTrimesh(trimesh);
}

void SceneModelItem::LoadTrimesh(A_Trimesh* trimesh)
{
	mesh = dynamic_cast<ActorModelBase*>(trimesh);
	meshtype = MeshType::Trimesh;

	ui.model_name->setText(tr(trimesh->GetMeshFileName().c_str()));
}

void SceneModelItem::LoadQuadmesh(const std::string& path)
{
	A_Quadmesh* quadmesh = new A_Quadmesh();
	quadmesh->LoadMesh(path.c_str());
	LoadQuadmesh(quadmesh);
}

void SceneModelItem::LoadQuadmesh(A_Quadmesh* quadmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(quadmesh);
	meshtype = MeshType::Quadmesh;

	ui.model_name->setText(tr(quadmesh->GetMeshFileName().c_str()));
}

void SceneModelItem::LoadPolygonmesh(const std::string& path)
{
	A_Polygonmesh* polygonmesh = new A_Polygonmesh();
	polygonmesh->LoadMesh(path.c_str());
	LoadPolygonmesh(polygonmesh);
}

void SceneModelItem::LoadPolygonmesh(A_Polygonmesh* polygonmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(polygonmesh);
	meshtype = MeshType::Polygonmesh;

	ui.model_name->setText(tr(polygonmesh->GetMeshFileName().c_str()));
}

void SceneModelItem::LoadTetmesh(const std::string& path)
{
	A_Tetmesh* tetmesh = new A_Tetmesh();
	tetmesh->LoadMesh(path.c_str());
	LoadTetmesh(tetmesh);
}

void SceneModelItem::LoadTetmesh(A_Tetmesh* tetmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(tetmesh);
	meshtype = MeshType::Tetmesh;

	ui.model_name->setText(tr(tetmesh->GetMeshFileName().c_str()));
}

void SceneModelItem::LoadHexmesh(const std::string& path)
{
	A_Hexmesh* hexmesh = new A_Hexmesh();
	hexmesh->LoadMesh(path.c_str());
	LoadHexmesh(hexmesh);
}

void SceneModelItem::LoadHexmesh(A_Hexmesh* hexmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(hexmesh);
	meshtype = MeshType::Hexmesh;

	ui.model_name->setText(tr(hexmesh->GetMeshFileName().c_str()));
}

void SceneModelItem::LoadPolyhedralmesh(const std::string& path)
{
	A_Polyhedralmesh* polyhedralmesh = new A_Polyhedralmesh();
	polyhedralmesh->LoadMesh(path.c_str());
	LoadPolyhedralmesh(polyhedralmesh);
}

void SceneModelItem::LoadPolyhedralmesh(A_Polyhedralmesh* polyhedralmesh)
{
	mesh = dynamic_cast<ActorModelBase*>(polyhedralmesh);
	meshtype = MeshType::Polyhedralmesh;

	ui.model_name->setText(tr(polyhedralmesh->GetMeshFileName().c_str()));
}

void SceneModelItem::toggle_show_model(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->ShowMesh();
	else mesh->HideMesh();
}

void SceneModelItem::toggle_show_model_line(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->ShowMeshEdge();
	else mesh->HideMeshEdge();
}

void SceneModelItem::toggle_show_model_vertex(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->ShowMeshVertex();
	else mesh->HideMeshVertex();
}

void SceneModelItem::toggle_show_face_normal(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->ShowMeshFaceNormal();
	else mesh->HideMeshFaceNormal();
}

void SceneModelItem::toggle_show_vertex_normal(int value)
{
	assert(mesh != nullptr);
	if (value) mesh->ShowMeshVertexNormal();
	else mesh->HideMeshVertexNormal();
}

void SceneModelItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		m_pMenuRD->exec(QCursor::pos());
	}

	for (auto listener : press_listener) {
		listener(this);
	}
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
		for (auto listener : delete_listener) {
			listener(this);
		}
	}
	else if (action == m_pActionTest1)
	{
		std::cout << "click test1" << std::endl;
	}
}
