#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 pro;
uniform mat4 view;
out vec3 _worldNormals;

void main()
{
    TexCoords = texCord;
	_worldNormals = (model * vec4(1.0,1.0,1.0, 0.0)).xyz;
	
    gl_Position = pro * model * vec4(vertex, 1.0);
}