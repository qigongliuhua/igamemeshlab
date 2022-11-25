#pragma once
#include "Mesh.h"

#include <fstream>
#include <string>
#include <sstream>
#include <array>


namespace MeshKernel {
	class IO {
	public:
		SurfaceMesh ReadObjFile(const std::string& _InputFile, int& sides_num);
		SurfaceMesh ReadOffFile(const std::string& _InputFile, int& sides_num);
		SurfaceMesh ReadStlFile(const std::string& _InputFile, int& sides_num);
		TetMesh ReadTetMeshFile(const std::string& _InputFile);
		bool WriteObjFile(const SurfaceMesh& _mesh, const std::string& _OutputFile);
		bool WriteOffFile(const SurfaceMesh& _mesh, const std::string& _OutputFile);
		bool WriteStlFile(const SurfaceMesh& _mesh, const std::string& _OutputFile);
		bool WriteMeshFile(const VolumeMesh& _mesh, const std::string& _OutputFile);
		bool WriteVtkFile(const SurfaceMesh& _mesh, const std::string& _OutputFile);
		std::string WriteOffString(const SurfaceMesh& _mesh);
		TetMesh ReadMeshFileFromStr(const std::string& data);
	private:
		void ReOrderVertexHandle(const SurfaceMesh& _mesh);
		std::vector<iGameVertexHandle> reorderedvh_;                        // 重排顶点
		std::unordered_map<iGameVertexHandle, std::size_t> newvh_;          // 新的顶点handle
	};
}

MeshKernel::SurfaceMesh MeshKernel::IO::ReadObjFile(const std::string& _InputFile, int& sides_num) {
	std::ifstream inputfile(_InputFile, std::ios::in);
	std::vector<iGameVertex> vertices;
	std::vector<std::vector<iGameVertexHandle>> faces;
	std::vector<std::vector<double>> normals;
	std::vector<std::vector<double>> uvs;
	std::unordered_map<int, int> V2N;// vertex to normal
	std::unordered_map<int, int> V2T;// vertex to uv
	std::string line;

	std::cout << "Reading " << _InputFile << " File" << std::endl;
	// std::cout << inputfile.good() << std::endl;
	while (inputfile) {
		line.clear();
		getline(inputfile, line);
		if (line[0] == '#') {
			continue;// 注释
		}
		std::stringstream linestream;
		linestream.str(line);

		std::string flag;
		linestream >> flag;
		if (flag == "v") {
			double x, y, z;
			linestream >> x >> y >> z;
			vertices.push_back(iGameVertex(x, y, z));
		}
		else if (flag == "f") {
			// f 1575/1514/1569 1581/1520/1575 1576/1515/1570
			// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
			std::vector<std::string> vex;
			std::string tmp;
			while (linestream >> tmp) vex.push_back(tmp);
			auto n = vex.size();
			sides_num = n;
			//std::cout << n << std::endl;
			// 创建面
			std::vector<iGameVertexHandle> face(n);
			for (size_t i = 0; i < n; i++) {
				size_t idx = 0;
				while (idx < vex[i].length() && std::isdigit(vex[i][idx])) idx++;
				int vh = std::stoi(vex[i].substr(0, idx)) - 1;// 注意 obj 文件 v 从1开始
				face[i] = (iGameVertexHandle)(vh);
				if (idx != vex[i].length()) {// 指定了纹理坐标
					// 注意：纹理坐标存在复用！法向量存在复用！顶点、纹理坐标与法向量个数均可互相不等
					size_t beg = idx;
					if (beg < vex[i].length() && !std::isdigit(vex[i][beg])) beg++;
					size_t end = beg;
					if (end < vex[i].length() && std::isdigit(vex[i][end])) end++;
					if (!V2T.count(vh)) {
						int uv_idx = std::stoi(vex[i].substr(beg, end - beg));// 注意 obj 文件 vt 从0开始
						V2T[vh] = uv_idx;
					}
					if (end != vex[i].length() && !V2N.count(vh)) {// 指定了法向量
						beg = end;
						if (beg < vex[i].length() && !std::isdigit(vex[i][beg])) beg++;
						end = beg;
						if (end < vex[i].length() && std::isdigit(vex[i][end])) end++;
						int n_idx = std::stoi(vex[i].substr(beg, end - beg));// 注意 obj 文件 vn 从0开始
						V2N[vh] = n_idx;
					}
				}
			}
			faces.push_back(face);
		}
		else if (flag == "vt") {
			double u, v;
			linestream >> u >> v;
			uvs.push_back({ u, v });
		}
		else if (flag == "vn") {
			double x, y, z;
			linestream >> x >> y >> z;
			normals.push_back({ x, y, z });
		}
	}
	//printf("read file success, fcnt: %d, vcnt: %d, vtcnt: %d, vncnt: %d\n", faces.size(), vertices.size(), uvs.size(), normals.size());
	if (!normals.empty()) {
		int ncnt = normals.size();
		for (int i = 0; i < vertices.size(); ++i) {
			int nidx = V2N[i];
			//if (nidx < 0 || nidx >= ncnt) printf("error: nidx = %d\n", nidx);// debug 用
			assert(nidx >= 0 && nidx < ncnt);
			vertices[i].setNormal(normals[nidx]);
		}
	}
	if (!uvs.empty()) {
		int uvcnt = uvs.size();
		for (int i = 0; i < vertices.size(); ++i) {
			int uvidx = V2T[i];
			//if (uvidx < 0 || uvidx >= uvcnt) printf("error: uvidx = %d\n", uvidx);// debug 用
			assert(uvidx >= 0 && uvidx < uvcnt);
			//vertices[i].setUV(uvs[uvidx]);
		}
	}

	// 把模型位置移到[-1,1]范围内，中心点移到原点
	double minX = std::numeric_limits<double>::max(); double maxX = std::numeric_limits<double>::min();
	double minY = std::numeric_limits<double>::max(); double maxY = std::numeric_limits<double>::min();
	double minZ = std::numeric_limits<double>::max(); double maxZ = std::numeric_limits<double>::min();
	for (auto& v : vertices) {
		minX = std::min(minX, v.x()), maxX = std::max(maxX, v.x());
		minY = std::min(minY, v.y()), maxY = std::max(maxY, v.y());
		minZ = std::min(minZ, v.z()), maxZ = std::max(maxZ, v.z());
	}
	double centerX = (minX + maxX) / 2;
	double centerY = (minY + maxY) / 2;
	double centerZ = (minZ + maxZ) / 2;
	double radius = std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ)) / 2;
	for (auto& v : vertices) {
		v.x() -= centerX;
		v.y() -= centerY;
		v.z() -= centerZ;
		v = v / radius;
	}
	auto mesh = SurfaceMesh(vertices, faces);
	//if (!normals.empty()) mesh.setHasVN(true);
	//if (!uvs.empty()) mesh.setHasVT(true);
	inputfile.close();
	return mesh;
}


