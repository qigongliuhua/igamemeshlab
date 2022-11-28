#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVBoxLayout>
#include <QStringListModel>
#include "SceneModelItem.h"
#include "Camera.h"
#include "Light.h"

class SceneManger
{
public:
	SceneManger(QVBoxLayout* list_widget_) :list_widget(list_widget_) {}
	SceneManger() {}
	~SceneManger() {}

	void set_list_widget(QVBoxLayout* list_widget_) { list_widget = list_widget_; }

	void add_item(SceneModelItem* item);
	void remove_at(int index);
	void remove_item(SceneModelItem* item);
	int count() { return (int)items.size(); }

	QVBoxLayout* list_widget = nullptr;
	std::vector<SceneModelItem* >items; 
	std::vector<SceneModelItem* > need_delete_items; //稍后再opengl的环境下释放

	SceneModelItem* last_clicked_item = nullptr; //上一次选中的item
private:
};

