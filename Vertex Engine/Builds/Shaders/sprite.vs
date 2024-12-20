#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 pro;
out vec3 _worldNormals;

void main()
{
    TexCoords = vertex.zw;
	_worldNormals = (model * vec4(1.0,1.0,1.0, 0.0)).xyz;
	
    gl_Position = pro * model * vec4(vertex.xy, 0.0, 1.0);
}