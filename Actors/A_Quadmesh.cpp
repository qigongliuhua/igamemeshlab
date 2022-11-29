#include "A_Quadmesh.h"
#include "cinolib/io/read_write.h"


void A_Quadmesh::render()
{
	assert(core != nullptr && camera != nullptr && light != nullptr);

	if (need_update_gl) {
		need_update_gl = false;
		if (flag_show_lines) {
			std::vector<GLfloat> verts;
			for (unsigned int eh = 0; eh < mesh.num_edges(); ++eh) {
				for (auto vh : mesh.adj_e2v(eh)) {
					auto v = mesh.vert(vh);
					auto vdata = mesh.vert_data(vh);
					verts.push_back(v.x());
					verts.push_back(v.y());
					verts.push_back(v.z());
					verts.push_back(lines_color.redF());
					verts.push_back(lines_color.greenF());
					verts.push_back(lines_color.blueF());
				}
			}
			material_line.set_shaders(verts);
		}

		if (flag_show_points) {
			std::vector<GLfloat> verts;
			for (unsigned int vid = 0; vid < mesh.num_verts(); ++vid) {
				auto v = mesh.vert(vid);
				verts.push_back(v.x());
				verts.push_back(v.y());
				verts.push_back(v.z());
				verts.push_back(point_color.redF());
				verts.push_back(point_color.greenF());
				verts.push_back(point_color.blueF());
			}
			material_point.set_shaders(verts);
		}

		if(flag_face_normal_lines || flag_face_normal_lines || flag_show_model) mesh.update_normals(); //重新生成法线

		if (flag_face_normal_lines) {
			std::vector<GLfloat> verts;
			for (unsigned int ph = 0; ph < mesh.num_polys(); ++ph) {
				auto center = mesh.poly_centroid(ph);
				auto pdata = mesh.poly_data(ph);
				verts.push_back(center.x());
				verts.push_back(center.y());
				verts.push_back(center.z());
				verts.push_back(face_normal_lines_color.redF());
				verts.push_back(face_normal_lines_color.greenF());
				verts.push_back(face_normal_lines_color.blueF());

				verts.push_back(center.x() + pdata.normal.x() * normal_lines_length);
				verts.push_back(center.y() + pdata.normal.y() * normal_lines_length);
				verts.push_back(center.z() + pdata.normal.z() * normal_lines_length);
				verts.push_back(face_normal_lines_color.redF());
				verts.push_back(face_normal_lines_color.greenF());
				verts.push_back(face_normal_lines_color.blueF());
			}
			material_face_normal_lines.set_shaders(verts);
		}

		if (flag_vertex_normal_lines) {
			std::vector<GLfloat> verts;
			for (unsigned int vh = 0; vh < mesh.num_verts(); ++vh) {
				auto center = mesh.vert(vh);
				auto vdata = mesh.vert_data(vh);
				verts.push_back(center.x());
				verts.push_back(center.y());
				verts.push_back(center.z());
				verts.push_back(vertex_normal_lines_color.redF());
				verts.push_back(vertex_normal_lines_color.greenF());
				verts.push_back(vertex_normal_lines_color.blueF());

				verts.push_back(center.x() + vdata.normal.x() * normal_lines_length);
				verts.push_back(center.y() + vdata.normal.y() * normal_lines_length);
				verts.push_back(center.z() + vdata.normal.z() * normal_lines_length);
				verts.push_back(vertex_normal_lines_color.redF());
				verts.push_back(vertex_normal_lines_color.greenF());
				verts.push_back(vertex_normal_lines_color.blueF());
			}
			material_vertex_normal_lines.set_shaders(verts);
		}

		if (flag_show_model) {
			std::vector<GLfloat> verts;
			for (unsigned int pid = 0; pid < mesh.num_polys(); ++pid) {
				for (uint i = 0; i < mesh.poly_tessellation(pid).size(); ++i) {


					auto v = mesh.vert(mesh.poly_tessellation(pid).at(i));
					auto vdata = mesh.vert_data(mesh.poly_tessellation(pid).at(i));
					verts.push_back(v.x());
					verts.push_back(v.y());
					verts.push_back(v.z());
					verts.push_back(vdata.normal.x());
					verts.push_back(vdata.normal.y());
					verts.push_back(vdata.normal.z());
					verts.push_back(vdata.color.r);
					verts.push_back(vdata.color.g);
					verts.push_back(vdata.color.b);
				}
			}
			material.set_shaders(verts);
		}
	}

	if (flag_show_model) material.render(*camera, *light);
	if (flag_show_lines && flag_show_model) material_line.render(*camera);
	if (flag_show_points && flag_show_model) material_point.render(*camera);
	if (flag_face_normal_lines && flag_show_model) material_face_normal_lines.render(*camera);
	if (flag_vertex_normal_lines && flag_show_model) material_vertex_normal_lines.render(*camera);
}

void A_Quadmesh::load_model(const std::string& path)
{
	mesh.load(path.c_str());
	mesh.normalize_bbox();
	mesh.translate(-mesh.centroid());
	mesh.update_bbox();
	filetype = path.substr(path.find_last_of(".") + 1);
	filename = path.substr(path.find_last_of("/\\") + 1);
	filepath = path.substr(0, path.size() - filename.size());
	filename = filename.substr(0, filename.size() - filetype.size() - 1);

}

void A_Quadmesh::save_model(const std::string& path)
{
	mesh.save(path.c_str());
}