MeshKernel::SurfaceMesh MeshKernel::IO::ReadOffFile(const std::string& _InputFile, int& sides_num) {
	std::ifstream inputfile(_InputFile, std::ios::in);
	std::vector<iGameVertex> vertices;
	std::vector<std::vector<iGameVertexHandle>> faces;
	std::vector<std::vector<double>> normals;
	std::vector<std::vector<double>> uvs;
	std::unordered_map<int, int> V2N;// vertex to normal
	std::unordered_map<int, int> V2T;// vertex to uv
	std::string line;
	int v_size, f_size, e_size;

	std::cout << "Reading " << _InputFile << " File" << std::endl;
	line.clear();
	getline(inputfile, line);
	std::stringstream linestream;

	if (line == "OFF") {
		line.clear();
		getline(inputfile, line);
		while(line.size() == 0 || line[0] == '#')
            getline(inputfile, line);
		linestream.str(line);
		linestream >> v_size >> f_size >> e_size;
	}
	for (int i = 0; i < v_size; i++) {
		line.clear();
		getline(inputfile, line);
		std::stringstream linestream;
		linestream.str(line);
		double x, y, z;
		linestream >> x >> y >> z;
		vertices.push_back(iGameVertex(x, y, z));
		//std::cout << x << " " << y << " " << z << std::endl;
	}
	for (int i = 0; i < f_size; i++) {
		line.clear();
		getline(inputfile, line);
		std::stringstream linestream;
		linestream.str(line);
		int v;
		linestream >> v;
		//std::cout << v << " " << x << " " << y << " " << z << std::endl;
		std::vector<iGameVertexHandle> face(v);
		sides_num = v;
		for (int i = 0; i < v; i++) {
			int temp;
			linestream >> temp;
			face[i] = (iGameVertexHandle)(temp);
		}
		faces.push_back(face);
	}
	printf("read file success, fcnt: %d, vcnt: %d, vtcnt: %d, vncnt: %d\n", faces.size(), vertices.size(), uvs.size(), normals.size());
	if (!normals.empty()) {
		int ncnt = normals.size();
		for (int i = 0; i < vertices.size(); ++i) {
			int nidx = V2N[i];
			//if (nidx < 0 || nidx >= ncnt) printf("error: nidx = %d\n", nidx);// debug 用
			assert(nidx >= 0 && nidx < ncnt);
			vertices[i].setNormal(normals[nidx]);
		}
	}
	if (!uvs.empty()) {
		int uvcnt = uvs.size();
		for (int i = 0; i < vertices.size(); ++i) {
			int uvidx = V2T[i];
			//if (uvidx < 0 || uvidx >= uvcnt) printf("error: uvidx = %d\n", uvidx);// debug 用
			assert(uvidx >= 0 && uvidx < uvcnt);
			//vertices[i].setUV(uvs[uvidx]);
		}
	}

	// 把模型位置移到[-1,1]范围内，中心点移到原点
	double minX = std::numeric_limits<double>::max(); double maxX = std::numeric_limits<double>::min();
	double minY = std::numeric_limits<double>::max(); double maxY = std::numeric_limits<double>::min();
	double minZ = std::numeric_limits<double>::max(); double maxZ = std::numeric_limits<double>::min();
	for (auto& v : vertices) {
		minX = std::min(minX, v.x()), maxX = std::max(maxX, v.x());
		minY = std::min(minY, v.y()), maxY = std::max(maxY, v.y());
		minZ = std::min(minZ, v.z()), maxZ = std::max(maxZ, v.z());
	}
	double centerX = (minX + maxX) / 2;
	double centerY = (minY + maxY) / 2;
	double centerZ = (minZ + maxZ) / 2;
	double radius = std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ)) / 2;
	for (auto& v : vertices) {
		v.x() -= centerX;
		v.y() -= centerY;
		v.z() -= centerZ;
		v = v / radius;
	}

	auto mesh = SurfaceMesh(vertices, faces);
	//if (!normals.empty()) mesh.setHasVN(true);
	//if (!uvs.empty()) mesh.setHasVT(true);
	inputfile.close();
	return mesh;
}


