#pragma once

#include <QWidget>
#include <QMenu>
#include "ui_SceneModelItem.h"
#include "A_Trimesh.h"
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
		TriMesh,
	} meshtype = MeshType::None;


	ActorModelBase* get_mesh() { return mesh; }
	A_Trimesh* get_trimesh() { return dynamic_cast<A_Trimesh*>(mesh); }

	void load_trimesh(const std::string& path); //加载三角表面网格
	void load_trimesh(A_Trimesh* trimesh); //加载三角表面网格

	void set_press_listener(std::function<void(SceneModelItem*)> listener) { press_listener = listener; };
	void set_delete_listener(std::function<void(SceneModelItem*)> listener) { delete_listener = listener; };

	bool is_focus = false; //当前是否被选中成为交点
public slots:
	void toggle_show_model(int value);
	void toggle_show_model_line(int value);
	void toggle_show_model_vertex(int value);
	void toggle_show_face_normal(int value);
	void toggle_show_vertex_normal(int value);

private:
	ActorModelBase* mesh = nullptr;
	std::function<void(SceneModelItem*)> press_listener;
	std::function<void(SceneModelItem*)> delete_listener;
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
