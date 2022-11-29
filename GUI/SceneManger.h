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
	SceneManger(QVBoxLayout* list_widget_) :container(list_widget_) {}
	SceneManger() {}
	~SceneManger() {}

	void SetContainer(QVBoxLayout* container_) { container = container_; }
	inline void AddChangeFocusListener(std::function<void(SceneModelItem*)> listener) { change_focus_item_listener.push_back(listener); }; //焦点item变换时触发

	void Add(SceneModelItem* item);
	void RemoveAt(int index);
	void Remove(SceneModelItem* item);
	int Size() { return (int)items.size(); }

	QVBoxLayout* container = nullptr;
	std::vector<SceneModelItem* >items; 
	std::vector<SceneModelItem* > need_delete_items; //稍后再opengl的环境下释放
	SceneModelItem* last_clicked_item = nullptr; //上一次选中的item

private:
	std::vector<std::function<void(SceneModelItem*)>> change_focus_item_listener;
};

