#pragma once

#include <QWidget>
#include <QMenu>
#include "ui_SceneModelItem.h"
#include "A_Trimesh.h"
#include "A_Quadmesh.h"
#include "A_Polygonmesh.h"
#include "A_Tetmesh.h"
#include "A_Hexmesh.h"
#include "A_Polyhedralmesh.h"
#include <functional>


class SceneModelItem : public QWidget
{
	Q_OBJECT

public:
	SceneModelItem(QWidget *parent = nullptr);
	~SceneModelItem();

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
public:
	enum MeshType
	{
		None,
		Trimesh,
		Quadmesh,
		Polygonmesh,
		Tetmesh,
		Hexmesh,
		Polyhedralmesh,
	} meshtype = MeshType::None;


	inline ActorModelBase* GetMesh() { return mesh; }
	inline A_Trimesh* GetTrimesh() { return dynamic_cast<A_Trimesh*>(mesh); }
	inline A_Quadmesh* GetQuadmesh() { return dynamic_cast<A_Quadmesh*>(mesh); }
	inline A_Polygonmesh* GetPolygonmesh() { return dynamic_cast<A_Polygonmesh*>(mesh); }
	inline A_Tetmesh* GetTetmesh() { return dynamic_cast<A_Tetmesh*>(mesh); }
	inline A_Hexmesh* GetHexmesh() { return dynamic_cast<A_Hexmesh*>(mesh); }
	inline A_Polyhedralmesh* GetPolyhedralmesh() { return dynamic_cast<A_Polyhedralmesh*>(mesh); }
	void ClearMesh();

	void LoadTrimesh(const std::string& path); //加载三角表面网格
	void LoadTrimesh(A_Trimesh* trimesh); //加载三角表面网格

	void LoadQuadmesh(const std::string& path); //加载四边形表面网格
	void LoadQuadmesh(A_Quadmesh* quadmesh); //加载四边形表面网格

	void LoadPolygonmesh(const std::string& path); //加载四边形表面网格
	void LoadPolygonmesh(A_Polygonmesh* polygonmesh); //加载四边形表面网格

	void LoadTetmesh(const std::string& path); //加载四面体网格
	void LoadTetmesh(A_Tetmesh* tetmesh); //加载四面体网格

	void LoadHexmesh(const std::string& path); //加载六面体网格
	void LoadHexmesh(A_Hexmesh* hexmesh); //加载六面体网格

	void LoadPolyhedralmesh(const std::string& path); //加载多面体网格
	void LoadPolyhedralmesh(A_Polyhedralmesh* polyhedralmesh); //加载多面体网格

	inline void AddPressListener(std::function<void(SceneModelItem*)> listener) { press_listener.push_back(listener); };
	inline void AddDeleteListener(std::function<void(SceneModelItem*)> listener) { delete_listener.push_back(listener); };

	bool is_focus = false; //当前是否被选中成为交点
public slots:
	void toggle_show_model(int value);
	void toggle_show_model_line(int value);
	void toggle_show_model_vertex(int value);
	void toggle_show_face_normal(int value);
	void toggle_show_vertex_normal(int value);

private:
	ActorModelBase* mesh = nullptr;
	std::vector<std::function<void(SceneModelItem*)>> press_listener;
	std::vector < std::function<void(SceneModelItem*)>> delete_listener;
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event)override;
	void mouseReleaseEvent(QMouseEvent* event)override;
	void paintEvent(QPaintEvent*)override;
	void enterEvent(QEvent* event)override;
	void leaveEvent(QEvent* event)override;


	//右键菜单
	QMenu* m_pMenuRD;
	QAction* m_pActionAdd;
	QAction* m_pActionDelete;
	QAction* m_pActionEdit;

	//测试菜单
	QMenu* m_pMenuTest;
	QAction* m_pActionTest1;
	QAction* m_pActionTest2;

	void CreateMenu();

private slots:
	void SlotMenuClicked(QAction*);

private:
	Ui::SceneModelItemClass ui;
};