std::string MeshKernel::IO::WriteOffString(const SurfaceMesh& _mesh) {
	std::stringstream ret;
	ReOrderVertexHandle(_mesh);
	ret << "OFF" << std::endl;
	ret << _mesh.allvertices().size() << " " << _mesh.allfaces().size() << " " << _mesh.alledges().size() << std::endl;
	for (iGameVertexHandle vh : reorderedvh_) {
		iGameVertex v(_mesh.vertices(vh));
		ret << v.x() << " " << v.y() << " " << v.z() << std::endl;
	}
	auto allf = _mesh.allfaces();
	for (auto f : allf) {
		ret << f.second.size();
		for (int i = 0; i < f.second.size(); ++i) {
			ret << " " << newvh_[f.second.vh(i)];
		}
		ret << std::endl;
	}
	return std::string(ret.str());
}

bool MeshKernel::IO::WriteObjFile(const SurfaceMesh& _mesh, const std::string& _OutputFile) {
	std::ofstream outputfile(_OutputFile, std::ios::out);
	ReOrderVertexHandle(_mesh);
	for (iGameVertexHandle vh : reorderedvh_) {
		iGameVertex v(_mesh.vertices(vh));
		outputfile << "v " << v.x() << " " << v.y() << " " << v.z() << std::endl;
	}
	auto allf = _mesh.allfaces();
	for (auto f : allf) {
		outputfile << "f";
		for (int i = 0; i < f.second.size(); ++i) {
			outputfile << " " << newvh_[f.second.vh(i)] + 1;
		}
		outputfile << std::endl;
	}
	outputfile.close();
	return true;
}

