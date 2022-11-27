#vertexshader
#version 330
layout(location = 0) in vec3 p;
layout(location = 1) in vec3 c;
uniform mat4 M = mat4(1.0);
uniform mat4 V = mat4(1.0);
uniform mat4 P = mat4(1.0);
out vec3 color;
void main() {
	color = c; 
	gl_Position = P * V * M * vec4(p, 1.0);
}


#fragmentshader
#version 330
out vec4 FragColor;
in vec3 color;
void main() {
    FragColor = vec4(color, 1.0);
}
