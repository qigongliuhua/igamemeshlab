#include "Camera.h"
#include <QVector4D>
#include <sstream> 
#include <iostream> 

Camera::Camera(const int width_, const int height_)
{
	width = width_;
	height = height_;
    scene_center = QVector3D(0, 0, 0);
    scene_radius = 1;
    reset();
}


void Camera::reset()
{
    zoom_factor = 0.5;
    reset_matrices();
}

void Camera::reset_matrices()
{
    reset_modelview();
    //reset_projection();
	//reset_projection_ortho();
	reset_projection_persp();
}

void Camera::reset_modelview()
{
    // set the scene center at the WORLD origin
    // set the camera outside of the scene radius along WORLD's -Z
    QMatrix4x4 ind1, ind2;
    ind1.translate(-scene_center);
    model = ind1;
    ind2.translate(-QVector3D(0, 0, 4 * scene_radius));
    view = ind2;
}

void Camera::reset_projection()
{
    if (is_ortho()) reset_projection_ortho();
    else            reset_projection_persp();
}


void Camera::reset_projection_persp() 
{
    static constexpr double fov = 90; // vertical field of view, in degrees.
    // A 55 degrees fov in perspective mode
    // is roughly equivalent to no zoom in
    // orthographic mode


    projection = frustum_persp(fov * zoom_factor,    // vertical field of view
        (double)width / height, // aspect ratio
        0.01 * scene_radius,       // near
        100 * scene_radius);      // far
}


void Camera::reset_projection_ortho()
{
    double ar = (double)width / height; // aspect ratio

    projection = frustum_ortho(-2 * scene_radius * zoom_factor * ar, // left
        2 * scene_radius * zoom_factor * ar, // right
        -2 * scene_radius * zoom_factor,      // bottom
        2 * scene_radius * zoom_factor,      // top
        0.01 * scene_radius,                    // near
        100 * scene_radius);                   // far
}

void Camera::set_rotation_pivot(const QVector3D& p)
{
    // map the pivot to the WORLD space, then position its mapped
    // coordinates at the origin the coordinate reference system
    //

    QVector3D Mp = (model * p.toVector4D()).toVector3D();
    QMatrix4x4 ind;
    ind.translate(-Mp);
    model = ind * model;
}

void Camera::set_focus_point(const QVector3D& p)
{
    set_rotation_pivot(p);
    zoom(-zoom_factor * 0.5);
}

void Camera::zoom(const double& delta)
{
    auto clamp = [](double a, double mi, double ma)->double {
        return std::max(std::min(a, ma), mi);
    };

    zoom_factor += delta;
    zoom_factor = clamp(zoom_factor, 1e-20, 1e3);
    reset_projection();
}

void Camera::rotate(const double& deg, const QVector3D& axis)
{
    QMatrix4x4 R;
    R.rotate(deg, axis); // deg角度制
    model = R * model;
}

void Camera::rotate_x(const double& deg)
{
    rotate(deg, QVector3D(1, 0, 0));
}

void Camera::rotate_y(const double& deg)
{
    rotate(deg, QVector3D(0, 1, 0));
}

void Camera::rotate_z(const double& deg)
{
    rotate(deg, QVector3D(0, 0, 1));
}

void Camera::translate(const QVector3D& delta)
{
    QMatrix4x4 Tx;
    Tx.translate(delta);
    model = Tx * model;
}

void Camera::translate_x(const double& delta)
{
    translate(QVector3D(delta, 0, 0));
}

void Camera::translate_y(const double& delta)
{
    translate(QVector3D(0, delta, 0));
}

void Camera::translate_z(const double& delta)
{
    translate(QVector3D(0, 0, delta));
}

QMatrix4x4 Camera::MV() const
{
    return view * model;
}

QMatrix4x4 Camera::MVP() const
{
    return projection * MV();
}

std::string Camera::serialize() const
{
    std::stringstream ss;
    ss << width << " " << height << " "
        << scene_center[0] << " " << scene_center[1] << " " << scene_center[2] << " "
        << scene_radius << " "
        << zoom_factor << " "
        << model.row(0)[0] << " " << model.row(0)[1] << " " << model.row(0)[2] << " " << model.row(0)[3] << " "
        << model.row(1)[0] << " " << model.row(1)[1] << " " << model.row(1)[2] << " " << model.row(1)[3] << " "
        << model.row(2)[0] << " " << model.row(2)[1] << " " << model.row(2)[2] << " " << model.row(2)[3] << " "
        << model.row(3)[0] << " " << model.row(3)[1] << " " << model.row(3)[2] << " " << model.row(3)[3] << " "
        << view.row(0)[0] << " " << view.row(0)[1] << " " << view.row(0)[2] << " " << view.row(0)[3] << " "
        << view.row(1)[0] << " " << view.row(1)[1] << " " << view.row(1)[2] << " " << view.row(1)[3] << " "
        << view.row(2)[0] << " " << view.row(2)[1] << " " << view.row(2)[2] << " " << view.row(2)[3] << " "
        << view.row(3)[0] << " " << view.row(3)[1] << " " << view.row(3)[2] << " " << view.row(3)[3] << " "
        << projection.row(0)[0] << " " << projection.row(0)[1] << " " << projection.row(0)[2] << " " << projection.row(0)[3] << " "
        << projection.row(1)[0] << " " << projection.row(1)[1] << " " << projection.row(1)[2] << " " << projection.row(1)[3] << " "
        << projection.row(2)[0] << " " << projection.row(2)[1] << " " << projection.row(2)[2] << " " << projection.row(2)[3] << " "
        << projection.row(3)[0] << " " << projection.row(3)[1] << " " << projection.row(3)[2] << " " << projection.row(3)[3];
    return ss.str();
}

