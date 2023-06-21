#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 Colour;
uniform int UseDepth;

float near = 0.1;
float far = 100.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{    
	if(UseDepth == 0)
	{
		color = vec4(Colour) * texture(image, TexCoords);
	}
	else
	{
		float depth = LinearizeDepth(gl_FragCoord.z) / far;
		color = vec4(vec3(depth),1.0);
	}
}  