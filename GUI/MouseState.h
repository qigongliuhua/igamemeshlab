#pragma once

#include <QWidget>

class MouseState
{
public:
	MouseState() {}
	~MouseState() {}

	enum class MouseStatus
	{
		No_Press,
		Only_Press_Left_Buttom,
		Only_Press_Right_Buttom,
		Only_Press_Mid_Buttom,
		Press_Left_Mid_Buttom,
		Press_Right_Mid_Buttom,
		Press_Left_Right_Buttom,
		Press_Left_Mid_Right_Buttom,
	} status = MouseStatus::No_Press;

	void press(Qt::MouseButton mb);
	void release(Qt::MouseButton mb);
	void print();

private:
	Qt::MouseButton last = Qt::MouseButton::NoButton;

};