bool MeshKernel::IO::WriteOffFile(const SurfaceMesh& _mesh, const std::string& _OutputFile) {
	std::ofstream outputfile(_OutputFile, std::ios::out);
	ReOrderVertexHandle(_mesh);
	outputfile << "OFF" << std::endl;
	outputfile << _mesh.allvertices().size() << " " << _mesh.allfaces().size() << " " << _mesh.alledges().size() << std::endl;
	for (iGameVertexHandle vh : reorderedvh_) {
		iGameVertex v(_mesh.vertices(vh));
		outputfile << v.x() << " " << v.y() << " " << v.z() << std::endl;
	}
	auto allf = _mesh.allfaces();
	for (auto f : allf) {
		outputfile << f.second.size();
		for (int i = 0; i < f.second.size(); ++i) {
			outputfile << " " << newvh_[f.second.vh(i)];
		}
		outputfile << std::endl;
	}
	outputfile.close();
	return true;
}

std::vector<std::string> SplitFileName(const std::string& fileName)
{
	// JFR DO NOT CHANGE TO std::vector<std::string> s(3), it segfaults while
	// destructor si called
	std::vector<std::string> s;
	s.resize(3);
	if (fileName.size()) {
		// returns [path, baseName, extension]
		int idot = (int)fileName.find_last_of('.');
		int islash = (int)fileName.find_last_of("/\\");
		if (idot == (int)std::string::npos) idot = -1;
		if (islash == (int)std::string::npos) islash = -1;
		if (idot > 0) s[2] = fileName.substr(idot);
		if (islash > 0) s[0] = fileName.substr(0, islash + 1);
		s[1] =
			fileName.substr(s[0].size(), fileName.size() - s[0].size() - s[2].size());
	}
	return s;
}

