#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 Colour;
uniform int UseDepth;
void main()
{    
	if(UseDepth == 0)
	{
		color = vec4(Colour) * texture(image, TexCoords);
	}
	else
	{
		color = vec4(vec3(gl_FragCoord.z),1.0);
	}
}  