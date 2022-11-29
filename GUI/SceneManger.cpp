#include "SceneManger.h"

void SceneManger::add_item(SceneModelItem* item)
{
	assert(list_widget != nullptr);
	std::function<void(SceneModelItem*)> listener = [&](SceneModelItem* item) {
		if (last_clicked_item) last_clicked_item->is_focus = false;
		last_clicked_item = item;
		last_clicked_item->is_focus = true;

		for (auto it : items) {
			it->update();
		}

		change_focus_item_listener(last_clicked_item);
	};
	std::function<void(SceneModelItem*)> delete_listener = [&](SceneModelItem* item) {
		//int index = std::find(items.begin(), items.end(), item) - items.begin();
		//if(index < this->count())
		//	need_delete_items.push_back(item);
		last_clicked_item = nullptr;
		this->remove_item(item);
		item->clear_mesh();
	};
	item->set_press_listener(listener);
	item->set_delete_listener(delete_listener);
	items.push_back(item);
	list_widget->insertWidget(list_widget->count() - 1, item); //加到最后
	if (last_clicked_item == nullptr)
	{
		last_clicked_item = item;
		last_clicked_item->is_focus = true;
		last_clicked_item->update();
		change_focus_item_listener(last_clicked_item);
	}
}

void SceneManger::remove_at(int index)
{
	if (index >= this->count()) return;
	SceneModelItem* item = items.at(index);
	list_widget->removeWidget(item);
	items.erase(items.begin() + index);
	need_delete_items.push_back(item);

	if (this->count() == 0) last_clicked_item = nullptr;
}

void SceneManger::remove_item(SceneModelItem* item)
{
	int index = std::find(items.begin(), items.end(), item) - items.begin();
	remove_at(index);
}