bool MeshKernel::IO::WriteVtkFile(const SurfaceMesh& _mesh, const std::string& _OutputFile) {
	// 还不支持约束线的输出

	std::vector<std::array<double, 3>>verts;
	std::vector<std::vector<uint32_t>> faces;
	std::vector<std::array<int, 2>> constrains;

	for (int i = 0; i < _mesh.allvertices().size(); i++) {
		std::array<double, 3> vert;
		vert[0] = _mesh.vertices(MeshKernel::iGameVertexHandle(i)).x();
		vert[1] = _mesh.vertices(MeshKernel::iGameVertexHandle(i)).y();
		vert[2] = _mesh.vertices(MeshKernel::iGameVertexHandle(i)).z();
		verts.push_back(vert);
	}

	for (auto fp : _mesh.allfaces()) {
		auto fh = fp.first;
		auto face = fp.second;
		std::vector<uint32_t> oneface;
		for (int i = 0; i < face.getVertexHandle().size(); i++) {
			oneface.push_back(face.getVertexHandle()[i]);
		}
		faces.push_back(oneface);
	}

	int numVertices = verts.size();

	FILE* fp = fopen(_OutputFile.c_str(), "w");

	std::vector<std::string> s = SplitFileName(_OutputFile);
	s.resize(3);

	if (!fp) {
		fprintf(stdout, "Unable to open file '%s'\n", _OutputFile.c_str());
		return false;
	}

	fprintf(fp, "# vtk DataFile Version 2.0\n");
	fprintf(fp, "%s, Created by labb \n", s[1].c_str());
	fprintf(fp, "ASCII\n");
	fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");

	// write mesh vertices
	fprintf(fp, "POINTS %d double\n", numVertices);
	for (std::size_t i = 0; i < verts.size(); i++)
		fprintf(fp, "%.16g %.16g %.16g\n", verts[i][0], verts[i][1], verts[i][2]);
	fprintf(fp, "\n");

	int numElements = faces.size();
	//int totalNumInt = numElements * 5;
	int totalNumInt = 0;
	for (std::size_t i = 0; i < faces.size(); i++) {
		if (faces[i].size() == 3) totalNumInt += 4;
		else if (faces[i].size() == 4) totalNumInt += 5;
	}

	for (std::size_t i = 0; i < constrains.size(); ++i) {
		++numElements;
		totalNumInt += 3;
	}

	// print vertex indices in ascii or binary
	fprintf(fp, "CELLS %d %d\n", numElements, totalNumInt);
	for (std::size_t i = 0; i < constrains.size(); ++i) {
		fprintf(fp, "%d", 2);
		fprintf(fp, " %ld", constrains[i][0]);
		fprintf(fp, " %ld", constrains[i][1]);
		fprintf(fp, "\n");
	}

	for (std::size_t i = 0; i < faces.size(); i++) {
		fprintf(fp, "%d", faces[i].size());
		for (int j = 0; j < faces[i].size(); j++)
			fprintf(fp, " %ld", faces[i][j]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	// print element types in ascii or binary
	fprintf(fp, "CELL_TYPES %d\n", numElements);
	for (std::size_t i = 0; i < numElements - faces.size(); i++) {
		fprintf(fp, "%d\n", 3);
	}
	for (std::size_t i = 0; i < faces.size(); i++) {
		if (faces[i].size() == 3) fprintf(fp, "%d\n", 5);
		else if (faces[i].size() == 4) fprintf(fp, "%d\n", 9);
	}
	fclose(fp);
	return true;
}

void MeshKernel::IO::ReOrderVertexHandle(const SurfaceMesh& _mesh) {
	auto allv = _mesh.allvertices();
	int idx = 0;
	for (auto v : allv) {
		reorderedvh_.push_back(v.first);
		newvh_[v.first] = idx++;
	}
}


MeshKernel::TetMesh MeshKernel::IO::ReadMeshFileFromStr(const std::string& data) {
	std::stringstream ss(data);
	std::vector<iGameVertex> vertices;
	std::vector<std::vector<iGameVertexHandle>> surface_faces;
	std::vector<std::vector<iGameVertexHandle>> cells;
	std::vector<iGameVertexHandle> cell;
	std::string str;
	enum State
	{
		USELESS = 1, VERTEX, FACE, TET, EDGE
	}state = USELESS;
	while (std::getline(ss, str)) {
		int len = str.length();
		std::vector<std::string>info;
		std::string s;
		for (int i = 0; i < len; i++) {
			if (str[i] == '#')break;
			if (str[i] == ' ') {
				if (s.length() > 0)
					info.push_back(s);
				s = "";
			}
			else {
				s.push_back(str[i]);
			}
		}
		if (s.length() > 0)
			info.push_back(s);
		if (info.size() == 0)continue;
		if (info[0] == "MeshVersionFormatted") {
			state = USELESS;
		}
		else if (info[0] == "Dimension") {
			std::getline(ss, str);
			state = USELESS;
		}
		else if (info[0] == "Vertices") {
			std::getline(ss, str);
			state = VERTEX;
		}
		else if (info[0] == "Tetrahedra") {
			std::getline(ss, str);
			state = TET;
		}
		else if (info[0] == "Triangles") {
			std::getline(ss, str);
			state = FACE;
		}
		else if (info[0] == "Edges") {
			std::getline(ss, str);
			state = EDGE;
		}
		else if (info[0] == "End") {
			state = USELESS;
		}
		else {
			if (state == USELESS) {
				continue;
			}
			else if (state == VERTEX) {
				vertices.push_back(MeshKernel::iGameVertex(std::stod(info[0])
					, std::stod(info[1]), std::stod(info[2])));
			}
			else if (state == TET) {
				cell = std::vector<MeshKernel::iGameVertexHandle>{
						(iGameVertexHandle)(std::stoi(info[0]) - 1)
						, (iGameVertexHandle)(std::stoi(info[1]) - 1)
						, (iGameVertexHandle)(std::stoi(info[2]) - 1)
						, (iGameVertexHandle)(std::stoi(info[3]) - 1)
				};
				cells.push_back(cell);
				cell.clear();
			}
			else if (state == FACE) {
				surface_faces.push_back(std::vector<MeshKernel::iGameVertexHandle>{
					(iGameVertexHandle)(std::stoi(info[0]) - 1)
						, (iGameVertexHandle)(std::stoi(info[1]) - 1)
						, (iGameVertexHandle)(std::stoi(info[2]) - 1)
				});
			}
		}
	}
	
	// 把模型位置移到[-1,1]范围内，中心点移到原点
	double minX = std::numeric_limits<double>::max(); double maxX = std::numeric_limits<double>::min();
	double minY = std::numeric_limits<double>::max(); double maxY = std::numeric_limits<double>::min();
	double minZ = std::numeric_limits<double>::max(); double maxZ = std::numeric_limits<double>::min();
	for (auto& v : vertices) {
		minX = std::min(minX, v.x()), maxX = std::max(maxX, v.x());
		minY = std::min(minY, v.y()), maxY = std::max(maxY, v.y());
		minZ = std::min(minZ, v.z()), maxZ = std::max(maxZ, v.z());
	}
	double centerX = (minX + maxX) / 2;
	double centerY = (minY + maxY) / 2;
	double centerZ = (minZ + maxZ) / 2;
	double radius = std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ)) / 2;
	for (auto& v : vertices) {
		v.x() -= centerX;
		v.y() -= centerY;
		v.z() -= centerZ;
		v = v / radius;
	}

	return TetMesh(vertices, cells);
}

