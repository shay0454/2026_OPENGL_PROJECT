#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 texCoords;

uniform mat4  modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

uniform vec3 faceColor;



out vec3 vColor;
out vec2 vUV;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(position, 1.0);
	vColor =  faceColor;
	vUV = texCoords;

	FragPos = vec3(modelTransform * vec4(position, 1.0));
	Normal = transpose(inverse(mat3(modelTransform))) * vNormal;
} 