#include "iGameMeshLab.h"
#include <QColorDialog>
#include< QFileDialog>
#include "MaterialEnum.h"

iGameMeshLab::iGameMeshLab(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connect(ui.Btn_Background_color, SIGNAL(clicked()), this, SLOT(change_background_color()));
	connect(ui.Btn_model_line_color, SIGNAL(clicked()), this, SLOT(change_model_line_color()));
	connect(ui.Btn_model_point_color, SIGNAL(clicked()), this, SLOT(change_model_point_color()));
	connect(ui.Btn_reset_all_color, SIGNAL(clicked()), this, SLOT(reset_all_color()));
	connect(ui.camera_fov, SIGNAL(valueChanged(int)), this, SLOT(change_camera_fov(int)));
	connect(ui.camera_persp_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(change_camera_persp_mode(int)));
	connect(ui.reset_camera, SIGNAL(clicked()), this, SLOT(reset_camera()));
	connect(ui.normalMode_flat, SIGNAL(clicked()), this, SLOT(set_normalmode_flat()));
	connect(ui.normalMode_smooth, SIGNAL(clicked()), this, SLOT(set_normalmode_smooth()));
	connect(ui.toggle_coor, SIGNAL(clicked()), this, SLOT(toggle_world_coor()));

	connect(ui.slice_x_inverse, SIGNAL(stateChanged(int)), this, SLOT(slice_x_inverse_slot(int)));
	connect(ui.slice_y_inverse, SIGNAL(stateChanged(int)), this, SLOT(slice_y_inverse_slot(int)));
	connect(ui.slice_z_inverse, SIGNAL(stateChanged(int)), this, SLOT(slice_z_inverse_slot(int)));
	connect(ui.slice_x_rate, SIGNAL(valueChanged(int)), this, SLOT(slice_x_rate_slot(int)));
	connect(ui.slice_y_rate, SIGNAL(valueChanged(int)), this, SLOT(slice_y_rate_slot(int)));
	connect(ui.slice_z_rate, SIGNAL(valueChanged(int)), this, SLOT(slice_z_rate_slot(int)));

	ui.openGLWidget1->sceneManger = new SceneManger(ui.sceneList);
	sceneManger = ui.openGLWidget1->sceneManger;
	sceneManger->set_list_widget(ui.sceneList);

	std::function<void(SceneModelItem*)> change_focus_listener = [&](SceneModelItem* item) {
		if (item == nullptr || item->get_mesh() == nullptr) {
			ui.slice_x_inverse->setChecked(false);
			ui.slice_y_inverse->setChecked(false);
			ui.slice_z_inverse->setChecked(false);
			ui.slice_x_rate->setValue((int)0);
			ui.slice_y_rate->setValue((int)0);
			ui.slice_z_rate->setValue((int)0);
		}
		else {
			double xr, yr, zr;
			bool xi, yi, zi;
			item->get_mesh()->get_slice_status(xr, yr, zr, xi, yi, zi);
			ui.slice_x_inverse->setChecked(xi);
			ui.slice_y_inverse->setChecked(yi);
			ui.slice_z_inverse->setChecked(zi);
			ui.slice_x_rate->setValue((int)xr);
			ui.slice_y_rate->setValue((int)yr);
			ui.slice_z_rate->setValue((int)zr);
		}
		
	};

	sceneManger->set_change_focus_item_listener(change_focus_listener);

	connect(ui.menuBar, SIGNAL(triggered(QAction*)), this, SLOT(MenuClicked(QAction*)));
}



iGameMeshLab::~iGameMeshLab()
{}

void iGameMeshLab::change_model_line_color()
{
	if (sceneManger->last_clicked_item->get_mesh() == nullptr) return;
	QColor m_color;

	QColor defaultColor(255, 255, 255, 255);
	QColorDialog colorDlg(this);
	colorDlg.setGeometry(pos().x() + width() * 0.3, pos().y() + height() * 0.3, 300, 280);
	colorDlg.setWindowTitle(QStringLiteral("选择线框颜色"));
	colorDlg.setCurrentColor(defaultColor);
	if (colorDlg.exec() == QColorDialog::Accepted) {
		m_color = colorDlg.selectedColor();
		sceneManger->last_clicked_item->get_mesh()->set_lines_color(m_color);
	}
	ui.openGLWidget1->update();
}

void iGameMeshLab::change_model_point_color()
{
	if (sceneManger->last_clicked_item->get_mesh() == nullptr) return;
	QColor m_color;

	QColor defaultColor(255, 255, 255, 255);
	QColorDialog colorDlg(this);
	colorDlg.setGeometry(pos().x() + width() * 0.3, pos().y() + height() * 0.3, 300, 280);
	colorDlg.setWindowTitle(QStringLiteral("选择顶点颜色"));
	colorDlg.setCurrentColor(defaultColor);
	if (colorDlg.exec() == QColorDialog::Accepted) {
		m_color = colorDlg.selectedColor();
		sceneManger->last_clicked_item->get_mesh()->set_points_color(m_color);
	}
	ui.openGLWidget1->update();
}

