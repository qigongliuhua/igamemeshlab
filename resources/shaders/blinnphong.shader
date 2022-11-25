
#vertexshader

#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	FragPos = vec3(M * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(M))) * aNormal;
	Color = aColor;
	gl_Position = P * V * vec4(FragPos, 1.0);
}

#fragmentshader

#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{
	// ambient
	float ambientStrength = 0.6;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse 
	float diffuseStrength = 0.3;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	// specular
	float specularStrength = 0.1;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 h = normalize(viewPos + lightDir); //blinn-phong
	float spec = pow(max(dot(h, norm), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) ;
	FragColor = vec4(result, 1.0);
}
