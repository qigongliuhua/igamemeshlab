#include "MaterialBase.h"

MaterialBase::~MaterialBase()
{
	clear();
}

void MaterialBase::set_shaders(std::vector<GLfloat>& verts, int vertex_size, std::vector<int> offset_in_one_vertex)
{
	shader.bind();
	clear();
	// 1.新建顶点数组对象
	core->glGenVertexArrays(1, &VAOs[0]);
	core->glGenBuffers(1, &VBOs[0]);
	// 2. 绑定顶点数组对象
	core->glBindVertexArray(VAOs[0]);
	// 3. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	core->glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	core->glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
	// 4. 设定顶点属性指针
	int param_size = (int)offset_in_one_vertex.size();
	int offset = 0;
	for (int idx = 0; idx < param_size; ++idx) {
		core->glVertexAttribPointer(idx, offset_in_one_vertex[idx], GL_FLOAT, GL_FALSE, vertex_size * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
		core->glEnableVertexAttribArray(idx);
		offset += offset_in_one_vertex[idx];
	}
	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);

	drawable_size = (GLuint)verts.size() / vertex_size; //绘制单元数量
}

void MaterialBase::render(Camera& camera)
{
	shader.bind();
	shader.setUniformValue("viewPos", -camera.scene_center);	// 相机世界坐标

	shader.setUniformValue("M", camera.model);	// 模型矩阵
	shader.setUniformValue("V", camera.view); // 视角矩阵
	shader.setUniformValue("P", camera.projection); // 投影矩阵


	core->glBindVertexArray(VAOs[0]);	// 前面已经记录了VBO和EBO现在只需一步调用即可
	core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core->glDrawArrays(GL_TRIANGLES, 0, drawable_size);
	core->glBindVertexArray(0);
}

void MaterialBase::render(QMatrix4x4& M, QMatrix4x4& V, QMatrix4x4& P)
{
	shader.bind();
	QMatrix4x4 ind;
	shader.setUniformValue("M", M);	// 模型矩阵
	shader.setUniformValue("V", V); // 视角矩阵
	shader.setUniformValue("P", P); // 投影矩阵


	core->glBindVertexArray(VAOs[0]);	// 前面已经记录了VBO和EBO现在只需一步调用即可
	core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core->glDrawArrays(GL_TRIANGLES, 0, drawable_size);
	core->glBindVertexArray(0);
}

void MaterialBase::render()
{
	shader.bind();
	QMatrix4x4 ind;
	shader.setUniformValue("M", ind);	// 模型矩阵
	shader.setUniformValue("V", ind); // 视角矩阵
	shader.setUniformValue("P", ind); // 投影矩阵


	core->glBindVertexArray(VAOs[0]);	// 前面已经记录了VBO和EBO现在只需一步调用即可
	core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core->glDrawArrays(GL_TRIANGLES, 0, drawable_size);
	core->glBindVertexArray(0);
}

void MaterialBase::clear()
{
	shader.bind();
	for (GLuint VAO : VAOs) {
		if (VAO != 0) {
			core->glDeleteVertexArrays(1, &VAO);
		}
	}

	for (GLuint VBO : VBOs) {
		if (VBO != 0) {
			core->glDeleteBuffers(1, &VBO);
		}
	}
}
