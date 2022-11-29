
#include "MouseState.h"
#include <iostream>

void MouseState::PressEvent(Qt::MouseButton mb)
{
	switch (mb)
	{
	case Qt::LeftButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			status = MouseStatus::Only_Press_Left_Buttom;
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			status = MouseStatus::Press_Left_Right_Buttom;
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			status = MouseStatus::Press_Left_Mid_Buttom;
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			break;
		case MouseStatus::Press_Right_Mid_Buttom:
			status = MouseStatus::Press_Left_Mid_Right_Buttom;
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			break;
		default:
			break;
		}

		break;
	case Qt::RightButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			status = MouseStatus::Only_Press_Right_Buttom;
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			status = MouseStatus::Press_Left_Right_Buttom;
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			status = MouseStatus::Press_Right_Mid_Buttom;
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			status = MouseStatus::Press_Left_Mid_Right_Buttom;
			break;
		case MouseStatus::Press_Right_Mid_Buttom:	
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			break;
		default:
			break;
		}

		break;
	case Qt::MidButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			status = MouseStatus::Only_Press_Mid_Buttom;
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			status = MouseStatus::Press_Left_Mid_Buttom;
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			status = MouseStatus::Press_Right_Mid_Buttom;
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			break;
		case MouseStatus::Press_Right_Mid_Buttom:
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			status = MouseStatus::Press_Left_Mid_Right_Buttom;
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}

void MouseState::ReleaseEvent(Qt::MouseButton mb)
{
	switch (mb)
	{
	case Qt::LeftButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			status = MouseStatus::No_Press;
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			status = MouseStatus::Only_Press_Mid_Buttom;
			break;
		case MouseStatus::Press_Right_Mid_Buttom:
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			status = MouseStatus::Only_Press_Right_Buttom;
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			status = MouseStatus::Press_Right_Mid_Buttom;
			break;
		default:
			break;
		}

		break;
	case Qt::RightButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			status = MouseStatus::No_Press;
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			break;
		case MouseStatus::Press_Right_Mid_Buttom:
			status = MouseStatus::Only_Press_Mid_Buttom;
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			status = MouseStatus::Only_Press_Left_Buttom;
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			status = MouseStatus::Press_Left_Mid_Buttom;
			break;
		default:
			break;
		}

		break;
	case Qt::MidButton:
		switch (status)
		{
		case MouseStatus::No_Press:
			break;
		case MouseStatus::Only_Press_Left_Buttom:
			break;
		case MouseStatus::Only_Press_Right_Buttom:
			break;
		case MouseStatus::Only_Press_Mid_Buttom:
			status = MouseStatus::No_Press;
			break;
		case MouseStatus::Press_Left_Mid_Buttom:
			status = MouseStatus::Only_Press_Left_Buttom;
			break;
		case MouseStatus::Press_Right_Mid_Buttom:
			status = MouseStatus::Only_Press_Right_Buttom;
			break;
		case MouseStatus::Press_Left_Right_Buttom:
			break;
		case MouseStatus::Press_Left_Mid_Right_Buttom:
			status = MouseStatus::Press_Left_Right_Buttom;
			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}


void MouseState::Print()
{
	switch (status)
	{
	case MouseState::MouseStatus::No_Press:
		std::cout << "[Mouse Status] None" << std::endl;
		break;
	case MouseState::MouseStatus::Only_Press_Left_Buttom:
		std::cout << "[Mouse Status] Only Left Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Only_Press_Right_Buttom:
		std::cout << "[Mouse Status] Only Right Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Only_Press_Mid_Buttom:
		std::cout << "[Mouse Status] Only Middle Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Press_Left_Mid_Buttom:
		std::cout << "[Mouse Status] Left And Middle Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Press_Right_Mid_Buttom:
		std::cout << "[Mouse Status] Middle And Right Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Press_Left_Right_Buttom:
		std::cout << "[Mouse Status] Left And Right Buttom" << std::endl;
		break;
	case MouseState::MouseStatus::Press_Left_Mid_Right_Buttom:
		std::cout << "[Mouse Status] Left And Middle And Right Buttom" << std::endl;
		break;
	default:
		break;
	}
}