void iGameMeshLab::reset_all_color()
{
	ui.openGLWidget1->background_color = QColor(255, 255, 255);
	ui.openGLWidget1->need_update_shaders = true;
	if (sceneManger->last_clicked_item->get_mesh()) {
		sceneManger->last_clicked_item->get_mesh()->set_lines_color(QColor(0, 0, 0));
		sceneManger->last_clicked_item->get_mesh()->set_points_color(QColor(255, 0, 0));
	}
	ui.openGLWidget1->update();
}

void iGameMeshLab::change_camera_fov(int value)
{
	ui.openGLWidget1->camera.fov = value;
	ui.openGLWidget1->camera.reset_projection();
	ui.openGLWidget1->update();
}

void iGameMeshLab::change_camera_persp_mode(int value)
{
	if (value == 0) { //透视模式
		ui.openGLWidget1->camera.reset_projection_persp();
	}
	else if(value == 1){ //正交模式
		ui.openGLWidget1->camera.reset_projection_ortho();
	}
	ui.openGLWidget1->update();
}

void iGameMeshLab::reset_camera()
{
	ui.openGLWidget1->camera.reset();
	ui.openGLWidget1->update();
}

void iGameMeshLab::set_normalmode_flat()
{
	if (sceneManger->last_clicked_item->get_mesh() == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_normal_mode(Normal_Mode::Flat);
	ui.openGLWidget1->update();
}

void iGameMeshLab::set_normalmode_smooth()
{
	if (sceneManger->last_clicked_item->get_mesh() == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_normal_mode(Normal_Mode::Smooth);

	ui.openGLWidget1->update();
}


void iGameMeshLab::toggle_world_coor()
{
	ui.openGLWidget1->world_coor.toggle_show();
	ui.openGLWidget1->update();
}

void iGameMeshLab::slice_x_inverse_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_inverse_x(!!value);
}

void iGameMeshLab::slice_y_inverse_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_inverse_y(!!value);
}

void iGameMeshLab::slice_z_inverse_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_inverse_z(!!value);
}

void iGameMeshLab::slice_x_rate_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_rate_x((double)value);
}

void iGameMeshLab::slice_y_rate_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_rate_y((double)value);
}

void iGameMeshLab::slice_z_rate_slot(int value)
{
	if (sceneManger->last_clicked_item == nullptr) return;
	sceneManger->last_clicked_item->get_mesh()->set_slice_rate_z((double)value);
}

void iGameMeshLab::MenuClicked(QAction* action)
{
	if (action == ui.open_trimesh) //打开三角表面网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj or Off od Stl Files(*.obj *.off *.stl)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_trimesh(filePath.toStdString());
		sceneManger->add_item(item);
	}
	else if (action == ui.open_quadmesh) //打开四边形表面网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj or Off od Stl Files(*.obj *.off *.stl)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_quadmesh(filePath.toStdString());
		sceneManger->add_item(item);
	}
	else if (action == ui.open_polymesh) //打开多边形表面网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj or Off od Stl Files(*.obj *.off *.stl)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_polygonmesh(filePath.toStdString());
		sceneManger->add_item(item);
	}
	else if (action == ui.open_tetmesh) //打开四面体网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("mesh,vtu,vtk,tet(*.mesh *.vtu *.vtk *.tet *.MESH *.VTU *.VTK *.TET)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_tetmesh(filePath.toStdString());
		sceneManger->add_item(item);
	}	
	else if (action == ui.open_quadtimesh) //打开六面体网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("mesh,vtu,vtk,tet(*.mesh *.vtu *.vtk *.tet *.MESH *.VTU *.VTK *.TET)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_hexmesh(filePath.toStdString());
		sceneManger->add_item(item);
	}
	else if (action == ui.open_polytimesh) //打开多面体网格
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("hybrid,hedra,mesh,vtu,vtk,tet(*.hybrid *.HYBRID *.hedra *.HEDRA *.mesh *.vtu *.vtk *.tet *.MESH *.VTU *.VTK *.TET)"));
		//QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), tr(""), tr("Obj Files(*.obj)"));
		if (filePath.isEmpty())
			return;

		SceneModelItem* item = new SceneModelItem();
		item->load_polyhedralmesh(filePath.toStdString());
		sceneManger->add_item(item);
	}
	ui.openGLWidget1->update();
}

void iGameMeshLab::change_background_color()
{
	QColor m_color;

	QColor defaultColor(255, 255, 255, 255);
	QColorDialog colorDlg(this);
	colorDlg.setGeometry(pos().x() + width() * 0.3, pos().y() + height() * 0.3, 300, 280);
	colorDlg.setWindowTitle(QStringLiteral("选择背景颜色"));
	colorDlg.setCurrentColor(defaultColor);
	if (colorDlg.exec() == QColorDialog::Accepted) {
		m_color = colorDlg.selectedColor();
		ui.openGLWidget1->background_color = m_color;
		ui.openGLWidget1->need_update_shaders = true;
	}
}
