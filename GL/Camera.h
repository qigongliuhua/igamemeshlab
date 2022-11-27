#pragma once

// https://github.com/mlivesu/cinolib/blob/master/include/cinolib/gl/camera.h


#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
public:

	int			width, height;	// sensor size
	QVector3D	scene_center;	// center of the scene
	double		scene_radius;	// radius of the smallest ball containing the whole scene
	double		zoom_factor;	// works for both perspective and orthographic modes
	QMatrix4x4	model;			// from OBJECT space to WORLD  space
	QMatrix4x4	view;			// from WORLD  space to CAMERA space
	QMatrix4x4	projection;		// defines the view frustum
	double fov = 26;
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	Camera(const int width_ = 640, const int height_ = 480);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	void reset();
	void reset_matrices();
	void reset_modelview();
	void reset_projection();
	void reset_projection_persp();
	void reset_projection_ortho();

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	QMatrix4x4 MV()		const;	// Model-View matrix
	QMatrix4x4 MVP()	const;	// Model-View-Projecetion matrix

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	void toggle_persp_ortho();
	bool is_ortho() const;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	void set_rotation_pivot(const QVector3D& p);
	void set_focus_point(const QVector3D& p);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	void zoom(const double& delta);
	void rotate(const double& deg, const QVector3D& axis);
	void rotate_x(const double& deg);
	void rotate_y(const double& deg);
	void rotate_z(const double& deg);
	void translate(const QVector3D& delta);
	void translate_x(const double& delta);
	void translate_y(const double& delta);
	void translate_z(const double& delta);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	std::string serialize() const;
	void        deserialize(const std::string& s);

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

	void print() const;

	// STATIC FACILITIES FOR MATRIX SYNTHESIS ::::::::::::::::::::::::::::::::

	static QMatrix4x4 look_at(const QVector3D& eye,    // camera pos
	const QVector3D& center, // center of scene (rotation pivot)
	const QVector3D& up);    // upwards direction

	static QMatrix4x4 frustum_ortho(const double& l,   // left
	const double& r,   // right
	const double& b,   // bottom
	const double& t,   // top
	const double& n,   // near
	const double& f);  // far

	static QMatrix4x4 frustum_persp(const double& l,   // left
	const double& r,   // right
	const double& b,   // bottom
	const double& t,   // top
	const double& n,   // near
	const double& f);  // far

	static QMatrix4x4 frustum_persp(const double& fov, // vertical field of view (degrees)
	const double& ar,  // aspect ratio
	const double& n,   // near
	const double& f);  // far

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
};