void Camera::deserialize(const std::string& s)
{
    std::stringstream ss(s);
    ss >> width
        >> height
        >> scene_center[0] >> scene_center[1] >> scene_center[2]
        >> scene_radius
        >> zoom_factor
        >> model.data()[0] >> model.data()[1] >> model.data()[2] >> model.data()[3]
        >> model.data()[4] >> model.data()[5] >> model.data()[6] >> model.data()[7]
        >> model.data()[8] >> model.data()[9] >> model.data()[10] >> model.data()[11]
        >> model.data()[12] >> model.data()[13] >> model.data()[14] >> model.data()[15]
        >> view.data()[0] >> view.data()[1] >> view.data()[2] >> view.data()[3]
		>> view.data()[4] >> view.data()[5] >> view.data()[6] >> view.data()[7]
		>> view.data()[8] >> view.data()[9] >> view.data()[10] >> view.data()[11]
		>> view.data()[12] >> view.data()[13] >> view.data()[14] >> view.data()[15]
        >> projection.data()[0] >> projection.data()[1] >> projection.data()[2] >> projection.data()[3]
        >> projection.data()[4] >> projection.data()[5] >> projection.data()[6] >> projection.data()[7]
        >> projection.data()[8] >> projection.data()[9] >> projection.data()[10] >> projection.data()[11]
        >> projection.data()[12] >> projection.data()[13] >> projection.data()[14] >> projection.data()[15];
}


void Camera::toggle_persp_ortho()
{
    if (is_ortho()) reset_projection_persp();
    else           reset_projection_ortho();
}

bool Camera::is_ortho() const
{
    return (projection.row(3)[2] == 0);
}

void Camera::print() const
{
    QMatrix4x4 MV = view * model;
    std::cout << "\nCamera M:\n"
        << model.row(0)[0] << " " << model.row(0)[1] << " " << model.row(0)[2] << " " << model.row(0)[3] << std::endl
        << model.row(1)[0] << " " << model.row(1)[1] << " " << model.row(1)[2] << " " << model.row(1)[3] << std::endl
        << model.row(2)[0] << " " << model.row(2)[1] << " " << model.row(2)[2] << " " << model.row(2)[3] << std::endl
        << model.row(3)[0] << " " << model.row(3)[1] << " " << model.row(3)[2] << " " << model.row(3)[3] << std::endl
        << "\nCamera V:\n"
        << view.row(0)[0] << " " << view.row(0)[1] << " " << view.row(0)[2] << " " << view.row(0)[3] << std::endl
        << view.row(1)[0] << " " << view.row(1)[1] << " " << view.row(1)[2] << " " << view.row(1)[3] << std::endl
        << view.row(2)[0] << " " << view.row(2)[1] << " " << view.row(2)[2] << " " << view.row(2)[3] << std::endl
        << view.row(3)[0] << " " << view.row(3)[1] << " " << view.row(3)[2] << " " << view.row(3)[3] << std::endl
        << "\nCamera MV:\n"
        << MV.row(0)[0] << " " << MV.row(0)[1] << " " << MV.row(0)[2] << " " << MV.row(0)[3] << std::endl
        << MV.row(1)[0] << " " << MV.row(1)[1] << " " << MV.row(1)[2] << " " << MV.row(1)[3] << std::endl
        << MV.row(2)[0] << " " << MV.row(2)[1] << " " << MV.row(2)[2] << " " << MV.row(2)[3] << std::endl
        << MV.row(3)[0] << " " << MV.row(3)[1] << " " << MV.row(3)[2] << " " << MV.row(3)[3] << std::endl
        << "\nCamera PR:\n"
        << projection.row(0)[0] << " " << projection.row(0)[1] << " " << projection.row(0)[2] << " " << projection.row(0)[3] << std::endl
        << projection.row(1)[0] << " " << projection.row(1)[1] << " " << projection.row(1)[2] << " " << projection.row(1)[3] << std::endl
        << projection.row(2)[0] << " " << projection.row(2)[1] << " " << projection.row(2)[2] << " " << projection.row(2)[3] << std::endl
        << projection.row(3)[0] << " " << projection.row(3)[1] << " " << projection.row(3)[2] << " " << projection.row(3)[3] << std::endl;
}

QMatrix4x4 Camera::look_at(const QVector3D& eye,    // camera pos
    const QVector3D& center, // center of scene (rotation pivot)
    const QVector3D& up)     // upwards direction
{

    QMatrix4x4 ind;
    ind.lookAt(eye, center, up);
    return ind;
}

QMatrix4x4 Camera::frustum_ortho(const double& l, // left
    const double& r, // right
    const double& b, // bottom
    const double& t, // top
    const double& n, // near
    const double& f) // far
{
    QMatrix4x4 ind;
    ind.ortho(l, r, b, t, n, f);
    return ind;
}

QMatrix4x4 Camera::frustum_persp(const double& l, // left
    const double& r, // right
    const double& b, // bottom
    const double& t, // top
    const double& n, // near
    const double& f) // far
{
    QMatrix4x4 ind;
    ind.frustum(l, r, b, t, n, f);
    return ind;
}

QMatrix4x4 Camera::frustum_persp(const double& fov, // vertical field of view (in degrees)
    const double& ar,  // aspect ratio
    const double& n,   // near
    const double& f)   // far
{
    QMatrix4x4 ind;
    ind.perspective(fov, ar, n, f);
    return ind;
}
