#include "M_Lines.h"
#include "ShaderUtils.h"
#include <iostream>

void M_Lines::load_shaders()
{
	ShaderUtils::LoadShaderFileFromQrc(":/shaders/shaders/flat.shader", shader);
	shader.bind();
}

void M_Lines::set_shaders(const std::vector<GLfloat>& verts_and_colors)
{
	assert(verts_and_colors.size() % 6 == 0);
	shader.bind();
	clear();


	// 1.新建顶点数组对象
	core->glGenVertexArrays(1, &VAOs[0]);
	core->glGenBuffers(1, &VBOs[0]);
	// 2. 绑定顶点数组对象
	core->glBindVertexArray(VAOs[0]);
	// 3. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	core->glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	core->glBufferData(GL_ARRAY_BUFFER, verts_and_colors.size() * sizeof(GLfloat), verts_and_colors.data(), GL_STATIC_DRAW);

	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(0);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(1);

	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);


	drawable_size = (GLuint)verts_and_colors.size() / 6; //绘制单元数量
}

void M_Lines::set_shaders(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors)
{
	assert(pos.size() == colors.size());
	assert(pos.size() % 3 == 0);

	shader.bind();
	clear();

	VBOs.resize(2, 0);
	// 1.新建顶点数组对象
	core->glGenVertexArrays(1, &VAOs[0]);
	core->glBindVertexArray(VAOs[0]);
	// 2. 绑定顶点数组对象
	core->glGenBuffers(1, &VBOs[0]);
	// 3. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	core->glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	core->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(GLfloat), pos.data(), GL_STATIC_DRAW);
	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(0);

	core->glGenBuffers(1, &VBOs[1]);
	core->glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	core->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(1);

	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);

	drawable_size = (GLuint)pos.size() / 3; //绘制单元数量
}

void M_Lines::set_shaders(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b)
{
	assert(pos.size() % 3 == 0);

	std::vector<GLfloat> colors(pos.size());

	for (int i = 0; i < pos.size() / 3; ++i) {
		colors[3 * i] = r;
		colors[3 * i + 1] = g;
		colors[3 * i + 2] = b;
	}

	set_shaders(pos, colors);
}

void M_Lines::render(Camera& camera)
{
	shader.bind();

	shader.setUniformValue("M", camera.model);	// 模型矩阵
	shader.setUniformValue("V", camera.view); // 视角矩阵
	shader.setUniformValue("P", camera.projection); // 投影矩阵

	core->glBindVertexArray(VAOs[0]);	// 前面已经记录了VBO和EBO现在只需一步调用即可
	core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core->glLineWidth(width);
	core->glDepthRange(-1, 0.999999);	// 把线框位置深度调近，防止面片盖住线框
	core->glDrawArrays(GL_LINES, 0, drawable_size);
	core->glDepthRange(-1, 1);	// 画完线框后改回原本深度
	core->glLineWidth(1);
	core->glBindVertexArray(0);
}

void M_Lines::render(QMatrix4x4& M, QMatrix4x4& V, QMatrix4x4& P)
{
	shader.bind();

	shader.setUniformValue("M", M);	// 模型矩阵
	shader.setUniformValue("V", V); // 视角矩阵
	shader.setUniformValue("P", P); // 投影矩阵

	core->glBindVertexArray(VAOs[0]);	// 前面已经记录了VBO和EBO现在只需一步调用即可
	core->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core->glLineWidth(width);
	core->glDepthRange(-1, 0.999999);	// 把线框位置深度调近，防止面片盖住线框
	core->glDrawArrays(GL_LINES, 0, drawable_size);
	core->glDepthRange(-1, 1);	// 画完线框后改回原本深度
	core->glLineWidth(1);
	core->glBindVertexArray(0);
}
