#include "A_Quadmesh.h"
#include "cinolib/io/read_write.h"


void A_Quadmesh::Render()
{
	assert(core != nullptr && camera != nullptr && light != nullptr);

	if (need_update_gl) {
		need_update_gl = false;
		if (flag_show_mesh_edge) {
			std::vector<GLfloat> verts;
			for (unsigned int eh = 0; eh < mesh.num_edges(); ++eh) {
				for (auto vh : mesh.adj_e2v(eh)) {
					auto v = mesh.vert(vh);
					auto vdata = mesh.vert_data(vh);
					verts.push_back(v.x());
					verts.push_back(v.y());
					verts.push_back(v.z());
					verts.push_back(mesh_edge_color.redF());
					verts.push_back(mesh_edge_color.greenF());
					verts.push_back(mesh_edge_color.blueF());
				}
			}
			material_mesh_edge.SetShadersData(verts);
		}

		if (flag_show_mesh_vertex) {
			std::vector<GLfloat> verts;
			for (unsigned int vid = 0; vid < mesh.num_verts(); ++vid) {
				auto v = mesh.vert(vid);
				verts.push_back(v.x());
				verts.push_back(v.y());
				verts.push_back(v.z());
				verts.push_back(mesh_vertex_color.redF());
				verts.push_back(mesh_vertex_color.greenF());
				verts.push_back(mesh_vertex_color.blueF());
			}
			material_mesh_vertex.SetShadersData(verts);
		}

		if(flag_show_mesh_face_normal || flag_show_mesh_vertex_normal || flag_show_mesh) mesh.update_normals(); //重新生成法线

		if (flag_show_mesh_face_normal) {
			std::vector<GLfloat> verts;
			for (unsigned int ph = 0; ph < mesh.num_polys(); ++ph) {
				auto center = mesh.poly_centroid(ph);
				auto pdata = mesh.poly_data(ph);
				verts.push_back(center.x());
				verts.push_back(center.y());
				verts.push_back(center.z());
				verts.push_back(mesh_face_normal_color.redF());
				verts.push_back(mesh_face_normal_color.greenF());
				verts.push_back(mesh_face_normal_color.blueF());

				verts.push_back(center.x() + pdata.normal.x() * normal_dislay_length_zoom);
				verts.push_back(center.y() + pdata.normal.y() * normal_dislay_length_zoom);
				verts.push_back(center.z() + pdata.normal.z() * normal_dislay_length_zoom);
				verts.push_back(mesh_face_normal_color.redF());
				verts.push_back(mesh_face_normal_color.greenF());
				verts.push_back(mesh_face_normal_color.blueF());
			}
			material_mesh_face_normal.SetShadersData(verts);
		}

		if (flag_show_mesh_vertex_normal) {
			std::vector<GLfloat> verts;
			for (unsigned int vh = 0; vh < mesh.num_verts(); ++vh) {
				auto center = mesh.vert(vh);
				auto vdata = mesh.vert_data(vh);
				verts.push_back(center.x());
				verts.push_back(center.y());
				verts.push_back(center.z());
				verts.push_back(mesh_vertex_normal_color.redF());
				verts.push_back(mesh_vertex_normal_color.greenF());
				verts.push_back(mesh_vertex_normal_color.blueF());

				verts.push_back(center.x() + vdata.normal.x() * normal_dislay_length_zoom);
				verts.push_back(center.y() + vdata.normal.y() * normal_dislay_length_zoom);
				verts.push_back(center.z() + vdata.normal.z() * normal_dislay_length_zoom);
				verts.push_back(mesh_vertex_normal_color.redF());
				verts.push_back(mesh_vertex_normal_color.greenF());
				verts.push_back(mesh_vertex_normal_color.blueF());
			}
			material_face_vertex_normal.SetShadersData(verts);
		}

		if (flag_show_mesh) {
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
			material_mesh.SetShadersData(verts);
		}
	}

	if (flag_show_mesh) material_mesh.Render(*camera, *light);
	if (flag_show_mesh_edge && flag_show_mesh) material_mesh_edge.Render(*camera);
	if (flag_show_mesh_vertex && flag_show_mesh) material_mesh_vertex.Render(*camera);
	if (flag_show_mesh_vertex && flag_show_mesh) material_mesh_face_normal.Render(*camera);
	if (flag_show_mesh_vertex_normal && flag_show_mesh) material_face_vertex_normal.Render(*camera);
}

void A_Quadmesh::LoadMesh(const std::string& path)
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

void A_Quadmesh::SaveMesh(const std::string& path)
{
	mesh.save(path.c_str());
}
