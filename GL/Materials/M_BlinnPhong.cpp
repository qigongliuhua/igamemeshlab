#include "M_BlinnPhong.h"
#include "ShaderUtils.h"
#include <iostream>

void M_BlinnPhong::load_shaders()
{
	ShaderUtils::LoadShaderFileFromQrc(":/shaders/shaders/blinnphong.shader", shader);
	shader.bind();
}

void M_BlinnPhong::set_shaders(const std::vector<GLfloat>& verts)
{
	assert(verts.size() % 9 == 0);

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

	core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(0);
	core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(1);
	core->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	core->glEnableVertexAttribArray(2);


	core->glBindVertexArray(0);
	core->glBindBuffer(GL_ARRAY_BUFFER, 0);


	drawable_size = (GLuint)verts.size() / 9; //绘制单元数量

}

void M_BlinnPhong::set_shaders(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& colors)
{
	assert(pos.size() == colors.size());
	assert(pos.size() % 3 == 0);

	shader.bind();
	clear();

	VBOs.resize(2, 0);
	// 1.新建顶点数组对象
	core->glGenVertexArrays(1, &VAOs[0]);
	core->glGenBuffers(1, &VBOs[0]);
	// 2. 绑定顶点数组对象
	core->glBindVertexArray(VAOs[0]);
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

void M_BlinnPhong::set_shaders(const std::vector<GLfloat>& pos, GLfloat r, GLfloat g, GLfloat b)
{
	assert(pos.size() % 3 == 0);

	shader.bind();

	std::vector<GLfloat> colors(pos.size());

	for (int i = 0; i < pos.size() / 3; ++i) {
		colors[3 * i] = r;
		colors[3 * i + 1] = g;
		colors[3 * i + 2] = b;
	}

	set_shaders(pos, colors);
}

void M_BlinnPhong::render(Camera& camera, Light& light)
{
	shader.bind();

	shader.setUniformValue("normalMode", (int)normal_mode);
	shader.setUniformValue("lightPos", light.light_pos);
	shader.setUniformValue("lightColor", light.light_color);

	MaterialBase::render(camera);
}