MeshKernel::TetMesh MeshKernel::IO::ReadTetMeshFile(const std::string& _InputFile) {
	FILE* fp = fopen(_InputFile.c_str(), "r");
	char str[100];
	enum State
	{
		USELESS = 1, VERTEX, FACE, TET, EDGE
	}state = USELESS;
	std::vector<iGameVertex> vertices;
	std::vector<std::vector<iGameVertexHandle>> surface_faces;
	std::vector<std::vector<iGameVertexHandle>> cells;
	std::vector<iGameVertexHandle> cell;
	while (fscanf(fp, "%[^\n]\n", str) != EOF) {
		int len = strlen(str);
		std::vector<std::string>info;
		std::string s;
		for (int i = 0; i < len; i++) {
			if (str[i] == '#')break;
			if (str[i] == ' ') {
				if (s.length() > 0)
					info.push_back(s);
				s = "";
			}
			else {
				s.push_back(str[i]);
			}
		}
		if (s.length() > 0)
			info.push_back(s);
		if (info.size() == 0)continue;
		if (info[0] == "MeshVersionFormatted") {
			state = USELESS;
		}
		else if (info[0] == "Dimension") {
			fscanf(fp, "%[^\n]\n", str);
			state = USELESS;
		}
		else if (info[0] == "Vertices") {
			fscanf(fp, "%[^\n]\n", str);
			state = VERTEX;
		}
		else if (info[0] == "Tetrahedra") {
			fscanf(fp, "%[^\n]\n", str);
			state = TET;
		}
		else if (info[0] == "Triangles") {
			fscanf(fp, "%[^\n]\n", str);
			state = FACE;
		}
		else if (info[0] == "Edges") {
			fscanf(fp, "%[^\n]\n", str);
			state = EDGE;
		}
		else if (info[0] == "End") {
			state = USELESS;
		}
		else {
			if (state == USELESS) {
				continue;
			}
			else if (state == VERTEX) {
				vertices.push_back(MeshKernel::iGameVertex(std::stod(info[0])
					, std::stod(info[1]), std::stod(info[2])));
			}
			else if (state == TET) {
				cell = std::vector<MeshKernel::iGameVertexHandle>{
						(iGameVertexHandle)(std::stoi(info[0]) - 1)
						, (iGameVertexHandle)(std::stoi(info[1]) - 1)
						, (iGameVertexHandle)(std::stoi(info[2]) - 1)
						, (iGameVertexHandle)(std::stoi(info[3]) - 1)
				};
				cells.push_back(cell);
				cell.clear();
			}
			else if (state == FACE) {
				surface_faces.push_back(std::vector<MeshKernel::iGameVertexHandle>{
					(iGameVertexHandle)(std::stoi(info[0]) - 1)
						, (iGameVertexHandle)(std::stoi(info[1]) - 1)
						, (iGameVertexHandle)(std::stoi(info[2]) - 1)
				});
			}
		}
	}

	// 把模型位置移到[-1,1]范围内，中心点移到原点
	double minX = std::numeric_limits<double>::max(); double maxX = std::numeric_limits<double>::min();
	double minY = std::numeric_limits<double>::max(); double maxY = std::numeric_limits<double>::min();
	double minZ = std::numeric_limits<double>::max(); double maxZ = std::numeric_limits<double>::min();
	for (auto& v : vertices) {
		minX = std::min(minX, v.x()), maxX = std::max(maxX, v.x());
		minY = std::min(minY, v.y()), maxY = std::max(maxY, v.y());
		minZ = std::min(minZ, v.z()), maxZ = std::max(maxZ, v.z());
	}
	double centerX = (minX + maxX) / 2;
	double centerY = (minY + maxY) / 2;
	double centerZ = (minZ + maxZ) / 2;
	double radius = std::max(maxX - minX, std::max(maxY - minY, maxZ - minZ)) / 2;
	for (auto& v : vertices) {
		v.x() -= centerX;
		v.y() -= centerY;
		v.z() -= centerZ;
		v = v / radius;
	}

	return TetMesh(vertices, cells);
}
