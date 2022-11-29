#include "SceneManger.h"

void SceneManger::Add(SceneModelItem* item)
{
	assert(container != nullptr);
	std::function<void(SceneModelItem*)> listener = [&](SceneModelItem* item) {
		if (last_clicked_item) last_clicked_item->is_focus = false;
		last_clicked_item = item;
		last_clicked_item->is_focus = true;

		for (auto it : items) {
			it->update();
		}

		for (auto listener : change_focus_item_listener) {
			listener(last_clicked_item);
		}
	};
	std::function<void(SceneModelItem*)> delete_listener = [&](SceneModelItem* item) {
		//int index = std::find(items.begin(), items.end(), item) - items.begin();
		//if(index < this->count())
		//	need_delete_items.push_back(item);
		this->Remove(item);
		item->ClearMesh();
	};
	item->AddPressListener(listener);
	item->AddDeleteListener(delete_listener);
	items.push_back(item);
	container->insertWidget(container->count() - 1, item); //加到最后
	if (last_clicked_item == nullptr)
	{
		last_clicked_item = item;
		last_clicked_item->is_focus = true;
		last_clicked_item->update();
		for (auto listener : change_focus_item_listener) {
			listener(last_clicked_item);
		}
	}
}

void SceneManger::RemoveAt(int index)
{
	if (index >= Size()) return;
	SceneModelItem* item = items.at(index);
	container->removeWidget(item);
	items.erase(items.begin() + index);
	need_delete_items.push_back(item);

	if (Size() == 0) last_clicked_item = nullptr;
}

void SceneManger::Remove(SceneModelItem* item)
{
	int index = std::find(items.begin(), items.end(), item) - items.begin();
	RemoveAt(index);
}
